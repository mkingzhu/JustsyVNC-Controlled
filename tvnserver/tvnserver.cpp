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

#include "util/CommonHeader.h"
#include "util/winhdr.h"
#include "util/CommandLine.h"
#include "win-system/WinCommandLineArgs.h"
#include "win-system/WinProcessCommandLine.h"

#include "tvnserver-app/TvnService.h"
#include "tvnserver-app/TvnServerApplication.h"
#include "tvnserver-app/QueryConnectionApplication.h"
#include "tvnserver-app/DesktopServerApplication.h"
#include "tvnserver-app/AdditionalActionApplication.h"
#include "tvnserver-app/ServiceControlApplication.h"
#include "tvnserver-app/ServiceControlCommandLine.h"
#include "tvnserver-app/QueryConnectionCommandLine.h"
#include "tvnserver-app/DesktopServerCommandLine.h"

#include "tvncontrol-app/ControlApplication.h"
#include "tvncontrol-app/ControlCommandLine.h"

#include "tvnserver/resource.h"
#include "tvnserver-app/CrashHook.h"
#include "tvnserver-app/NamingDefs.h"

#include "tvnserver-app/WinEventLogWriter.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine, int nCmdShow)
{
  WinProcessCommandLine m_wpcl;
  if (5 != m_wpcl.getArgumentsCount())
    return 0;

  bool isOK = true;
  StringStorage strIp;
  isOK &= m_wpcl.getArgument(1, &strIp);
  StringStorage strUser;
  isOK &= m_wpcl.getArgument(2, &strUser);
  StringStorage strNeedConfirm;
  isOK &= m_wpcl.getArgument(3, &strNeedConfirm);
  StringStorage strMagic;
  isOK &= m_wpcl.getArgument(4, &strMagic);

  if (!isOK)
    return 0;

  LogWriter preLog(0);

  // Life time of the sysLog must be greater than a TvnService object
  // because the crashHook uses it but fully functional usage possible
  // only after the TvnService object start.
  WinEventLogWriter winEventLogWriter(&preLog);
  CrashHook crashHook(&winEventLogWriter);

  ResourceLoader resourceLoaderSingleton(hInstance);

  // No additional applications, run TightVNC server as single application.
  crashHook.setGuiEnabled();
  TvnServerApplication tvnServer(hInstance,
    WindowNames::WINDOW_CLASS_NAME,
    lpCmdLine, &winEventLogWriter);
  tvnServer.setIp(strIp);
  tvnServer.setUser(strUser);
  tvnServer.setNeedConfirm(strNeedConfirm);
  tvnServer.setMagic(strMagic);

  return tvnServer.run();
}
