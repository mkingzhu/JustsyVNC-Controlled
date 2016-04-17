// Copyright (C) 2009,2010,2011,2012 GlavSoft LLC.
// All rights reserved.
//
//-------------------------------------------------------------------------
// This file is part of the TightVNC software.  Please visit our Web site:
//
//                       http://www.tightvnc.com/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//-------------------------------------------------------------------------
//

#include "TvnServerApplication.h"
#include "ServerCommandLine.h"
#include "TvnServerHelp.h"

#include "thread/GlobalMutex.h"

#include "util/ResourceLoader.h"
#include "util/StringTable.h"
#include "tvnserver-app/NamingDefs.h"
#include "win-system/WinCommandLineArgs.h"

#include "network/RfbInputGate.h"
#include "network/RfbOutputGate.h"
#include "network/socket/SocketStream.h"

#include "tvnserver/resource.h"

#include "util/AnsiStringStorage.h"

TvnServerApplication::TvnServerApplication(HINSTANCE hInstance,
                                           const TCHAR *windowClassName,
                                           const TCHAR *commandLine,
                                           NewConnectionEvents *newConnectionEvents)
: WindowsApplication(hInstance, windowClassName),
  m_fileLogger(true),
  m_tvnServer(0),
  m_commandLine(commandLine),
  m_newConnectionEvents(newConnectionEvents)
{
  m_confirmDialog = new ConfirmDialog(this);
}

TvnServerApplication::~TvnServerApplication()
{
}

void TvnServerApplication::setIp(const StringStorage &ip)
{
  m_ip.setString(ip.getString());
}

StringStorage TvnServerApplication::getIp() const
{
  StringStorage deviceId(m_ip);
  return deviceId;
}

void TvnServerApplication::setUser(const StringStorage &user)
{
  m_user.setString(user.getString());
}

StringStorage TvnServerApplication::getUser() const
{
  StringStorage user(m_user);
  return user;
}

void TvnServerApplication::setNeedConfirm(const StringStorage &needConfirm)
{
  m_needConfirm.setString(needConfirm.getString());
}

StringStorage TvnServerApplication::getNeedConfirm() const
{
  StringStorage needConfirm(m_needConfirm);
  return needConfirm;
}

void TvnServerApplication::setMagic(const StringStorage &magic)
{
  m_magic.setString(magic.getString());
}

StringStorage TvnServerApplication::getMagic() const
{
  StringStorage magic(m_magic);
  return magic;
}

int TvnServerApplication::run()
{
  // Reject 2 instances of TightVNC server application.
  GlobalMutex *appInstanceMutex;
  try {
    appInstanceMutex = new GlobalMutex(
      ServerApplicationNames::SERVER_INSTANCE_MUTEX_NAME, false, true);
  } catch (...) {
    MessageBox(0,
               StringTable::getString(IDS_SERVER_ALREADY_RUNNING),
               StringTable::getString(IDS_MBC_TVNSERVER), MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }

  try {
    if (m_needConfirm.isEqualTo(_T("1"))) {
      m_confirmDialog->show();
    }
    else {
      onConfirm(true);
    }
    
    int exitCode = WindowsApplication::run();

    if (m_tvnServer) {
      m_tvnServer->removeListener(this);
      delete m_tvnServer;
    }
    if (appInstanceMutex) {
      delete appInstanceMutex;
    }
    return exitCode;
  } catch (Exception &e) {
    // FIXME: Move string to resource
    StringStorage message;
    message.format(_T("Couldn't run the server: %s"), e.getMessage());
    MessageBox(0,
               message.getString(),
               _T("Server error"), MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }
}

void TvnServerApplication::onTvnServerShutdown()
{
  WindowsApplication::shutdown();
}

void TvnServerApplication::onLogInit(const TCHAR *logDir, const TCHAR *fileName,
                                     unsigned char logLevel)
{
  m_fileLogger.init(logDir, fileName, logLevel);
  m_fileLogger.storeHeader();
}

void TvnServerApplication::onChangeLogProps(const TCHAR *newLogDir, unsigned char newLevel)
{
  m_fileLogger.changeLogProps(newLogDir, newLevel);
}

void TvnServerApplication::onConfirm(bool confirmed)
{
  SocketIPv4 *socket;
  try {
    socket = connectToServer();
    writeHead(socket, confirmed);
    if (confirmed) {
      m_tvnServer = new TvnServer(false, m_newConnectionEvents, this, &m_fileLogger, this);
      m_tvnServer->addListener(this);
      m_tvnServer->getRfbClientManager()->addNewConnection(socket, new ViewPortState, false, false);
    }
    else {
      socket->close();
      WindowsApplication::shutdown();
    }
  }
  catch (Exception &ex) {
    MessageBox(0,
               _T("Can not connect to server"),
               _T("Server error"),
               MB_OK | MB_ICONEXCLAMATION);

    if (socket)
      socket->close();
    exit(0);
  }
}

SocketIPv4 *TvnServerApplication::connectToServer()
{
  SocketAddressIPv4 ipAddress(m_ip.getString(), 5900);

  StringStorage ipAddressString;
  ipAddress.toString(&ipAddressString);

  SocketIPv4 *socket = new SocketIPv4;
  socket->connect(ipAddress);
  socket->enableNaggleAlgorithm(false);

  DWORD timeout = 60000;
  socket->setSocketOptions(SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
  socket->setSocketOptions(SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
  return socket;
}

void TvnServerApplication::writeHead(SocketIPv4 *socket, bool confirmed)
{
  SocketStream *socketStream = new SocketStream(socket);

  RfbInputGate *input = new RfbInputGate(socketStream);
  RfbOutputGate *output = new RfbOutputGate(socketStream);

  static AnsiStringStorage header("JUSTSY-VNC-CONTROLLED");
  static AnsiStringStorage magicKey("MAGIC");
  static AnsiStringStorage confirmedKey("CONFIRMED");

  AnsiStringStorage magic(&m_magic);
  AnsiStringStorage confirmedStr(confirmed ? "1" : "0");

  INT32 length = sizeof(INT32) + header.getLength()
    + 2 * sizeof(INT32) + magicKey.getLength() + magic.getLength()
    + 2 * sizeof(INT32) + confirmedKey.getLength() + confirmedStr.getLength();

  output->writeInt32(length);
  output->writeFully(header.getString(), header.getLength());

  output->writeInt32(magicKey.getLength());
  output->writeFully(magicKey.getString(), magicKey.getLength());
  output->writeInt32(magic.getLength());
  output->writeFully(magic.getString(), magic.getLength());

  output->writeInt32(confirmedKey.getLength());
  output->writeFully(confirmedKey.getString(), confirmedKey.getLength());
  output->writeInt32(confirmedStr.getLength());
  output->writeFully(confirmedStr.getString(), confirmedStr.getLength());

  output->flush();

  length = input->readInt32();
  if (8 != length)
    throw Exception();
  INT32 statusCode = input->readInt32();
  if (200 != statusCode)
    throw Exception();
}
