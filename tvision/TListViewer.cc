/*
 * TListViewer.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TListViewer
#define Uses_TScrollBar
#define Uses_TDrawBuffer
#define Uses_TPoint
#define Uses_TEvent
#define Uses_TGroup
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <string.h>

#define cpListViewer "\x1A\x1A\x1B\x1C\x1D"

TListViewer::TListViewer(const TRect & bounds,
			 ushort aNumCols,
			 TScrollBar *aHScrollBar,
			 TScrollBar *aVScrollBar):TView(bounds),
numCols(aNumCols), topItem(0), focused(0), range(0)
{
	short arStep, pgStep;

	options |= ofFirstClick | ofSelectable;
	eventMask |= evBroadcast;
	if (aVScrollBar != nullptr) {
		if (numCols == 1) {
			pgStep = size.y - 1;
			arStep = 1;
		} else {
			pgStep = size.y * numCols;
			arStep = size.y;
		}
		aVScrollBar->setStep(pgStep, arStep);
	}

	if (aHScrollBar != nullptr)
		aHScrollBar->setStep(size.x / numCols, 1);

	hScrollBar = aHScrollBar;
	vScrollBar = aVScrollBar;
}

void TListViewer::changeBounds(const TRect & bounds)
{
	TView::changeBounds(bounds);
	if (hScrollBar != nullptr)
		hScrollBar->setStep(size.x / numCols, hScrollBar->arStep);
#ifndef __UNPATCHED
	if (vScrollBar != nullptr)
		vScrollBar->setStep(size.y, vScrollBar->arStep);
#else
	if (vScrollBar != nullptr)
		vScrollBar->setStep(size.y, hScrollBar->arStep);
#endif
}

void TListViewer::draw()
{
	short i, j, item;
	ushort normalColor, selectedColor, focusedColor = 0, color;
	short colWidth, curCol, indent;
	TDrawBuffer b;
	uchar scOff;

	if ((state & (sfSelected | sfActive)) == (sfSelected | sfActive)) {
		normalColor = getColor(1);
		focusedColor = getColor(3);
		selectedColor = getColor(4);
	} else {
		normalColor = getColor(2);
		selectedColor = getColor(4);
	}

	if (hScrollBar != nullptr)
		indent = hScrollBar->value;
	else
		indent = 0;

	colWidth = size.x / numCols + 1;
	for (i = 0; i < size.y; i++) {
		for (j = 0; j < numCols; j++) {
			item = j * size.y + i + topItem;
			curCol = j * colWidth;
			if ((state & (sfSelected | sfActive)) ==
			    (sfSelected | sfActive) && focused == item
			    && range > 0) {
				color = focusedColor;
				setCursor(curCol + 1, i);
				scOff = 0;
			} else if (item < range && isSelected(item)) {
				color = selectedColor;
				scOff = 2;
			} else {
				color = normalColor;
				scOff = 4;
			}

			b.moveChar(curCol, ' ', color, colWidth);
			if (item < range) {
				char text[256];
				getText(text, item, colWidth + indent);
				char buf[256];
				memmove(buf, text + indent, colWidth);
				buf[colWidth] = EOS;
				b.moveStr(curCol + 1, buf, color);
				if (showMarkers) {
					b.putChar(curCol, specialChars[scOff]);
					b.putChar(curCol + colWidth - 2,
						  specialChars[scOff + 1]);
				}
			} else if (i == 0 && j == 0)
				b.moveStr(curCol + 1, emptyText, getColor(1));

			b.moveChar(curCol + colWidth - 1, separatorChar,
				   getColor(5), 1);
		}
		writeLine(0, i, size.x, 1, b);
	}
}

void TListViewer::focusItem(short item)
{
	focused = item;
	if (vScrollBar != nullptr)
		vScrollBar->setValue(item);
	else
		drawView();
	if (item < topItem) {
		if (numCols == 1)
			topItem = item;
		else
			topItem = item - item % size.y;
	} else {
		if (item >= topItem + size.y * numCols) {
			if (numCols == 1)
				topItem = item - size.y + 1;
			else
				topItem =
				    item - item % size.y -
				    (size.y * (numCols - 1));
		}
	}
}

void TListViewer::focusItemNum(short item)
{
	if (item < 0)
		item = 0;
	else if (item >= range && range > 0)
		item = range - 1;

	if (range != 0)
		focusItem(item);
}

TPalette & TListViewer::getPalette()const
{
	static TPalette palette(cpListViewer, sizeof(cpListViewer) - 1);
	return palette;
}

void TListViewer::getText(char *dest, short, short)
{
	*dest = EOS;
}

Boolean TListViewer::isSelected(short item)
{
	return Boolean(item == focused);
}

void TListViewer::handleEvent(TEvent & event)
{
	TPoint mouse;
	ushort colWidth;
	short oldItem, newItem;
	ushort count;
	int mouseAutosToSkip = 4;

	TView::handleEvent(event);

	if (event.what == evMouseDown) {
		colWidth = size.x / numCols + 1;
		oldItem = focused;
		mouse = makeLocal(event.mouse.where);
		if (mouseInView(event.mouse.where))
			newItem =
			    mouse.y + (size.y * (mouse.x / colWidth)) + topItem;
		else
			newItem = oldItem;
		count = 0;
		do {
			if (newItem != oldItem) {
				focusItemNum(newItem);
				drawView();
			}
			oldItem = newItem;
			mouse = makeLocal(event.mouse.where);
			if (mouseInView(event.mouse.where))
				newItem =
				    mouse.y + (size.y * (mouse.x / colWidth)) +
				    topItem;
			else {
				if (numCols == 1) {
					if (event.what == evMouseAuto)
						count++;
					if (count == mouseAutosToSkip) {
						count = 0;
						if (mouse.y < 0)
							newItem = focused - 1;
						else if (mouse.y >= size.y)
							newItem = focused + 1;
					}
				} else {
					if (event.what == evMouseAuto)
						count++;
					if (count == mouseAutosToSkip) {
						count = 0;
						if (mouse.x < 0)
							newItem =
							    focused - size.y;
						else if (mouse.x >= size.x)
							newItem =
							    focused + size.y;
						else if (mouse.y < 0)
							newItem =
							    focused -
							    focused % size.y;
						else if (mouse.y > size.y)
							newItem =
							    focused -
							    focused % size.y +
							    size.y - 1;
					}
				}
			}
			if (event.mouse.eventFlags & meDoubleClick)
				break;
		} while (mouseEvent(event, evMouseMove | evMouseAuto));
		focusItemNum(newItem);
		drawView();
		if ((event.mouse.eventFlags & meDoubleClick) && range > newItem)
			selectItem(newItem);
		clearEvent(event);
	} else if (event.what == evKeyDown) {
		if (event.keyDown.charScan.charCode == ' ' && focused < range) {
			selectItem(focused);
			newItem = focused;
		} else {
			switch (ctrlToArrow(event.keyDown.keyCode)) {
			case kbUp:
				newItem = focused - 1;
				break;
			case kbDown:
				newItem = focused + 1;
				break;
			case kbRight:
				if (numCols > 1)
					newItem = focused + size.y;
				else
					return;
				break;
			case kbLeft:
				if (numCols > 1)
					newItem = focused - size.y;
				else
					return;
				break;
			case kbPgDn:
				newItem = focused + size.y * numCols;
				break;
			case kbPgUp:
				newItem = focused - size.y * numCols;
				break;
			case kbHome:
				newItem = topItem;
				break;
			case kbEnd:
				newItem = topItem + (size.y * numCols) - 1;
				break;
			case kbCtrlPgDn:
				newItem = range - 1;
				break;
			case kbCtrlPgUp:
				newItem = 0;
				break;
			default:
				return;
			}
		}
		focusItemNum(newItem);
		drawView();
		clearEvent(event);
	} else if (event.what == evBroadcast) {
		if ((options & ofSelectable) != 0) {
			if (event.message.command == cmScrollBarClicked &&
			    (event.message.infoPtr == hScrollBar ||
			     event.message.infoPtr == vScrollBar))
#ifndef __UNPATCHED
				focus();	// BUG FIX                  <<------ Change
#else
				select();
#endif
			else if (event.message.command == cmScrollBarChanged) {
				if (vScrollBar == event.message.infoPtr) {
					focusItemNum(vScrollBar->value);
					drawView();
				} else if (hScrollBar == event.message.infoPtr)
					drawView();
			}
		}
	}
}

void TListViewer::selectItem(short)
{
	message(owner, evBroadcast, cmListItemSelected, this);
}

void TListViewer::setRange(short aRange)
{
	range = aRange;

#ifndef __UNPATCHED
	if (focused >= aRange)	// BUG FIX - EFW - Tue 06/26/95
		focused = (aRange - 1 >= 0) ? aRange - 1 : 0;
#else
	if (focused > aRange)
		focused = 0;
#endif
	if (vScrollBar != nullptr)
		vScrollBar->setParams(focused, 0, aRange - 1,
				      vScrollBar->pgStep, vScrollBar->arStep);
	else
		drawView();
}

void TListViewer::setState(ushort aState, Boolean enable)
{
	TView::setState(aState, enable);
	if ((aState & (sfSelected | sfActive | sfVisible)) != 0) {
		if (hScrollBar != nullptr) {
			if (getState(sfActive) && getState(sfVisible))
				hScrollBar->show();
			else
				hScrollBar->hide();
		}
		if (vScrollBar != nullptr) {
			if (getState(sfActive) && getState(sfVisible))
				vScrollBar->show();
			else
				vScrollBar->hide();
		}
		drawView();
	}
}

void TListViewer::shutDown()
{
	hScrollBar = nullptr;
	vScrollBar = nullptr;
	TView::shutDown();
}

#if !defined(NO_STREAMABLE)

void TListViewer::write(opstream & os)
{
	TView::write(os);
	os << hScrollBar << vScrollBar << numCols
	    << topItem << focused << range;
}

void *TListViewer::read(ipstream & is)
{
	TView::read(is);
	is >> hScrollBar >> vScrollBar >> numCols
	    >> topItem >> focused >> range;
	return this;
}

TStreamable *TListViewer::build()
{
	return new TListViewer(streamableInit);
}

TListViewer::TListViewer(StreamableInit):TView(streamableInit)
{
}

#endif
