/*
 * textview.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TTextDevice
#define Uses_TTerminal
#define Uses_otstream
#include <tv.h>

#include <string.h>

TTextDevice::TTextDevice(const TRect & bounds,
			 TScrollBar *aHScrollBar,
			 TScrollBar *aVScrollBar):TScroller(bounds, aHScrollBar,
							    aVScrollBar)
{
}

TTerminal::TTerminal(const TRect & bounds,
		     TScrollBar *aHScrollBar,
		     TScrollBar *aVScrollBar,
		     ushort aBufSize):TTextDevice(bounds, aHScrollBar,
						  aVScrollBar), queFront(0),
#ifndef __UNPATCHED
queBack(0), curLineWidth(0)	// Added for horizontal cursor tracking.
#else
queBack(0)
#endif
{
	growMode = gfGrowHiX + gfGrowHiY;
	bufSize = min(32000U, aBufSize);
	buffer = new char[bufSize];
	setLimit(0, 1);
	setCursor(0, 0);
	showCursor();
}

TTerminal::~TTerminal()
{
	delete buffer;
}

void TTerminal::bufDec(ushort & val)
{
	if (val == 0)
		val = bufSize - 1;
	else
		val--;
}

void TTerminal::bufInc(ushort & val)
{
	if (++val >= bufSize)
		val = 0;
}

Boolean TTerminal::canInsert(ushort amount)
{
	long T = (queFront < queBack) ? (queFront + amount) : (long (queFront) - bufSize + amount);	// cast needed so we get
	// signed comparison
	return Boolean(queBack > T);
}

void TTerminal::draw()
{
	short i;
	ushort begLine, endLine;
#ifndef __UNPATCHED
	char s[maxViewWidth + 1];	// KRW: Bug Fix/Enhancement: 95/01/05
	// Do *NOT* attempt to display more
	// than maxViewWidth chars
	// ** assume size.x <= maxViewWidth **
#else
	char s[256];
#endif
	ushort bottomLine;

	bottomLine = size.y + delta.y;
	if (limit.y > bottomLine) {
		endLine = prevLines(queFront, limit.y - bottomLine);
		bufDec(endLine);
	} else
		endLine = queFront;

	if (limit.y > size.y)
		i = size.y - 1;
	else {
		for (i = limit.y; i <= size.y - 1; i++)
			writeChar(0, i, ' ', 1, size.x);
		i = limit.y - 1;
	}

	for (; i >= 0; i--) {
#ifndef __UNPATCHED
		// KRW: Bug fix/enhancement - handle any length line by only copying
		//   to s those characters to be displayed, which we assume will
		//   be < maxViewWidth
		//  This whole block rewritten
		memset(s, ' ', size.x);	// must display blanks if no characters!
		begLine = prevLines(endLine, 1);
		if (endLine >= begLine) {
			// Entire string to be displayed is in one fragment, not
			// wrapped around the end of the buffer
			int fragmentLength = int (endLine - begLine);
			if (fragmentLength > delta.x) {
				fragmentLength -= delta.x;
				fragmentLength = min(size.x, fragmentLength);
				memcpy(s, &buffer[begLine + delta.x],
				       fragmentLength);
			}
		} else {
			// Extract first fragment into s
			int deltaLeft = delta.x;
			int firstFragmentLength = int (bufSize - begLine);
			if (firstFragmentLength > delta.x) {
				deltaLeft = 0;
				firstFragmentLength -= delta.x;
				firstFragmentLength =
				    min(size.x, firstFragmentLength);
				memcpy(s, &buffer[begLine + delta.x],
				       firstFragmentLength);
			} else {
				deltaLeft -= firstFragmentLength;
				firstFragmentLength = 0;
			}
			// Extract second fragment into s
			if ((endLine > deltaLeft)
			    && (firstFragmentLength < size.x)) {
				int secondFragmentLength = endLine - deltaLeft;
				secondFragmentLength =
				    min(size.x - firstFragmentLength,
					secondFragmentLength);
				memcpy(&s[firstFragmentLength],
				       &buffer[deltaLeft],
				       secondFragmentLength);
			}
		}

		s[size.x] = EOS;
		writeStr(0, i, s, 1);

		endLine = begLine;
		bufDec(endLine);
#else
		begLine = prevLines(endLine, 1);
		if (endLine >= begLine) {
			int T = int (endLine - begLine);
			memcpy(s, &buffer[begLine], T);
			s[T] = EOS;
		} else {
			int T = int (bufSize - begLine);
			memcpy(s, &buffer[begLine], T);
			memcpy(s + T, buffer, endLine);
			s[T + endLine] = EOS;
		}
		if (delta.x >= strlen(s))
			*s = EOS;
		else
			strcpy(s, &s[delta.x]);

		writeStr(0, i, s, 1);
		writeChar(strlen(s), i, ' ', 1, size.x);
		endLine = begLine;
		bufDec(endLine);
#endif
	}
}

ushort TTerminal::nextLine(ushort pos)
{
	if (pos != queFront) {
		while (buffer[pos] != '\n' && pos != queFront)
			bufInc(pos);
		if (pos != queFront)
			bufInc(pos);
	}
	return pos;
}

int TTerminal::do_sputn(const char *s, int count)
{
#ifndef __UNPATCHED
	ushort screenWidth = limit.x;
	ushort screenLines = limit.y;

	// BUG FIX - Mon 07/25/94 - EFW: Made TTerminal adjust horizontal
	// scrollbar limit too.

	/* SS: stupid thing */

	ushort i;
	for (i = 0; i < count; i++, curLineWidth++)
		if (s[i] == '\n') {
			screenLines++;

			// Check width when an LF is seen.
			if (curLineWidth > screenWidth)
				screenWidth = curLineWidth;

			// Reset width for the next line.
			curLineWidth = 0;
		}
	// One last check for width for cases where whole lines aren't
	// received, maybe just a character or two.
	if (curLineWidth > screenWidth)
		screenWidth = curLineWidth;
#else
	ushort screenLines = limit.y;
	for (ushort i = 0; i < count; i++)
		if (s[i] == '\n')
			screenLines++;
#endif

	while (!canInsert(count)) {
		queBack = nextLine(queBack);
		screenLines--;
	}

	if (queFront + count >= bufSize) {
		i = bufSize - queFront;
		memcpy(&buffer[queFront], s, i);
		memcpy(buffer, &s[i], count - i);
		queFront = count - i;
	} else {
		memcpy(&buffer[queFront], s, count);
		queFront += count;
	}

#ifndef __UNPATCHED
	setLimit(screenWidth, screenLines);
#else
	setLimit(limit.x, screenLines);
#endif
	scrollTo(0, screenLines + 1);
	i = prevLines(queFront, 1);
	if (i <= queFront)
		i = queFront - i;
	else
		i = bufSize - (i - queFront);
	setCursor(i, screenLines - delta.y - 1);
	drawView();
	return count;
}

Boolean TTerminal::queEmpty()
{
	return Boolean(queBack == queFront);
}

TerminalBuf::TerminalBuf(TTerminal *tt):term(tt)
{
}

int TerminalBuf::overflow(int c)
{
	std::streamsize n = pptr() - pbase();

	if (n > 0 && sync() == EOF)
		return EOF;
	if (c != EOF) {
		char b = c;

		term->do_sputn(&b, sizeof(b));
	}
	pbump(-n);
	return 0;
}

int TerminalBuf::sync()
{
	std::streamsize n = pptr() - pbase();

	if (n > 0)
		term->do_sputn(pbase(), n);
	return 0;
}

otstream::otstream(TTerminal *tt):
std::ostream(&buf), buf(tt)
{
}
