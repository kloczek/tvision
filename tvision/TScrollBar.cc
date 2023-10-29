/*
 * TScrollBar.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TScrollBar
#define Uses_TRect
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_TGroup
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <ctype.h>
#include <string.h>

#define cpScrollBar  "\x04\x05\x05"

TScrollBar::TScrollBar(const TRect & bounds):TView(bounds),
value(0), minVal(0), maxVal(0), pgStep(1), arStep(1)
{
	if (size.x == 1) {
		growMode = gfGrowLoX | gfGrowHiX | gfGrowHiY;
		memcpy(chars, vChars, sizeof(vChars));
	} else {
		growMode = gfGrowLoY | gfGrowHiX | gfGrowHiY;
		memcpy(chars, hChars, sizeof(hChars));
	}
}

void TScrollBar::draw()
{
	drawPos(getPos());
}

void TScrollBar::drawPos(int pos)
{
	TDrawBuffer b;

	int s = getSize() - 1;
	b.moveChar(0, chars[0], getColor(2), 1);
	if (maxVal == minVal)
		b.moveChar(1, chars[4], getColor(1), s - 1);
	else {
		b.moveChar(1, chars[2], getColor(1), s - 1);
		b.moveChar(pos, chars[3], getColor(3), 1);
	}

	b.moveChar(s, chars[1], getColor(2), 1);
	writeBuf(0, 0, size.x, size.y, b);
}

TPalette & TScrollBar::getPalette()const
{
	static TPalette palette(cpScrollBar, sizeof(cpScrollBar) - 1);
	return palette;
}

int TScrollBar::getPos()
{
	int r = maxVal - minVal;
	if (r == 0)
		return 1;
	else
		return
		    int ((((long (value - minVal) * (getSize() - 3))+(r >> 1)) /
			  r) + 1);
}

int TScrollBar::getSize()
{
	int s;

	if (size.x == 1)
		s = size.y;
	else
		s = size.x;

#ifndef __UNPATCHED
	return max(2, s);
#else
	return max(3, s);
#endif
}

static TPoint mouse;
static int p, s;
static TRect extent;

int TScrollBar::getPartCode()
{
	int part = -1;
#ifndef __UNPATCHED
	if (extent.contains(mouse)) {
		int mark = (size.x == 1) ? mouse.y : mouse.x;

		// Check for vertical or horizontal size of 2
		if ((size.x == 1 && size.y == 2)
		    || (size.x == 2 && size.y == 1)) {
			// Set 'part' to left or right arrow only
			if (mark < 1)
				part = sbLeftArrow;
			else if (mark == p)
				part = sbRightArrow;
		} else {
			if (mark == p)
				part = sbIndicator;
			else {
				if (mark < 1)
					part = sbLeftArrow;
				else if (mark < p)
					part = sbPageLeft;
				else if (mark < s)
					part = sbPageRight;
				else
					part = sbRightArrow;
				if (size.x == 1)
					part += 4;
			}
		}
	}
#else
	if (extent.contains(mouse)) {
		int mark = (size.x == 1) ? mouse.y : mouse.x;

		if (mark == p)
			part = sbIndicator;
		else {
			if (mark < 1)
				part = sbLeftArrow;
			else if (mark < p)
				part = sbPageLeft;
			else if (mark < s)
				part = sbPageRight;
			else
				part = sbRightArrow;

			if (size.x == 1)
				part += 4;
		}
	}
#endif
	return part;
}

void TScrollBar::handleEvent(TEvent & event)
{
	Boolean Tracking;
	int i = 0, clickPart;

	TView::handleEvent(event);
	switch (event.what) {
	case evMouseDown:
		message(owner, evBroadcast, cmScrollBarClicked, this);	// Clicked()
		mouse = makeLocal(event.mouse.where);
		extent = getExtent();
		extent.grow(1, 1);
		p = getPos();
		s = getSize() - 1;
		clickPart = getPartCode();
		if (clickPart != sbIndicator) {
			do {
				mouse = makeLocal(event.mouse.where);
				if (getPartCode() == clickPart)
					setValue(value + scrollStep(clickPart));
			} while (mouseEvent(event, evMouseAuto));
		} else {
			do {
				mouse = makeLocal(event.mouse.where);
				Tracking = extent.contains(mouse);
				if (Tracking) {
					if (size.x == 1)
						i = mouse.y;
					else
						i = mouse.x;
					i = max(i, 1);
					i = min(i, s - 1);
				} else
					i = getPos();
				if (i != p) {
					drawPos(i);
#ifndef __UNPATCHED
					// nothing
#else
				p = i;
#endif
				}
			} while (mouseEvent(event, evMouseMove));
			if (Tracking && s > 2) {
				s -= 2;
				setValue(int
					 (((long (p - 1) * (maxVal - minVal) +
					    (s >> 1))/s) + minVal));
			}
		}
		clearEvent(event);
		break;
	case evKeyDown:
		if ((state & sfVisible) != 0) {
			clickPart = sbIndicator;
			if (size.y == 1)
				switch (ctrlToArrow(event.keyDown.keyCode)) {
				case kbLeft:
					clickPart = sbLeftArrow;
					break;
				case kbRight:
					clickPart = sbRightArrow;
					break;
				case kbCtrlLeft:
					clickPart = sbPageLeft;
					break;
				case kbCtrlRight:
					clickPart = sbPageRight;
					break;
				case kbHome:
					i = minVal;
					break;
				case kbEnd:
					i = maxVal;
					break;
				default:
					return;
			} else
				switch (ctrlToArrow(event.keyDown.keyCode)) {
				case kbUp:
					clickPart = sbUpArrow;
					break;
				case kbDown:
					clickPart = sbDownArrow;
					break;
				case kbPgUp:
					clickPart = sbPageUp;
					break;
				case kbPgDn:
					clickPart = sbPageDown;
					break;
				case kbCtrlPgUp:
					i = minVal;
					break;
				case kbCtrlPgDn:
					i = maxVal;
					break;
				default:
					return;
				}
			message(owner, evBroadcast, cmScrollBarClicked, this);	// Clicked
			if (clickPart != sbIndicator)
				i = value + scrollStep(clickPart);
			setValue(i);
			clearEvent(event);
		}
		break;
	default:
		;
	}
}

void TScrollBar::scrollDraw()
{
	message(owner, evBroadcast, cmScrollBarChanged, this);
}

int TScrollBar::scrollStep(int part)
{
	int step;

	if (!(part & 2))
		step = arStep;
	else
		step = pgStep;
	if (!(part & 1))
		return -step;
	else
		return step;
}

void TScrollBar::setParams(int aValue,
			   int aMin, int aMax, int aPgStep, int aArStep)
{
	int sValue;

	aMax = max(aMax, aMin);
	aValue = max(aMin, aValue);
	aValue = min(aMax, aValue);
	sValue = value;
	if (sValue != aValue || minVal != aMin || maxVal != aMax) {
		value = aValue;
		minVal = aMin;
		maxVal = aMax;
		drawView();
		if (sValue != aValue)
			scrollDraw();
	}
	pgStep = aPgStep;
	arStep = aArStep;
}

void TScrollBar::setRange(int aMin, int aMax)
{
	setParams(value, aMin, aMax, pgStep, arStep);
}

void TScrollBar::setStep(int aPgStep, int aArStep)
{
	setParams(value, minVal, maxVal, aPgStep, aArStep);
}

void TScrollBar::setValue(int aValue)
{
	setParams(aValue, minVal, maxVal, pgStep, arStep);
}

#if !defined(NO_STREAMABLE)

void TScrollBar::write(opstream & os)
{
	TView::write(os);
	os << value << minVal << maxVal << pgStep << arStep;
	os.writeBytes(chars, sizeof(chars));
}

void *TScrollBar::read(ipstream & is)
{
	TView::read(is);
	is >> value >> minVal >> maxVal >> pgStep >> arStep;
	is.readBytes(chars, sizeof(TScrollChars));
	return this;
}

TStreamable *TScrollBar::build()
{
	return new TScrollBar(streamableInit);
}

TScrollBar::TScrollBar(StreamableInit):TView(streamableInit)
{
}

#endif
