/*
 * TCluster.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TCluster
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_TPoint
#define Uses_TSItem
#define Uses_TStringCollection
#define Uses_TGroup
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <ctype.h>
#include <string.h>

#ifndef __UNPATCHED
#define cpCluster "\x10\x11\x12\x12\x1f"
#else
#define cpCluster "\x10\x11\x12\x12\1f"
#endif

TCluster::TCluster(const TRect & bounds, TSItem *aStrings):TView(bounds),
value(0), sel(0)
{
	options |= ofSelectable | ofFirstClick | ofPreProcess | ofPostProcess;
	short i = 0;
	TSItem *p;
	for (p = aStrings; p != nullptr; p = p->next)
		i++;

	strings = new TStringCollection(i, 0);

	while (aStrings != nullptr) {
		p = aStrings;
		strings->atInsert(strings->getCount(), newStr(aStrings->value));
		aStrings = aStrings->next;
		delete p;
	}

	setCursor(2, 0);
	showCursor();
	enableMask = 0xFFFFFFFFL;
}

TCluster::~TCluster()
{
	destroy(static_cast < TCollection * >(strings));
}

ushort TCluster::dataSize()
{
	// value is now a long, but for compatibility with earlier TV,
	// return size of short; TMultiCheckBoxes returns sizeof(long).

	return sizeof(short);
}

void TCluster::drawBox(const char *icon, char marker)
{
	char s[3];
	s[0] = ' ';
	s[1] = marker;
	s[2] = 0;
	drawMultiBox(icon, s);
}

void TCluster::drawMultiBox(const char *icon, const char *marker)
{
	TDrawBuffer b;
	ushort color;
	int i, j, cur;

	ushort cNorm = getColor(0x0301);
	ushort cSel = getColor(0x0402);
	ushort cDis = getColor(0x0505);
	for (i = 0; i <= size.y; i++) {
		b.moveChar(0, ' ', static_cast < uchar > (cNorm), size.x);
		for (j = 0; j <= (strings->getCount() - 1) / size.y + 1; j++) {
			cur = j * size.y + i;
			if (cur < strings->getCount()) {
				int col = column(cur);

				if (((col +
				      strlen((const char *)strings->at(cur)) +
				      5) < (sizeof(b) / sizeof(ushort)))
				    && (col < size.x)) {
					if (!buttonState(cur))
						color = cDis;
					else if ((cur == sel)
						 && (state & sfSelected) != 0)
						color = cSel;
					else
						color = cNorm;
					b.moveChar(col, ' ', color,
						   size.x - col);
					b.moveCStr(col, icon, color);

					b.putChar(col + 2,
						  marker[multiMark(cur)]);
					b.moveCStr(col + 5,
						   static_cast <
						   char *>(strings->at(cur)),
						   color);
					if (showMarkers
					    && ((state & sfSelected) != 0)
					    && cur == sel) {
						b.putChar(col, specialChars[0]);
						b.putChar(column(cur + size.y) -
							  1, specialChars[1]);
					}

				}
			}
		}
		writeBuf(0, i, size.x, 1, b);
	}
	setCursor(column(sel) + 2, row(sel));
}

void TCluster::getData(void *rec)
{
	*static_cast < ushort * >(rec) = value;
	drawView();
}

ushort TCluster::getHelpCtx()
{
	if (helpCtx == hcNoContext)
		return hcNoContext;
	else
		return helpCtx + sel;
}

TPalette & TCluster::getPalette()const
{
	static TPalette palette(cpCluster, sizeof(cpCluster) - 1);
	return palette;
}

void TCluster::moveSel(int i, int s)
{
	if (i <= strings->getCount()) {
		sel = s;
		movedTo(sel);
		drawView();
	}
}

void TCluster::handleEvent(TEvent & event)
{
	TView::handleEvent(event);
	if (!(options & ofSelectable))
		return;
	if (event.what == evMouseDown) {
		TPoint mouse = makeLocal(event.mouse.where);
		int i = findSel(mouse);
		if ((i != -1) && buttonState(i))
			sel = i;
		drawView();
		do {
			mouse = makeLocal(event.mouse.where);
			if ((findSel(mouse) == sel) && buttonState(sel))
				showCursor();
			else
				hideCursor();
		} while (mouseEvent(event, evMouseMove));
		showCursor();
		mouse = makeLocal(event.mouse.where);
		if (findSel(mouse) == sel) {
			press(sel);
			drawView();
		}
		clearEvent(event);
	} else if (event.what == evKeyDown) {
		int s = sel;
		switch (ctrlToArrow(event.keyDown.keyCode)) {
		case kbUp:
			if ((state & sfFocused) != 0) {
				int i = 0;
				do {
					i++;
					s--;
					if (s < 0)
						s = strings->getCount() - 1;
				} while (!
					 (buttonState(s)
					  || (i > strings->getCount())));
				moveSel(i, s);
				clearEvent(event);
			}
			break;

		case kbDown:
			if ((state & sfFocused) != 0) {
				int i = 0;
				do {
					i++;
					s++;
					if (s >= strings->getCount())
						s = 0;
				} while (!
					 (buttonState(s)
					  || (i > strings->getCount())));
				moveSel(i, s);
				clearEvent(event);
			}
			break;
		case kbRight:
			if ((state & sfFocused) != 0) {
				int i = 0;
				do {
					i++;
					s += size.y;
#ifndef __UNPATCHED
					if (s >= strings->getCount())	// BUG FIX - EFW - 10/25/94
					{
						s = (s + 1) % size.y;
						if (s >= strings->getCount())
							s = 0;
					}
#else
					if (s >= strings->getCount())
						s = 0;
#endif
				} while (!
					 (buttonState(s)
					  || (i > strings->getCount())));
#ifndef __UNPATCHED
				moveSel(i, s);	// BUG FIX - EFW - 10/25/94
#endif
				clearEvent(event);
			}
			break;
		case kbLeft:
			if ((state & sfFocused) != 0) {
				int i = 0;
				do {
					i++;
					if (s > 0) {
						s -= size.y;
						if (s < 0) {
							s = ((strings->
							      getCount() +
							      size.y -
							      1) / size.y) *
							    size.y + s - 1;
							if (s >=
							    strings->getCount())
								s = strings->
								    getCount() -
								    1;
						}
					} else
						s = strings->getCount() - 1;

				} while (!
					 (buttonState(s)
					  || (i > strings->getCount())));

				clearEvent(event);
			}
			break;
		default:
			for (int i = 0; i < strings->getCount(); i++) {
				char c =
				    hotKey(static_cast <
					   char *>(strings->at(i)));
				if (getAltCode(c) == event.keyDown.keyCode
				    ||
				    ((owner->phase == phPostProcess
				      || (state & sfFocused) != 0) && c != 0
				     && toupper(event.keyDown.charScan.
						charCode) == c)
				    ) {
					if (buttonState(i)) {
						if (focus()) {
							sel = i;
							movedTo(sel);
							press(sel);
							drawView();
						}
						clearEvent(event);
					}
					return;
				}
			}
			if (event.keyDown.charScan.charCode == ' ' &&
			    (state & sfFocused) != 0) {
				press(sel);
				drawView();
				clearEvent(event);
			}
		}
	}
}

void TCluster::setButtonState(unsigned long aMask, Boolean enable)
{
	if (!enable)
		enableMask &= ~aMask;
	else
		enableMask |= aMask;

	int n = strings->getCount();
	if (n < 32) {
		unsigned long testMask = (1 << n) - 1;
		if ((enableMask & testMask) != 0)
			options |= ofSelectable;
		else
			options &= ~ofSelectable;
	}
}

void TCluster::setData(void *rec)
{
	value = *static_cast < ushort * >(rec);
	drawView();
}

void TCluster::setState(ushort aState, Boolean enable)
{
	TView::setState(aState, enable);
	if (aState == sfSelected)
#ifndef __UNPATCHED
	{
		int i = 0, s = sel - 1;
		do {
			i++;
			s++;
			if (s >= strings->getCount())
				s = 0;
		} while (!(buttonState(s) || i > strings->getCount()));

		moveSel(i, s);
	}
#endif
	drawView();
}

Boolean TCluster::mark(int)
{
	return False;
}

uchar TCluster::multiMark(int item)
{
	return static_cast < uchar > (mark(item)) == True;
}

void TCluster::movedTo(int)
{
}

void TCluster::press(int)
{
}

int TCluster::column(int item)
{
	if (item < size.y)
		return 0;
	else {
		int width = 0;
		int col = -6;
		int l = 0;
		for (int i = 0; i <= item; i++) {
			if (i % size.y == 0) {
				col += width + 6;
				width = 0;
			}

			if (i < strings->getCount())
				l = cstrlen(static_cast <
					    char *>(strings->at(i)));
			if (l > width)
				width = l;
		}
		return col;
	}
}

int TCluster::findSel(TPoint p)
{
	TRect r = getExtent();
	if (!r.contains(p))
		return -1;
	else {
		int i = 0;
		while (p.x >= column(i + size.y))
			i += size.y;
		int s = i + p.y;
		if (s >= strings->getCount())
			return -1;
		else
			return s;
	}
}

int TCluster::row(int item)
{
	return item % size.y;
}

Boolean TCluster::buttonState(int item)
{
	if (item < 32) {
		unsigned long mask = 1;

		while (item--)
			mask <<= 1;

		if (enableMask & mask)
			return True;
		else
			return False;
	} else
		return False;
}

#if !defined(NO_STREAMABLE)

void TCluster::write(opstream & os)
{
	TView::write(os);
	os << value << sel << enableMask << strings;
}

void *TCluster::read(ipstream & is)
{
	TView::read(is);
	is >> value >> sel >> enableMask >> strings;

	setCursor(2, 0);
	showCursor();
	setButtonState(0, True);
	return this;
}

TStreamable *TCluster::build()
{
	return new TCluster(streamableInit);
}

TCluster::TCluster(StreamableInit):TView(streamableInit)
{
}

#endif
