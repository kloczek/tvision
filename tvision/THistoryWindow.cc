/*
 * THistoryWindow.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_THistoryWindow
#define Uses_THistoryViewer
#include <tv.h>

THistInit::THistInit(TListViewer *(*cListViewer) (TRect, TWindow *, ushort)):
createListViewer(cListViewer)
{
}

#define cpHistoryWindow "\x13\x13\x15\x18\x17\x13\x14"

THistoryWindow::THistoryWindow(const TRect & bounds,
			       ushort historyId):TWindowInit(&THistoryWindow::
							     initFrame),
THistInit(&THistoryWindow::initViewer), TWindow(bounds, nullptr, wnNoNumber)
{
	flags = wfClose;
	if (createListViewer != nullptr &&
	    (viewer =
	     createListViewer(getExtent(), this, historyId)) != nullptr)
		insert(viewer);
}

TPalette & THistoryWindow::getPalette()const
{
	static TPalette palette(cpHistoryWindow, sizeof(cpHistoryWindow) - 1);
	return palette;
}

void THistoryWindow::getSelection(char *dest)
{
	viewer->getText(dest, viewer->focused, 255);
}

TListViewer *THistoryWindow::initViewer(TRect r, TWindow *win, ushort historyId)
{
	r.grow(-1, -1);
	return new THistoryViewer(r,
				  win->
				  standardScrollBar(sbHorizontal |
						    sbHandleKeyboard),
				  win->
				  standardScrollBar(sbVertical |
						    sbHandleKeyboard),
				  historyId);
}
