/*
 * TStatusLine.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <string.h>

#define cpStatusLine "\x02\x03\x04\x05\x06\x07"

TStatusLine::TStatusLine(const TRect & bounds,
			 TStatusDef & aDefs):TView(bounds), defs(&aDefs)
{
	options |= ofPreProcess;
	eventMask |= evBroadcast;
	growMode = gfGrowLoY | gfGrowHiX | gfGrowHiY;
	findItems();
}

void TStatusLine::disposeItems(TStatusItem *item)
{
	while (item != nullptr) {
		TStatusItem *T = item;
		item = item->next;
		delete T;
	}
}

TStatusLine::~TStatusLine(void)
{
	while (defs != nullptr) {
		TStatusDef *T = defs;
		defs = defs->next;
		disposeItems(T->items);
		delete T;
	}
}

void TStatusLine::draw()
{
	drawSelect(nullptr);
}

void TStatusLine::drawSelect(TStatusItem *selected)
{
	TDrawBuffer b;
	ushort color;
	char hintBuf[256];

	ushort cNormal = getColor(0x0301);
	ushort cSelect = getColor(0x0604);
	ushort cNormDisabled = getColor(0x0202);
	ushort cSelDisabled = getColor(0x0505);
	b.moveChar(0, ' ', cNormal, size.x);
	TStatusItem *T = items;
	ushort i = 0;

	while (T != nullptr) {
		if (T->text != nullptr) {
			ushort l = cstrlen(T->text);
			if (i + l < size.x) {
				if (commandEnabled(T->command))
					if (T == selected)
						color = cSelect;
					else
						color = cNormal;
				else if (T == selected)
					color = cSelDisabled;
				else
					color = cNormDisabled;

				b.moveChar(i, ' ', color, 1);
				b.moveCStr(i + 1, T->text, color);
				b.moveChar(i + l + 1, ' ', color, 1);
			}
			i += l + 2;
		}
		T = T->next;
	}
	if (i < size.x - 2) {
		strcpy(hintBuf, hint(helpCtx));
		if (*hintBuf != EOS) {
			b.moveStr(i, hintSeparator, cNormal);
			i += 2;
			if (strlen(hintBuf) + i > (uint) size.x)
				hintBuf[size.x - i] = EOS;
			b.moveStr(i, hintBuf, cNormal);
			i += strlen(hintBuf);
		}
	}
	writeLine(0, 0, size.x, 1, b);
}

void TStatusLine::findItems()
{
	TStatusDef *p = defs;
	while (p != nullptr && (helpCtx < p->min || helpCtx > p->max))
		p = p->next;
	items = (p == nullptr) ? nullptr : p->items;
}

TPalette & TStatusLine::getPalette()const
{
	static TPalette palette(cpStatusLine, sizeof(cpStatusLine) - 1);
	return palette;
}

TStatusItem *TStatusLine::itemMouseIsIn(TPoint mouse)
{
	if (mouse.y != 0)
		return nullptr;

	ushort i;
	TStatusItem *T;

	for (i = 0, T = items; T != nullptr; T = T->next) {
		if (T->text != nullptr) {
			ushort k = i + cstrlen(T->text) + 2;
			if (mouse.x >= i && mouse.x < k)
				return T;
			i = k;
		}
	}
	return nullptr;
}

void TStatusLine::handleEvent(TEvent & event)
{
	TView::handleEvent(event);

	switch (event.what) {
	case evMouseDown:
		{
			TStatusItem *T = nullptr;

			do {
				TPoint mouse = makeLocal(event.mouse.where);
				if (T != itemMouseIsIn(mouse))
					drawSelect(T = itemMouseIsIn(mouse));
			} while (mouseEvent(event, evMouseMove));

			if (T != nullptr && commandEnabled(T->command)) {
				event.what = evCommand;
				event.message.command = T->command;
				event.message.infoPtr = nullptr;
				putEvent(event);
			}
			clearEvent(event);
			drawView();
			break;
		}
	case evKeyDown:
		{
			for (TStatusItem * T = items; T != nullptr; T = T->next) {
				if (event.keyDown.keyCode == T->keyCode &&
				    commandEnabled(T->command)) {
					event.what = evCommand;
					event.message.command = T->command;
					event.message.infoPtr = nullptr;
					return;
				}
			}
			break;
		}
	case evBroadcast:
		if (event.message.command == cmCommandSetChanged)
			drawView();
		break;
	default:
		;
	}
}

const char *TStatusLine::hint(ushort)
{
	return "";
}

void TStatusLine::update()
{
	TView *p = TopView();
	ushort h = (p != nullptr) ? p->getHelpCtx() : hcNoContext;
	if (helpCtx != h) {
		helpCtx = h;
		findItems();
		drawView();
	}
}

#if !defined(NO_STREAMABLE)

void TStatusLine::writeItems(opstream & os, TStatusItem *ts)
{
	int count = 0;
	for (TStatusItem * t = ts; t != nullptr; t = t->next)
		count++;
	os << count;
	for (; ts != nullptr; ts = ts->next) {
		os.writeString(ts->text);
		os << ts->keyCode << ts->command;
	}
}

void TStatusLine::writeDefs(opstream & os, TStatusDef *td)
{
	int count = 0;
	for (TStatusDef * t = td; t != nullptr; t = t->next)
		count++;
	os << count;
	for (; td != nullptr; td = td->next) {
		os << td->min << td->max;
		writeItems(os, td->items);
	}
}

void TStatusLine::write(opstream & os)
{
	TView::write(os);
	writeDefs(os, defs);
}

TStatusItem *TStatusLine::readItems(ipstream & is)
{
#ifndef __UNPATCHED
	TStatusItem *cur;
#else
	TStatusItem *cur = 0;
#endif
	TStatusItem *first;
	TStatusItem **last = &first;
	int count;
	is >> count;
	while (count-- > 0) {
		char *t = is.readString();
#ifndef __UNPATCHED
		ushort key, cmd;
#else
		int key, cmd;
#endif
		is >> key >> cmd;
		cur = new TStatusItem(t, key, cmd);
		*last = cur;
		last = &(cur->next);
		delete t;
	}
	*last = nullptr;
	return first;
}

TStatusDef *TStatusLine::readDefs(ipstream & is)
{
#ifndef __UNPATCHED
	TStatusDef *cur;
#else
	TStatusDef *cur = 0;
#endif
	TStatusDef *first;
	TStatusDef **last = &first;
	int count;
	is >> count;
	while (count-- > 0) {
#ifndef __UNPATCHED
		ushort min, max;
#else
		int min, max;
#endif
		is >> min >> max;
		cur = new TStatusDef(min, max, readItems(is));
		*last = cur;
		last = &(cur->next);
	}
	*last = nullptr;
	return first;
}

void *TStatusLine::read(ipstream & is)
{
	TView::read(is);
	defs = readDefs(is);
	findItems();
	return this;
}

TStreamable *TStatusLine::build()
{
	return new TStatusLine(streamableInit);
}

TStatusLine::TStatusLine(StreamableInit):TView(streamableInit)
{
}

#endif
