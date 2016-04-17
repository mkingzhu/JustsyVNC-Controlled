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

#ifndef _TVN_SERVER_APPLICATION_H_
#define _TVN_SERVER_APPLICATION_H_

#include "thread/GlobalMutex.h"

#include "util/CommonHeader.h"
#include "util/winhdr.h"

#include "network/socket/SocketIPv4.h"

#include "win-system/WindowsApplication.h"

#include "TvnServer.h"
#include "TvnServerListener.h"
#include "WsConfigRunner.h"
#include "log-writer/FileLogger.h"
#include "LogInitListener.h"
#include "ConfirmDialog.h"

class ConfirmDialog;

/**
 * Windows TightVNC server application.
 * Used for running TightVNC server as single windows application.
 */
class TvnServerApplication : public WindowsApplication,
                             public TvnServerListener,
                             private LogInitListener
{
public:
  /**
   * Creates TightVNC server application instance.
   * @param hInstance HINSTANCE of application.
   * @param commaneLine command line string.
   */
  TvnServerApplication(HINSTANCE hInstance,
                       const TCHAR *windowClassName,
                       const TCHAR *commandLine,
                       NewConnectionEvents *newConnectionEvents);
  /**
   * Deletes TightVNC server application instance.
   */
  virtual ~TvnServerApplication();

  void setIp(const StringStorage &ip);
  StringStorage getIp() const;

  void setUser(const StringStorage &user);
  StringStorage getUser() const;

  void setNeedConfirm(const StringStorage &needConfirm);
  StringStorage getNeedConfirm() const;

  void setMagic(const StringStorage &magic);
  StringStorage getMagic() const;

  void onConfirm(bool confirmed);

  /**
   * Runs TightVNC server windows application.
   *
   * Makes several things:
   *   1) Starts TigthVNC server.
   *   2) Starts TvnControl application.
   *   3) Enters main windows message loop.
   *   4) Stops TigthVNC server.
   *
   * @return application exit code.
   */
  virtual int run();

  /**
   * Inherited from TvnServerListener abstact class.
   *
   * Shutdowns WindowsApplication.
   */
  virtual void onTvnServerShutdown();

private:
  // This is a callback function that calls when the log can be initialized.
  virtual void onLogInit(const TCHAR *logDir, const TCHAR *fileName, unsigned char logLevel);

  // This is a callback function that calls when log properties have changed.
  virtual void onChangeLogProps(const TCHAR *newLogDir, unsigned char newLevel);

  SocketIPv4 *connectToServer();
  void writeHead(SocketIPv4 *socket, bool confirmed);
  void start(SocketIPv4 *socket);
  void closePreConnection();

  StringStorage m_ip;
  StringStorage m_user;
  StringStorage m_needConfirm;
  StringStorage m_magic;

  FileLogger m_fileLogger;

  /**
   * Command line string.
   */
  StringStorage m_commandLine;
  /**
   * TightVNC server.
   */
  TvnServer *m_tvnServer;

  GlobalMutex *m_appInstanceMutex;

  NewConnectionEvents *m_newConnectionEvents;

  ConfirmDialog *m_confirmDialog;
};

#endif
