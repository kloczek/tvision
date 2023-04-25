/*
 * TMemo.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TEditor
#define Uses_TMemo
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <string.h>

#define cpMemo      "\x1A\x1B"

TMemo::TMemo(const TRect & bounds,
	     TScrollBar *aHScrollBar,
	     TScrollBar *aVScrollBar,
	     TIndicator *aIndicator,
	     uint aBufSize):TEditor(bounds, aHScrollBar, aVScrollBar,
				    aIndicator, aBufSize)
{
}

ushort TMemo::dataSize()
{
	return bufSize + sizeof(ushort);
}

void TMemo::getData(void *rec)
{
	TMemoData *data = static_cast < TMemoData * >(rec);

	data->length = bufLen;
	memcpy(data->buffer, buffer, curPtr);
	memcpy(&data->buffer[curPtr], buffer + (curPtr + gapLen),
	       bufLen - curPtr);
	memset(&data->buffer[bufLen], 0, bufSize - bufLen);
}

void TMemo::setData(void *rec)
{
	TMemoData *data = static_cast < TMemoData * >(rec);

	memcpy(&buffer[bufSize - data->length], data->buffer, data->length);
	setBufLen(data->length);
}

TPalette & TMemo::getPalette()const
{
	static TPalette palette(cpMemo, sizeof(cpMemo) - 1);
	return palette;
}

void TMemo::handleEvent(TEvent & event)
{
	if (event.what != evKeyDown || event.keyDown.keyCode != kbTab)
		TEditor::handleEvent(event);
}

#if !defined(NO_STREAMABLE)

void TMemo::write(opstream & os)
{
	TEditor::write(os);
	os << static_cast < uint > (bufLen);
	os.writeBytes(buffer, curPtr);
	os.writeBytes(buffer + gapLen, bufLen - curPtr);
}

void *TMemo::read(ipstream & is)
{
	TEditor::read(is);
	long int length;
	is >> length;
	if (isValid) {
		is.readBytes(buffer + bufSize - length, length);
		setBufLen(length);
	} else
		is.seekg(is.tellg() + length);
	return this;
}

TStreamable *TMemo::build()
{
	return new TMemo(streamableInit);
}

TMemo::TMemo(StreamableInit):TEditor(streamableInit)
{
}

#endif
