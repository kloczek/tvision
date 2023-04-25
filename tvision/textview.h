/*
 * textview.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TTextDevice ) && !defined( TV_TTextDevice )
#define TV_TTextDevice

/** \file textview.h
 * textview.h
 */

#include <iostream>

class TRect;
class TScrollBar;

/**
 * TTextDevice is a scrollable TTY-type text viewer/device driver.
 *
 * Apart from the data members and member functions inherited from
 * @ref TScroller, TTextDevice defines virtual member functions for reading
 * and writing strings from and to the device.
 *
 * TTextDevice exists solely as a base type for deriving real terminal
 * drivers. TTextDevice uses TScroller's destructor.
 * @short Scrollable TTY-type text viewer/device driver
 */
class TTextDevice:public TScroller {
      public:
    /**
     * Creates a TTextDevice object with the given bounds, horizontal and
     * vertical scroll bars by calling @ref TScroller constructor with the
     * `bounds' and scroller arguments.
     * @see TScroller::TScroller
     */
	TTextDevice(const TRect & bounds,
		    TScrollBar * aHScrollBar, TScrollBar * aVScrollBar);
    /**
     * Overrides the corresponding function in class streambuf.
     *
     * This is an internal function that is called whenever a character string
     * is to be inserted into the internal buffer.
     */
	virtual int do_sputn(const char *s, int count) = 0;
};

#endif // Uses_TTextDevice

#if defined( Uses_TTerminal ) && !defined( TV_TTerminal )
#define TV_TTerminal

class TRect;
class TScrollBar;

/**
 * TTerminal implements a "dumb" terminal with buffered string reads and
 * writes. The default is a cyclic buffer of 64K bytes.
 * @short Implements a "dumb" terminal with buffered string reads and writes
 */
class TTerminal:public TTextDevice {
      public:
    /**
     * Creates a TTerminal object with the given bounds, horizontal and
     * vertical scroll bars, and buffer by calling @ref TTextDevice constructor
     * with the bounds and scroller arguments, then creating a buffer
     * (pointed to by buffer) with @ref bufSize equal to `aBufSize'.
     * @see TTextDevice::TTextDevice
     *
     * @ref growMode is set to @ref gfGrowHiX | @ref gfGrowHiY. @ref queFront
     * and @ref queBack are both initialized to 0, indicating an empty buffer.
     * The cursor is shown at the view's origin, (0,0).
     */
	TTerminal(const TRect & bounds,
		  TScrollBar * aHScrollBar,
		  TScrollBar * aVScrollBar, ushort aBufSize);
    /**
     * Deallocates the buffer and calls ~TTextDevice().
     */
	~TTerminal();
    /**
     * Overrides the corresponding function in class streambuf.
     *
     * This is an internal function that is called whenever a character string
     * is to be inserted into the internal buffer.
     */
	virtual int do_sputn(const char *s, int count);
    /**
     * Used to manipulate a queue offsets with wrap around: increments `val'
     * by 1, then if `val' >= @ref bufSize, `val' is set to zero.
     */
	void bufInc(ushort & val);
    /**
     * Returns True if the number of bytes given in amount can be inserted
     * into the terminal buffer without having to discard the top line.
     * Otherwise, returns False.
     */
	Boolean canInsert(ushort amount);
    /**
     * Undocumented.
     */
	short calcWidth();
    /**
     * Called whenever the TTerminal scroller needs to be redrawn; for
     * example, when the scroll bars are clicked on, the view is unhidden or
     * resized, the delta values are changed, or when added text forces a
     * scroll.
     */
	virtual void draw();
    /**
     * Returns the buffer offset of the start of the line that follows the
     * position given by `pos'.
     */
	ushort nextLine(ushort pos);
    /**
     * Returns the offset of the start of the line that is `lines' lines
     * previous to the position given by `pos'.
     */
	ushort prevLines(ushort pos, ushort lines);
    /**
     * Returns True if @ref queFront is equal to @ref queBack.
     */
	Boolean queEmpty();
      protected:
    /**
     * The size of the terminal's buffer in bytes.
     */
	 ushort bufSize;
    /**
     * Pointer to the first byte of the terminal's buffer.
     */
	char *buffer;
    /**
     * Offset (in bytes) of the first byte stored in the terminal buffer.
     */
	ushort queFront;
    /**
     * Offset (in bytes) of the last byte stored in the terminal buffer.
     */
	ushort queBack;
    /**
     * Used to manipulate queue offsets with wrap around: if `val' is zero,
     * `val' is set to (bufSize - 1); otherwise, `val' is decremented.
     */
	void bufDec(ushort & val);
#ifndef __UNPATCHED
    /**
     * Undocumented.
     */
	ushort curLineWidth;	// Added horizontal cursor tracking
#endif
};

#endif // Uses_TTerminal

#if defined( Uses_otstream ) && !defined( TV_otstream )
#define TV_otstream

class ostream;

#include <iostream>
#include <cstdio>

/**
 * Undocumented.
 */
class TerminalBuf:public std::streambuf {
      protected:
    /**
     * Undocumented.
     */
	TTerminal * term;
      public:
    /**
     * Undocumented.
     */
	TerminalBuf(TTerminal * tt);
    /**
     * Overrides the corresponding function in class streambuf.
     *
     * When the internal buffer in a streambuf is full and the iostream
     * associated with that streambuf tries to put another character into the
     * buffer, overflow() is called. Its argument `c' is the character that
     * caused the overflow.
     *
     * In TerminalBuf the underlying streambuf has no buffer, so every
     * character results in an overflow() call.
     */
	virtual int overflow(int c = EOF);
    /**
     * Undocumented.
     */
	virtual int sync();
};

/**
 * Undocumented.
 */
class otstream:public std::ostream {
      protected:
    /**
     * Undocumented.
     */
	TerminalBuf buf;
      public:
    /**
     * Undocumented.
     */
	otstream(TTerminal * tt);
};

#endif
