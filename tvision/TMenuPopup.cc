/*
 * TMenuPopup.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TEvent
#define Uses_TRect
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TMenuPopup
#include <tv.h>

TMenuPopup::TMenuPopup(TRect & bounds, TMenu *aMenu):
TMenuBox(bounds, aMenu, nullptr)
{
}

void TMenuPopup::handleEvent(TEvent & event)
{
	switch (event.what) {
	case evKeyDown:
		TMenuItem * p = findItem(getCtrlChar(event.keyDown.keyCode));
		if (!p)
			p = hotKey(event.keyDown.keyCode);
		if (p && commandEnabled(p->command)) {
			event.what = evCommand;
			event.message.command = p->command;
			event.message.infoPtr = nullptr;
			putEvent(event);
			clearEvent(event);
		} else if (getAltChar(event.keyDown.keyCode))
			clearEvent(event);
		break;
	}
	TMenuBox::handleEvent(event);
}
