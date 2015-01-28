/*
 *  IW0HDV Extio
 *
 *  Copyright 2015 by Andrea Montefusco IW0HDV
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

 /** 
 * @file extio_airspy.cpp
 * @brief extio_airspy.cpp : Defines the exported functions for the DLL application.
 * @author Andrea Montefusco IW0HDV
 * @version 0.0
 * @date 2015-01-07
 */
 
#include "log.h"
#include "extio_airspy.h"
#include "gui_splashscreen.h"
#include "gui_control.h"


ExtIODll singleton;

ExtIODll :: ExtIODll (): 
    Extio(0), pExr(0), pSplash(0), pGui(0) 
{
	fprintf (stderr, "%s\n", "ExtioDll AirSpy DEFAULT ctor");
    Dll::Register (this);	
}
	
//
// Global: the class fabric function
// defined in dllmain.h
// 
//DLL_CLASS(ExtIODll)


bool ExtIODll::InitHW(char *name, char *model, int & extio_type)
{
	static bool first = true;
	EXTIO_BASE_TYPE extio_type_;

	extio_type =  extio_type_.value;

	extio_type_.dummy = true;
	
	if (first) {
		first = false;
		
		pSplash = new AirSpySplash(0, 0);

		pExr = new ExtioAirSpyRadio<EXTIO_BASE_TYPE> (EXTIO_NS, &extioCallback );

		if  (pExr == 0 || !pExr->status()) {
			pSplash->SetStatus("Unable to find receiver ! [%s]", pExr ? pExr->last_error() : "");
			pSplash->Show();
			LOGT("%s\n", "Error creating instance.");
			return false;
		}
	}

	strcpy(name, "AIRSPY");
	strcpy(model, "A" ); 
	return extio_type_.dummy;
}


bool ExtIODll::OpenHW(void)
{
	bool rc = false;
	
	if  (pExr && pExr->status()) {
		if (pExr->open() == 0) {
			pGui = new AirSpyCtrlGui (pExr);
			if (pGui) pGui->Show ();
			rc = true;
			pSplash->Hide(); 
			delete pSplash; pSplash = 0;
		} else {
			pSplash->SetStatus("Unable to open receiver ! [%s]", pExr->last_error() );
			pSplash->Show();
		}
	} else {
		pSplash->Show();
	}
	
	return rc;	
}


void ExtIODll::CloseHW(void)
{

	LOGT("1Instance #%d %p %p %p\n", GetInstanceNumber(), pExr, pSplash, pGui);
	//LOG_CLOSE; is found in dllmain.cpp
	
	LOGT("2Instance #%d %p %p %p\n", GetInstanceNumber(), pExr, pSplash, pGui);
	if (pSplash) delete pSplash, pSplash = 0;

	LOGT("3Instance #%d %p %p %p\n", GetInstanceNumber(), pExr, pSplash, pGui);
	if (pGui) delete pGui, pGui = 0;

	LOGT("4Instance #%d %p %p %p\n", GetInstanceNumber(), pExr, pSplash, pGui);
	if (pExr) delete pExr, pExr = 0;

	return;
}


int  ExtIODll::StartHW(long freq)
{
	LOGT("EXTIO_NS: %d  EXTIO_BASE_TYPE_SIZE: %d N:%d\n", EXTIO_NS , EXTIO_BASE_TYPE_SIZE , 2);

	if (pExr) pExr->start(EXTIO_NS * EXTIO_BASE_TYPE_SIZE * 2);

	return EXTIO_NS; // # of samples returned by callback
}


void ExtIODll::StopHW(void)
{
	if (GetInstanceNumber() == 1) {
		LOGT("%s\n", "Stopping async data acquisition...");
		if (pExr) pExr->stop();
	}
	return;
}


int ExtIODll::SetHWLO(long freq)
{
/* Parameter freq shall be between 24000000(24MHz) and 1750000000(1.75GHz) */
	if (freq < 24000000) return -10000;
	if (freq > 1750000000) return 1750000000;
	
	if (pExr) pExr->set_frequency (freq);

	return 0;
}


long ExtIODll::GetHWLO(void)
{
	if (pExr) return pExr->get_frequency();
		else  return 0;
}


long ExtIODll::GetHWSR(void)
{
	int sr;
	if (pExr) {
		sr = pExr->get_sample_rate();
		LOGT("returned sr: %d\n", sr);
		return sr;
	} else
		return 0;
}

int ExtIODll::GetStatus(void)
{
	return 0;
}

void ExtIODll::ShowGUI(void)
{
	if ( GetInstanceNumber() == 1 ) {
		if (pSplash) pSplash->Show();
		if (pGui) pGui->Show ();
	}
	return;
}

void ExtIODll::HideGUI(void)
{
	if ( GetInstanceNumber() == 1 ) {
		if (pGui) pGui->Hide ();
		if (pSplash) pSplash->Hide();
	}
	return;
}