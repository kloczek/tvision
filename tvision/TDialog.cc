/*
 * TDialog.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TDialog
#define Uses_TEvent
#include <tv.h>

// TMultiCheckboxes flags
//   hibyte = number of bits
//   lobyte = bit mask

TDialog::TDialog(const TRect & bounds,
		 const char *aTitle):TWindowInit(&TDialog::initFrame),
TWindow(bounds, aTitle, wnNoNumber)
{
	growMode = 0;
	flags = wfMove | wfClose;
	palette = dpGrayDialog;
}

TPalette & TDialog::getPalette()const
{
	static TPalette paletteGray(cpGrayDialog, sizeof(cpGrayDialog) - 1);
	static TPalette paletteBlue(cpBlueDialog, sizeof(cpBlueDialog) - 1);
	static TPalette paletteCyan(cpCyanDialog, sizeof(cpCyanDialog) - 1);

	switch (palette) {
	case dpGrayDialog:
		return paletteGray;
	case dpBlueDialog:
		return paletteBlue;
	case dpCyanDialog:
		return paletteCyan;
	default:
		;
	}
	return paletteGray;
}

void TDialog::handleEvent(TEvent & event)
{
	TWindow::handleEvent(event);
	switch (event.what) {
	case evKeyDown:
		switch (event.keyDown.keyCode) {
		case kbEsc:
			event.what = evCommand;
			event.message.command = cmCancel;
			event.message.infoPtr = nullptr;
			putEvent(event);
			clearEvent(event);
			break;
		case kbEnter:
			event.what = evBroadcast;
			event.message.command = cmDefault;
			event.message.infoPtr = nullptr;
			putEvent(event);
			clearEvent(event);
			break;
		default:
			;
		}
		break;

	case evCommand:
		switch (event.message.command) {
		case cmOK:
		case cmCancel:
		case cmYes:
		case cmNo:
			if ((state & sfModal) != 0) {
				endModal(event.message.command);
				clearEvent(event);
			}
			break;
		default:
			;
		}
		break;
	default:
		;
	}
}

Boolean TDialog::valid(ushort command)
{
	if (command == cmCancel)
		return True;
	else
		return TGroup::valid(command);
}

#if !defined(NO_STREAMABLE)

TStreamable *TDialog::build()
{
	return new TDialog(streamableInit);
}

TDialog::TDialog(StreamableInit):
TWindowInit(nullptr), TWindow(streamableInit)
{
}

#endif
