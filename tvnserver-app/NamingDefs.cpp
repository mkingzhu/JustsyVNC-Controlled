// Copyright (C) 2012 GlavSoft LLC.
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

#include "NamingDefs.h"

const TCHAR NamingDefs::MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME[] =
  _T("MIRROR_DRIVER_MESSAGE_WINDOW_CLASS_NAME");

const TCHAR ProductNames::PRODUCT_NAME[] = _T("JustsyVNC");
const TCHAR ProductNames::SERVER_PRODUCT_NAME[] = _T("JustsyVNC Server");

const TCHAR RegistryPaths::SERVER_PATH[] = _T("Software\\JustsyVNC\\Server");
const TCHAR RegistryPaths::SERVER_REVERSE_CONN_HISTORY_PATH[] =
  _T("Software\\JustsyVNC\\Control\\ReverseConnectionHistory");
const TCHAR RegistryPaths::DISPATCHER_CONN_HISTORY_PATH[] =
  _T("Software\\JustsyVNC\\Control\\DispatcherConnectionHistory");

const TCHAR WindowNames::WINDOW_CLASS_NAME[] =
  _T("JvnWindowsApplicationClass");

const TCHAR LogNames::SERVER_LOG_FILE_STUB_NAME[] = _T("jvnserver");
const TCHAR LogNames::VIEWER_LOG_FILE_STUB_NAME[] = _T("jvnviewer");
const TCHAR LogNames::LOG_DIR_NAME[] = _T("JustsyVNC");
const TCHAR LogNames::LOG_PIPE_PUBLIC_NAME[] = _T("JVN_log_pipe_public_name");
const TCHAR LogNames::WIN_EVENT_PROVIDER_NAME[] = _T("jvnserver");

const TCHAR ClipboardNames::CLIPBOARD_WIN_CLASS_NAME[] = 
  _T("Jvnserver.Clipboard.MessageWindow");


const TCHAR ServiceNames::TVNCONTROL_START_REGISTRY_ENTRY_NAME[] =
  _T("jvncontrol");
const TCHAR ServiceNames::SERVICE_NAME[] = _T("jvnserver");
const TCHAR ServiceNames::SERVICE_NAME_TO_DISPLAY[] = _T("JustsyVNC Server");

const TCHAR ServerApplicationNames::SERVER_INSTANCE_MUTEX_NAME[] =
  _T("jvnserverApplication");
const TCHAR ServerApplicationNames::CONTROL_APP_INSTANCE_MUTEX_NAME[] =
  _T("jvnserverServiceControlSlave");
const TCHAR ServerApplicationNames::FOR_SERVICE_CONTROL_APP_PIPE_NAME[] =
  _T("JustsyVNC_Service_Control");
const TCHAR ServerApplicationNames::FOR_APP_CONTROL_APP_SERVICE_PIPE_NAME[] =
  _T("JustsyVNC_Application_Control");

const TCHAR HookDefinitions::HOOK_LOADER_WINDOW_CLASS[] =
  _T("JvnHookLoaderWindowClass");
const TCHAR HookDefinitions::HOOK_TARGET_WIN_CLASS_NAME[] =
  _T("HookTargetWinClassName");
const TCHAR HookDefinitions::HOOK_LOADER_NAME[] = _T("hookldr.exe");
const UINT HookDefinitions::LOADER_CLOSE_CODE =
  RegisterWindowMessage(_T("JVN.HOOK.LOADER.CLOSE.CODE"));
const UINT HookDefinitions::SPEC_IPC_CODE =
  RegisterWindowMessage(_T("JVN.HOOK.MESSAGE.CODE"));

const TCHAR DefaultNames::DEFAULT_COMPUTER_NAME[] = _T("JustsyVNC Server");


const char HttpStrings::HTTP_INDEX_PAGE_FORMAT[] =
"<HTML>\n"
"  <HEAD><TITLE>JustsyVNC desktop [%.256s]</TITLE></HEAD>\n"
"  <BODY>\n"
"    <APPLET ARCHIVE=\"justsyvnc-jviewer.jar\""
" CODE=\"com.glavsoft.viewer.Viewer\" WIDTH=1 HEIGHT=1>\n"
"      <PARAM NAME=\"PORT\" VALUE=\"%d\">\n"
"      <PARAM NAME=\"OpenNewWindow\" VALUE=\"YES\">\n"
"%.1024s"
"    </APPLET><BR>\n"
"    <A HREF=\"http://www.justsyvnc.com/\">www.JustsyVNC.com</A>\n"
"  </BODY>\n"
"</HTML>\n";

const char HttpStrings::HTTP_MSG_BADPARAMS[] =
"<HTML>\n"
"  <HEAD><TITLE>JustsyVNC desktop</TITLE></HEAD>\n"
"  <BODY>\n"
"    <H1>Bad Parameters</H1>\n"
"    The sequence of applet parameters specified within the URL is invalid.\n"
"  </BODY>\n"
"</HTML>\n";
