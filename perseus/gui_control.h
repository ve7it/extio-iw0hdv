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

#if !defined __GUI_CONTROL_H__
#define __GUI_CONTROL_H__

#include "gui.h"
#include "extio_config.h"
#include "extio_perseus.h"

class PerseusCtrlGui: public Gui {
public:
	PerseusCtrlGui (ExtioPerseusRadio2 < EXTIO_BASE_TYPE > *);
	~PerseusCtrlGui () {}

	//void Show ();
	void EnableControls();
	void DisableControls();

	virtual bool OnInit(const GuiEvent& ev);
	virtual bool ButtonClick(const GuiEvent &ev);
	//virtual bool OnHScroll(const GuiEventHScroll& ev);
	//virtual bool OnWmUser(int n, const GuiEvent& ev);
	bool ComboBoxSelChange(const GuiEvent &ev);


private:
	//int sr;
	ExtioPerseusRadio2 < EXTIO_BASE_TYPE > *pr_;
};



#endif