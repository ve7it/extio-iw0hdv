/**
 *  IW0HDV Extio
 *
 *  Copyright 2015 by Andrea Montefusco IW0HDV
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#if defined _MSC_VER || defined __MINGW32__
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment (lib, "ws2_32.lib")
// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")
#include <windowsx.h>
#include <commctrl.h>			// Include header
#pragma comment(lib, "comctl32.lib")	// Include library
#pragma warning( disable : 4995 )
#endif

#if defined _MSC_VER
#include <strsafe.h>
#endif


#include "util.h"
#include "perseus.h"
#include "log.h"
#include "log.h"
#include "dllmain.h" // for GetMyHandle()
#include "Extio_config.h"
#include "ExtIO_perseus.h"
#include "guievent.h"
#include "guiutil.h"
#include "gui_impl.h" 
#include "gui.h" 
#include "perseusw.h"
#include "gui_splashscreen.h"

PerseusSplash::PerseusSplash(Gui **p, CommandReceiver **ppCr):
	Gui(IDD_SPLASHSCREEN)
{
	OnInit(GuiEvent(pi->hDialog, -1));
	LOGT("**** PerseusSplash: pImpl: %p Gui addr: %p\n", pi, this);
}

PerseusSplash :: ~PerseusSplash()
{
	LOGT("**** PerseusSplash: pImpl: %p Gui addr: %p\n", pi, this);
}


bool PerseusSplash::OnInit(const GuiEvent& ev)
{
	AppendWinTitle(GuiEvent(pi->hDialog, 0), getBuildString());
	SetStatus ("%s", "");
	return true;
}


void PerseusSplash::AppendMessage(const char *pszBuf)
{
	PostMessage(pi->hDialog, WM_USER + 3, reinterpret_cast<WPARAM>(xstrdup(pszBuf)), static_cast<LPARAM>(0));
}


template <typename... ARGS>
void PerseusSplash::SetStatus(const char *format, ARGS... args)
{
	char szBuf[1024];

	snprintf(szBuf, sizeof(szBuf), format, args...);
	PostMessage(pi->hDialog, WM_USER + 2, reinterpret_cast<WPARAM>(xstrdup(szBuf)), static_cast<LPARAM>(0));
}

#if 0
int PerseusSplash::ScanStarted()
{
	SetStatus("%s", "Searching for hardware..."); 
	return 0; 
}

int PerseusSplash::ScanStopped(int nh)
{ 
	if (nh == 0) SetStatus("%s", "Search done. No device found"); 
	else SetStatus("Search done. %d device(s) found", nh);
	return 0; 
}
#endif

#if 0
int	PerseusSplash::InterfaceFound(Ethernet::NetInterface *pni)	
{
	AppendMessage(pni->name);  
	return 0;
}

int	PerseusSplash::DeviceFound(Ethernet::Device *pd)		
{
	// append a clear text (progress) message
	AppendMessage(pd->board_id); 
	// append to the list box
	char szBuf[1024];
	snprintf(szBuf, sizeof(szBuf), "%s - %s", pd->board_id, pd->ip_address);
	PostMessage(pi->hDialog, WM_USER + 4, reinterpret_cast<WPARAM>(xstrdup(szBuf)), static_cast<LPARAM>(0) );
	pDev = pd;
	return 0; 
}
#endif

void PerseusSplash::Show()
{
	int x, y;

	if (pi && pi->hDialog) {
		WSize(pi->hDialog).center(x, y);
		SetWindowPos(pi->hDialog, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
		ShowWindow(pi->hDialog, SW_SHOW);
	}
}



bool PerseusSplash::OnWmUser(int n, const GuiEvent& ev)
{

	char * pszText = reinterpret_cast<char *>(ev.id);

	if (n == 2 && pszText)	{
		SetWindowText(GetDlgItem(ev.hWnd, IDC_SPLASH_MSG), (LPCTSTR)(char *)pszText);
		BringWindowToTop(ev.hWnd);
		xstrdel(pszText, __LINE__);
		return TRUE;
	} 
#if 0
	else
	if (n == 3 && pszText) {
		AppendTextToEditCtrl(GuiEvent(ev.hWnd, IDC_SPLSH_MSG_PANE), pszText);
		AppendTextToEditCtrl(GuiEvent(ev.hWnd, IDC_SPLSH_MSG_PANE), "\r\n");
		BringWindowToTop(ev.hWnd);
		xstrdel(pszText, __LINE__);
		return TRUE;		
	} else 
	if (n == 4 && pszText)	{
		SendMessage(GetDlgItem(ev.hWnd, IDC_LBOX_RADIO_FOUND), LB_INSERTSTRING, -1 /* Index */, (LPARAM)pszText);
		BringWindowToTop(ev.hWnd);
		xstrdel(pszText, __LINE__);
		return TRUE;		
	}
#endif	
	return FALSE;
}

