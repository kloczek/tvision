/*
 * system.cc
 *
 * Copyright (c) 1998 Sergio Sigala, Brescia, Italy.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Modified by Sergey Clushin <serg@lamport.ru>, <Clushin@deol.ru> */
/* Modified by Dmitrij Korovkin <tkf@glasnet.ru> */

#define Uses_TButton
#define Uses_TColorSelector
#define Uses_TDeskTop
#define Uses_TDirListBox
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_TEventQueue
#define Uses_TFrame
#define Uses_THistory
#define Uses_TIndicator
#define Uses_TKeys
#define Uses_TListViewer
#define Uses_TMenuBox
#define Uses_TOutlineViewer
#define Uses_TScreen
#define Uses_TScrollBar
#define Uses_TStatusLine
#include <tv.h>

#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include <config.h>		/* configuration file */

#ifdef ENABLE_FBSDM
	/* #include <machine/console.h> */
#define CONS_MOUSECTL	_IOWR('c', 10, mouse_info_t)
#define	RIGHT_BUTTON		0x01
#define	MIDDLE_BUTTON		0x02
#define	LEFT_BUTTON		0x04
struct mouse_data {
	int x;
	int y;
	int buttons;
};
struct mouse_mode {
	int mode;
	int signal;
};
#define MOUSE_SHOW		0x01
#define MOUSE_HIDE		0x02
#define MOUSE_MOVEABS		0x03
#define MOUSE_MOVEREL		0x04
#define MOUSE_GETINFO		0x05
#define MOUSE_MODE		0x06
#define MOUSE_ACTION		0x07
struct mouse_info {
	int operation;
	union {
		struct mouse_data data;
		struct mouse_mode mode;
	} u;
};
typedef struct mouse_info mouse_info_t;
#endif

extern "C" {
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#include <curses.h>
#endif

#ifdef HAVE_GPM_H
#include <gpm.h>		/* mouse stuff */
#endif
};

#ifdef __linux__
#include <sys/kd.h>		/* keyboard stuff */
#endif

#include <term.h>
#undef buttons			/* delete this line and see what happens :-) */

/*
 * This is the delay in ms before the first evMouseAuto is generated when the
 * user holds down a mouse button.
 */
#define DELAY_AUTOCLICK_FIRST	400

/*
 * This is the delay in ms between next evMouseAuto events.  Must be greater
 * than DELAY_SIGALRM (see below).
 */
#define DELAY_AUTOCLICK_NEXT	100

/*
 * This is the time limit in ms within button presses are recognized as
 * double-click events.  Used only under FreeBSD because Gpm has its own
 * double-click detecting machanism.
 */
#define DELAY_DOUBLECLICK	300

/*
 * This is the time limit in ms within Esc-key sequences are detected as
 * Alt-letter sequences.  Useful when we can't generate Alt-letter sequences
 * directly.
 */
#define DELAY_ESCAPE		400

/*
 * This is the delay in ms between consecutive SIGALRM signals.  This
 * signal is used to generate evMouseAuto and cmSysWakeup events.
 */
#define DELAY_SIGALRM		100

/*
 * This broadcast event is used to update the StatusLine.
 */
#define DELAY_WAKEUP		200

/*
 * FreeBSD mouse system only: define which signal to use.
 */
#define FBSDM_SIGNAL		SIGUSR1

/* key modifiers */

#define MALT		(kbLeftAlt | kbRightAlt)
#define MCTRL		(kbLeftCtrl | kbRightCtrl)
#define MSHIFT		(kbLeftShift | kbRightShift)

/* key types */

#define TALT		0x01	/* alt-letter key */

typedef struct {
	int in;
	char type;
	char modifiers;
	unsigned short out;
} keym_t;

static keym_t keym[] = {
	/* ascii codes */

	{ 1, 0, 0, kbCtrlA }, { 2, 0, 0, kbCtrlB }, { 3, 0, 0, kbCtrlC },
	{ 4, 0, 0, kbCtrlD }, { 5, 0, 0, kbCtrlE }, { 6, 0, 0, kbCtrlF },
	{ 7, 0, 0, kbCtrlG }, { 8, 0, 0, kbCtrlH }, { 9, 0, 0, kbCtrlI },
	{ 10, 0, 0, kbCtrlJ }, { 11, 0, 0, kbCtrlK }, { 12, 0, 0, kbCtrlL },
	{ 13, 0, 0, kbCtrlM }, { 14, 0, 0, kbCtrlN }, { 15, 0, 0, kbCtrlO },
	{ 16, 0, 0, kbCtrlP }, { 17, 0, 0, kbCtrlQ }, { 18, 0, 0, kbCtrlR },
	{ 19, 0, 0, kbCtrlS }, { 20, 0, 0, kbCtrlT }, { 21, 0, 0, kbCtrlU },
	{ 22, 0, 0, kbCtrlV }, { 23, 0, 0, kbCtrlW }, { 24, 0, 0, kbCtrlX },
	{ 25, 0, 0, kbCtrlY }, { 26, 0, 0, kbCtrlZ }, { 9, 0, 0, kbTab },
	{ 13, 0, 0, kbEnter }, { 27, 0, 0, kbEsc }, { 31, 0, 0, kbCtrlBack },
	{ 127, 0, 0, kbBack },
	{ 10, 0, 0, kbEnter },	/* I need this */

	{ 9, 0, MSHIFT, kbShiftTab },

	/* alt-letter codes */

	{ 8, TALT, 0, kbAltBack }, { ' ', TALT, 0, kbAltSpace },
	{ '0', TALT, 0, kbAlt0 }, { '1', TALT, 0, kbAlt1 }, { '2', TALT, 0,
							     kbAlt2 },
	{ '3', TALT, 0, kbAlt3 }, { '4', TALT, 0, kbAlt4 }, { '5', TALT, 0,
							     kbAlt5 },
	{ '6', TALT, 0, kbAlt6 }, { '7', TALT, 0, kbAlt7 }, { '8', TALT, 0,
							     kbAlt8 },
	{ '9', TALT, 0, kbAlt9 },
	{ 'A', TALT, 0, kbAltA }, { 'B', TALT, 0, kbAltB }, { 'C', TALT, 0,
							     kbAltC },
	{ 'D', TALT, 0, kbAltD }, { 'E', TALT, 0, kbAltE }, { 'F', TALT, 0,
							     kbAltF },
	{ 'G', TALT, 0, kbAltG }, { 'H', TALT, 0, kbAltH }, { 'I', TALT, 0,
							     kbAltI },
	{ 'J', TALT, 0, kbAltJ }, { 'K', TALT, 0, kbAltK }, { 'L', TALT, 0,
							     kbAltL },
	{ 'M', TALT, 0, kbAltM }, { 'N', TALT, 0, kbAltN }, { 'O', TALT, 0,
							     kbAltO },
	{ 'P', TALT, 0, kbAltP }, { 'Q', TALT, 0, kbAltQ }, { 'R', TALT, 0,
							     kbAltR },
	{ 'S', TALT, 0, kbAltS }, { 'T', TALT, 0, kbAltT }, { 'U', TALT, 0,
							     kbAltU },
	{ 'V', TALT, 0, kbAltV }, { 'W', TALT, 0, kbAltW }, { 'X', TALT, 0,
							     kbAltX },
	{ 'Y', TALT, 0, kbAltY }, { 'Z', TALT, 0, kbAltZ }, { 127, TALT, 0,
							     kbAltBack },

	/* escape codes */

	{ KEY_DOWN, 0, 0, kbDown }, { KEY_UP, 0, 0, kbUp }, { KEY_LEFT, 0, 0,
							     kbLeft },
	{ KEY_RIGHT, 0, 0, kbRight }, { KEY_HOME, 0, 0, kbHome },
	{ KEY_BACKSPACE, 0, 0, kbBack }, { KEY_F(1), 0, 0, kbF1 },
	{ KEY_F(2), 0, 0, kbF2 }, { KEY_F(3), 0, 0, kbF3 }, { KEY_F(4), 0, 0,
							     kbF4 },
	{ KEY_F(5), 0, 0, kbF5 }, { KEY_F(6), 0, 0, kbF6 }, { KEY_F(7), 0, 0,
							     kbF7 },
	{ KEY_F(8), 0, 0, kbF8 }, { KEY_F(9), 0, 0, kbF9 }, { KEY_F(10), 0, 0,
							     kbF10 },
	{ KEY_DC, 0, 0, kbDel }, { KEY_IC, 0, 0, kbIns }, { KEY_NPAGE, 0, 0,
							   kbPgDn },
	{ KEY_PPAGE, 0, 0, kbPgUp }, { KEY_END, 0, 0, kbEnd },

	{ KEY_LEFT, 0, MCTRL, kbCtrlLeft }, { KEY_RIGHT, 0, MCTRL,
					     kbCtrlRight },
	{ KEY_HOME, 0, MCTRL, kbCtrlHome }, { KEY_F(1), 0, MCTRL, kbCtrlF1 },
	{ KEY_F(2), 0, MCTRL, kbCtrlF2 }, { KEY_F(3), 0, MCTRL, kbCtrlF3 },
	{ KEY_F(4), 0, MCTRL, kbCtrlF4 }, { KEY_F(5), 0, MCTRL, kbCtrlF5 },
	{ KEY_F(6), 0, MCTRL, kbCtrlF6 }, { KEY_F(7), 0, MCTRL, kbCtrlF7 },
	{ KEY_F(8), 0, MCTRL, kbCtrlF8 }, { KEY_F(9), 0, MCTRL, kbCtrlF9 },
	{ KEY_F(10), 0, MCTRL, kbCtrlF10 }, { KEY_DC, 0, MCTRL, kbCtrlDel },
	{ KEY_IC, 0, MCTRL, kbCtrlIns }, { KEY_NPAGE, 0, MCTRL, kbCtrlPgDn },
	{ KEY_PPAGE, 0, MCTRL, kbCtrlPgUp }, { KEY_END, 0, MCTRL, kbCtrlEnd },

	{ KEY_F(1), 0, MALT, kbAltF1 }, { KEY_F(2), 0, MALT, kbAltF2 },
	{ KEY_F(3), 0, MALT, kbAltF3 }, { KEY_F(4), 0, MALT, kbAltF4 },
	{ KEY_F(5), 0, MALT, kbAltF5 }, { KEY_F(6), 0, MALT, kbAltF6 },
	{ KEY_F(7), 0, MALT, kbAltF7 }, { KEY_F(8), 0, MALT, kbAltF8 },
	{ KEY_F(9), 0, MALT, kbAltF9 }, { KEY_F(10), 0, MALT, kbAltF10 },

	{ KEY_F(11), 0, MSHIFT, kbShiftF1 }, { KEY_F(12), 0, MSHIFT,
					      kbShiftF2 },
	{ KEY_F(13), 0, MSHIFT, kbShiftF3 }, { KEY_F(14), 0, MSHIFT,
					      kbShiftF4 },
	{ KEY_F(15), 0, MSHIFT, kbShiftF5 }, { KEY_F(16), 0, MSHIFT,
					      kbShiftF6 },
	{ KEY_F(17), 0, MSHIFT, kbShiftF7 }, { KEY_F(18), 0, MSHIFT,
					      kbShiftF8 },
	{ KEY_F(19), 0, MSHIFT, kbShiftF9 }, { KEY_F(20), 0, MSHIFT,
					      kbShiftF10 },
	{ KEY_DC, 0, MSHIFT, kbShiftDel }, { KEY_IC, 0, MSHIFT, kbShiftIns },

	/* Shift'ed codes in xterm */

	{ KEY_F(13), 0, 0, kbShiftF1 }, { KEY_F(14), 0, 0, kbShiftF2 },
	{ KEY_F(15), 0, 0, kbShiftF3 }, { KEY_F(16), 0, 0, kbShiftF4 },
	{ KEY_F(17), 0, 0, kbShiftF5 }, { KEY_F(18), 0, 0, kbShiftF6 },
	{ KEY_F(19), 0, 0, kbShiftF7 }, { KEY_F(20), 0, 0, kbShiftF8 },
	{ KEY_F(21), 0, 0, kbShiftF9 }, { KEY_F(22), 0, 0, kbShiftF10 },

	/* Ctrl'ed codes in xterm */

	{ KEY_F(25), 0, 0, kbCtrlF1 }, { KEY_F(26), 0, 0, kbCtrlF2 },
	{ KEY_F(27), 0, 0, kbCtrlF3 }, { KEY_F(28), 0, 0, kbCtrlF4 },
	{ KEY_F(29), 0, 0, kbCtrlF5 }, { KEY_F(30), 0, 0, kbCtrlF6 },
	{ KEY_F(31), 0, 0, kbCtrlF7 }, { KEY_F(32), 0, 0, kbCtrlF8 },
	{ KEY_F(33), 0, 0, kbCtrlF9 }, { KEY_F(34), 0, 0, kbCtrlF10 },
	{ KEY_LL, 0, 0, kbCtrlPgDn }, { KEY_BEG, 0, 0, kbCtrlPgUp },
	{ KEY_COPY, 0, 0, kbCtrlIns }, { KEY_SBEG, 0, 0, kbShiftIns },

	/* Alt'ed (Meta'ed) codes in xterm */

	{ KEY_F(37), 0, 0, kbAltF1 }, { KEY_F(38), 0, 0, kbAltF2 },
	{ KEY_F(39), 0, 0, kbAltF3 }, { KEY_F(40), 0, 0, kbAltF4 },
	{ KEY_F(41), 0, 0, kbAltF5 }, { KEY_F(42), 0, 0, kbAltF6 },
	{ KEY_F(43), 0, 0, kbAltF7 }, { KEY_F(44), 0, 0, kbAltF8 },
	{ KEY_F(45), 0, 0, kbAltF9 }, { KEY_F(46), 0, 0, kbAltF10 }
};

/* lookup table to translate characters from pc set to standard ascii */

static unsigned pcToAscii[256] = {
	32, 79, 79, 111, 111, 111, 111, 111,	/* 000-007 */
	111, 111, 111, 111, 33, 33, 33, 42,	/* 008-00F */
	62, 60, 124, 33, 33, 79, 95, 124,	/* 010-017 */
	94, 86, 62, 60, 45, 45, 94, 86,	/* 018-01F */
	32, 33, 34, 35, 36, 37, 38, 39,	/* 020-027 */
	40, 41, 42, 43, 44, 45, 46, 47,	/* 028-02F */
	48, 49, 50, 51, 52, 53, 54, 55,	/* 030-037 */
	56, 57, 58, 59, 60, 61, 62, 63,	/* 038-03F */
	64, 65, 66, 67, 68, 69, 70, 71,	/* 040-047 */
	72, 73, 74, 75, 76, 77, 78, 79,	/* 048-04F */
	80, 81, 82, 83, 84, 85, 86, 87,	/* 050-057 */
	88, 89, 90, 91, 92, 93, 94, 95,	/* 058-05F */
	96, 97, 98, 99, 100, 101, 102, 103,	/* 060-067 */
	104, 105, 106, 107, 108, 109, 110, 111,	/* 068-06F */
	112, 113, 114, 115, 116, 117, 118, 119,	/* 070-077 */
	120, 121, 122, 123, 124, 125, 126, 32,	/* 078-07F */
	111, 111, 111, 111, 111, 111, 111, 111,	/* 080-087 */
	111, 111, 111, 111, 111, 111, 111, 111,	/* 088-08F */
	111, 111, 111, 111, 111, 111, 111, 111,	/* 090-097 */
	111, 111, 111, 111, 111, 111, 111, 111,	/* 098-09F */
	160, 161, 162, 163, 164, 165, 166, 167,	/* 0A0-0A7 */
	168, 169, 170, 171, 172, 173, 174, 175,	/* 0A8-0AF */
	176, 177, 178, 179, 180, 181, 182, 183,	/* 0B0-0B7 */
	184, 185, 186, 187, 188, 189, 190, 191,	/* 0B8-0BF */
	192, 193, 194, 195, 196, 197, 198, 199,	/* 0C0-0C7 */
	200, 201, 202, 203, 204, 205, 206, 207,	/* 0C8-0CF */
	208, 209, 210, 211, 212, 213, 214, 215,	/* 0D0-0D7 */
	216, 217, 218, 219, 220, 221, 222, 223,	/* 0D8-0DF */
	224, 225, 226, 227, 228, 229, 230, 231,	/* 0E0-0E7 */
	232, 233, 234, 235, 236, 237, 238, 239,	/* 0E8-0EF */
	240, 241, 242, 243, 244, 245, 246, 247,	/* 0F0-0F7 */
	248, 249, 250, 251, 252, 253, 254, 255	/* 0C8-0FF */
};

/* this array stores the corresponding ncurses attribute for each TV color */

static unsigned attributeMap[256];

ushort TEventQueue::doubleDelay = 8;
Boolean TEventQueue::mouseReverse = False;

ushort TScreen::screenMode;
uchar TScreen::screenWidth;
uchar TScreen::screenHeight;
ushort *TScreen::screenBuffer;
fd_set TScreen::fdSetRead;
fd_set TScreen::fdSetWrite;
fd_set TScreen::fdSetExcept;
fd_set TScreen::fdActualRead;
fd_set TScreen::fdActualWrite;
fd_set TScreen::fdActualExcept;

static TEvent *evIn;		/* message queue system */
static TEvent *evOut;
static TEvent evQueue[eventQSize];
static TPoint msWhere;		/* mouse coordinates */
static char env[PATH_MAX];	/* value of the TVOPT environment variable */
static int curX, curY;		/* current cursor coordinates */
static int currentTime;		/* current timer value */
static int doRepaint;		/* should redraw the screen ? */
static int doResize;		/* resize screen ? */
static int evLength;		/* number of events in the queue */
static int msOldButtons;	/* mouse button status */
static std::ofstream xlog;	/* a logging file */

/*
 * A simple class which implements a timer.
 */

class Timer {
	int limit;
      public:
	Timer() {
		limit = -1;
	} int isExpired() {
		return limit != -1 && currentTime >= limit;
	}
	int isRunning() {
		return limit != -1;
	}
	void start(int timeout) {
		limit = currentTime + timeout;
	}
	void stop() {
		limit = -1;
	}
};

static Timer kbEscTimer;	/* time limit to detect Esc-key sequences */
static Timer msAutoTimer;	/* time when generate next cmMouseAuto */
static Timer wakeupTimer;	/* time when generate next cmWakeup */

#ifdef ENABLE_FBSDM
static Timer msDoubleTimer;	/* time limit to detect double-click events */
static int msFlag;		/* set if there are mouse events */
static int msUseArrow;		/* use arrow pointer */
#endif

#ifdef ENABLE_GPM
static int msFd;		/* mouse file descriptor */
#endif

/*
 * GENERAL FUNCTIONS
 */

#define LOG(s) xlog << s << std::endl

inline int range(int test, int min, int max)
{
	return test < min ? min : test > max ? max : test;
}

/*
 * KEYBOARD FUNCTIONS
 */

/*
 * Builds a keycode from code and modifiers.
 */

static int kbMapKey(int code, int type, int modifiers)
{
	keym_t *best = nullptr, *p;

	for (p = keym; p < keym + sizeof(keym) / sizeof(keym_t); p++) {
		/* code and type must match */

		if (p->in == code && p->type == type) {
			/*
			 * now get the best keycode we have, modifier keys
			 * may differ
			 */
			if (best == nullptr || p->modifiers == modifiers) {
				best = p;
			}
		}
	}
	if (best != nullptr)
		return best->out;	/* keycode found */
	if (code <= 255)
		return code;	/* it is an ascii character */
	return kbNoKey;		/* unknown code */
}

/*
 * Gets information about modifier keys (Alt, Ctrl and Shift).  This can
 * be done only if the program runs on the system console.
 */

static int kbReadShiftState()
{
#ifdef __linux__
	unsigned char arg = 6;	/* TIOCLINUX function #6 */
	int shift = 0;

	if (ioctl(STDIN_FILENO, TIOCLINUX, &arg) != -1) {
		if (arg & (2 | 8))
			shift |= kbLeftAlt | kbRightAlt;
		if (arg & 4)
			shift |= kbLeftCtrl | kbRightCtrl;
		if (arg & 1)
			shift |= kbLeftShift | kbRightShift;
	}
	return shift;
#else
	return 0;
#endif
}

/*
 * Reads a key from the keyboard.
 */
#ifdef NCURSES_MOUSE_VERSION
static void msHandle();
#endif

static void kbHandle()
{
	int code, type = 0;
	sigset_t alarmBlock, normalMask;

	sigemptyset(&alarmBlock);
	sigaddset(&alarmBlock, SIGALRM);

	/* see if there is data available */

	sigprocmask(SIG_BLOCK, &alarmBlock, &normalMask);
	code = getch();
	sigprocmask(SIG_SETMASK, &normalMask, nullptr);

#ifdef NCURSES_MOUSE_VERSION
	if (code == KEY_MOUSE) {	/* was it a mouse event ? */
		msHandle();
		return;
	} else
#endif
	if (code != ERR) {	/* was it a key press ? */
		/* grab the escape key and start the timer */
		if (code == 27 && !kbEscTimer.isRunning()) {
			kbEscTimer.start(DELAY_ESCAPE);
			return;
		}

		/* key pressed within time limit */

		if (kbEscTimer.isRunning() && !kbEscTimer.isExpired()) {
			kbEscTimer.stop();
			if (code != 27) {	/* simulate Alt-letter code */
				code = toupper(code);
				type = TALT;
			}
		}
	} else if (kbEscTimer.isExpired()) {	/* an Escape key timeout ? */
		/* timeout condition: generate standard Esc code */
		kbEscTimer.stop();
		code = 27;
	} else
		return;		/* nothing to do */

	int modifiers = kbReadShiftState();
	if ((code = kbMapKey(code, type, modifiers)) != kbNoKey) {
		TEvent event;

		event.what = evKeyDown;
		event.keyDown.keyCode = code;
		event.keyDown.controlKeyState = modifiers;
		TScreen::putEvent(event);
	}
}

/*
 * MOUSE FUNCTIONS
 */

/*
 * This function inserts a mouse event in the event queue after adjusting the
 * `event.mouse.buttons' field.
 */

static void msPutEvent(TEvent & event, int buttons, int flags, int what)
{
	event.mouse.buttons = 0;
	event.mouse.eventFlags = flags;
	event.what = what;
	if (TEventQueue::mouseReverse) {	/* swap buttons ? */
		if (buttons & mbLeftButton)
			event.mouse.buttons |= mbRightButton;
		if (buttons & mbRightButton)
			event.mouse.buttons |= mbLeftButton;
	} else
		event.mouse.buttons = buttons;	/* no swapping */
	TScreen::putEvent(event);
}

/*
 * Initializes the FreeBSD mouse.  The mouse is handled by the kernel.  We
 * control it with ioctl(...) calls.
 */

#ifdef ENABLE_FBSDM
static void fbsdmInit()
{
	mouse_info_t mi;

	msAutoTimer.stop();
	msDoubleTimer.stop();
	msFlag = msOldButtons = 0;
	msUseArrow = strstr(env, "noarrow") == nullptr;
	if (!msUseArrow)
		LOG("arrow pointer suppressed");
	mi.operation = MOUSE_MODE;
	mi.u.mode.signal = FBSDM_SIGNAL;
	if (ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi) < 0) {
		LOG("unable to use the mouse");
	}
	mi.operation = MOUSE_GETINFO;
	ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi);
	msWhere.x = range(mi.u.data.x / 8, 0, TScreen::screenWidth - 1);
	msWhere.y = range(mi.u.data.y / 16, 0, TScreen::screenHeight - 1);
}

/*
 * Closes the FreeBSD mouse.
 */

static void fbsdmClose()
{
	mouse_info_t mi;

	mi.operation = MOUSE_MODE;
	mi.u.mode.signal = 0;
	ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi);
}

/*
 * Checks the status of every button and generates the related event.
 */

static void fbsdmProcessButton(TEvent & event, int buttons, int mask)
{
	if (buttons & mask) {	/* is button pressed ? */
		msAutoTimer.start(DELAY_AUTOCLICK_FIRST);
		if (msDoubleTimer.isRunning() && !msDoubleTimer.isExpired()) {
			msDoubleTimer.stop();
			msPutEvent(event, mask, meDoubleClick, evMouseDown);
		} else {
			msDoubleTimer.start(DELAY_DOUBLECLICK);
			msPutEvent(event, mask, 0, evMouseDown);
		}
	} else {		/* button is released */
		msAutoTimer.stop();
		msPutEvent(event, mask, 0, evMouseUp);
	}
}

/*
 * Handles events from the FreeBSD mouse driver.
 */

static void fbsdmHandle()
{
	TEvent event;
	mouse_info_t mi;

	mi.operation = MOUSE_GETINFO;
	ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi);
	event.mouse.controlKeyState = kbReadShiftState();
	event.mouse.where.x = range(mi.u.data.x / 8, 0,
				    TScreen::screenWidth - 1);
	event.mouse.where.y = range(mi.u.data.y / 16, 0,
				    TScreen::screenHeight - 1);

	/* convert button bits to TV standard */

	int buttons = 0;
	if (mi.u.data.buttons & LEFT_BUTTON)
		buttons |= mbLeftButton;
	if (mi.u.data.buttons & RIGHT_BUTTON)
		buttons |= mbRightButton;

	/* is mouse moved ? */

	if (event.mouse.where != msWhere) {
		msAutoTimer.stop();
		msDoubleTimer.stop();
		msPutEvent(event, buttons, meMouseMoved, evMouseMove);
		msOldButtons = buttons;

		/* redraw the mouse in the new place */

		if (msUseArrow)
			msWhere = event.mouse.where;
		else {
			TScreen::drawMouse(0);
			msWhere = event.mouse.where;
			TScreen::drawMouse(1);
		}
	}
	if (buttons != msOldButtons) {	/* is button state changed ? */
		int changed = buttons ^ msOldButtons;
		int i;
		static int mask[] = { mbLeftButton, mbRightButton };

		/* check for pressed or released buttons */

		for (i = 0; i < 2; i++)
			if (changed & mask[i]) {
				fbsdmProcessButton(event, buttons, mask[i]);
			}
		msOldButtons = buttons;
	}
}
#endif

/*
 * Opens the connection.
 */

#ifdef ENABLE_GPM
static void gpmInit()
{
	msAutoTimer.stop();
	msFd = -1;
	msOldButtons = msWhere.x = msWhere.y = 0;
	if (strstr(env, "nogpm") != nullptr)
		LOG("gpm support disabled");
	else {
		Gpm_Connect conn;

		conn.eventMask = ~0;	/* I want all events */
		conn.defaultMask = 0;	/* no default treatment */
		conn.maxMod = ~0;
		conn.minMod = 0;
		gpm_zerobased = 1;	/* coordinates start from zero */

		int testFd = Gpm_Open(&conn, 0);
		/*
		 * The return value is either -1 or the file descriptor used
		 * to communicate with the server.  When run under xterm, a
		 * gpm client gets event through stdin, and the return value
		 * for Gpm_Open() will be -2.  This value is always available
		 * within gpm_fd.
		 */
		if (testFd == -2 && mousemask(ALL_MOUSE_EVENTS |
					      REPORT_MOUSE_POSITION,
					      nullptr) != 0) {
			msFd = testFd;
			LOG("gpm server " << Gpm_GetServerVersion(nullptr));
			LOG("gpm will send messages through ncurses");
		} else if (testFd >= 0) {
			msFd = testFd;
			FD_SET(msFd, &TScreen::fdSetRead);
			LOG("gpm server " << Gpm_GetServerVersion(nullptr));
			LOG("gpm will send messages directly");
		} else
			LOG("no working gpm, running without mouse");
	}
}

/*
 * Closes the connection.
 */

static void gpmClose()
{
	if (msFd != -1) {
		Gpm_Close();
		if (msFd == -2)
			mousemask(0, nullptr);
		else if (msFd >= 0)
			FD_CLR(msFd, &TScreen::fdSetRead);
		msFd = -1;
	}
}

/*
 * Suspend connection, see Open and Close in `info gpm' for details.
 */

static void gpmSuspend()
{
	if (msFd == -2)
		mousemask(0, nullptr);
	else if (msFd >= 0) {
		Gpm_Connect conn;

		/*
		 * Multiple opens are allowed, and a stack of Gpm_Connect
		 * structures is managed by the library.  You can, thus,
		 * re-open the connection in order to temporarily change the
		 * range of events you're interested in.  When you invoke an
		 * external program, for example, you should re-open the
		 * connection with eventMask zeroed, and defaultMask, minMod
		 * and maxMod all equal to ~0.
		 */
		conn.eventMask = 0;
		conn.defaultMask = ~0;
		conn.maxMod = ~0;
		conn.minMod = ~0;
		gpm_zerobased = 0;
		Gpm_Open(&conn, 0);
	}
}

/*
 * Resume connection, see Open and Close in `info gpm' for details.
 */

static void gpmResume()
{
	if (msFd == -2)
		mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
	else if (msFd >= 0) {
		/*
		 * Pops the connection stack. It is used to restore the
		 * previous situation after a change in the connection masks.
		 * Closes the actual connection when the stack gets empty.  On
		 * last close it returns 0, -1 otherwise.
		 */
		Gpm_Close();
		gpm_zerobased = 1;
	}
}

/*
 * Handles mouse events.
 */

static void gpmHandle()
{
	Gpm_Event me;
	TEvent event;

	/*
	 * Reads an event form gpm_fd.  It should be called only when the
	 * gpm_fd descriptor is reported as readable by a select() system
	 * call, or it will block until an event arrives (unless you put the
	 * mouse file in non-blocking mode).  It returns 1 on success, -1 on
	 * failure, and 0 after closing the connection.  Failure can happen if
	 * a signal interrupted the read system call.  This function doesn't
	 * work with xterm mouse reporting and is meant for internal use by
	 * the library.
	 */
	Gpm_GetEvent(&me);
	event.mouse.controlKeyState = kbReadShiftState();
	event.mouse.where.x = range(me.x, 0, TScreen::screenWidth - 1);
	event.mouse.where.y = range(me.y, 0, TScreen::screenHeight - 1);

	/* convert button bits to TV standard */

	int buttons = 0;
	if (me.buttons & GPM_B_LEFT)
		buttons |= mbLeftButton;
	if (me.buttons & GPM_B_RIGHT)
		buttons |= mbRightButton;

	/* is any button double-clicked ? */

	if (me.type & GPM_DOUBLE) {
		if (me.type & GPM_DOWN)
			return;
		if (me.type & GPM_UP) {
			msAutoTimer.stop();
			msPutEvent(event, buttons, meDoubleClick, evMouseDown);
			msOldButtons &= ~buttons;
		}
	}

	/* is mouse moved ? */

	if (me.type & (GPM_DRAG | GPM_MOVE) && event.mouse.where != msWhere) {
		/*
		 * Each bit set in buttons means the relative button is
		 * down.
		 */
		msAutoTimer.stop();
		msPutEvent(event, buttons, meMouseMoved, evMouseMove);
		msOldButtons = buttons;

		/* redraw the mouse in the new place */

		TScreen::drawMouse(0);
		msWhere = event.mouse.where;
		TScreen::drawMouse(1);
	}
	if (me.type & GPM_DOWN) {	/* is any button pressed ? */
		/*
		 * Each bit in buttons reports the actual state of the
		 * relative button.  We need to determine which button was
		 * pressed.
		 */
		msAutoTimer.start(DELAY_AUTOCLICK_FIRST);
		msPutEvent(event, buttons & ~msOldButtons, 0, evMouseDown);
		msOldButtons = buttons;
	}
	if (me.type & GPM_UP) {	/* is any button released ? */
		/*
		 * Each bit set in buttons means the relative button was
		 * released.
		 */
		msAutoTimer.stop();
		msPutEvent(event, buttons, 0, evMouseUp);
		msOldButtons &= ~buttons;
	}
}
#endif

/*
 * Handles mouse events.
 *
 * This function was changed to fit the message handling of the ncurses mouse
 * support that differs from the gpm by the following:
 *
 * - sends mouse clicks, double clicks, etc;
 * - sends the message only once: we needn't lock messages;
 * - doesn't send mouse drag and mouse move messages (or may be I couldn't
 *   find the way to get them).
 */

#ifdef NCURSES_MOUSE_VERSION
static void msHandle()
{
	TEvent event;
	MEVENT me;
/*
       BUTTON1_PRESSED          mouse button 1 down
       BUTTON1_RELEASED         mouse button 1 up
       BUTTON1_CLICKED          mouse button 1 clicked
       BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
       BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
       BUTTON2_PRESSED          mouse button 2 down
       BUTTON2_RELEASED         mouse button 2 up
       BUTTON2_CLICKED          mouse button 2 clicked
       BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
       BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
       BUTTON3_PRESSED          mouse button 3 down
       BUTTON3_RELEASED         mouse button 3 up
       BUTTON3_CLICKED          mouse button 3 clicked
       BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
       BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
       BUTTON4_PRESSED          mouse button 4 down
       BUTTON4_RELEASED         mouse button 4 up
       BUTTON4_CLICKED          mouse button 4 clicked
       BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
       BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
       BUTTON_SHIFT             shift was down during button state change
       BUTTON_CTRL              control was down during button state change
       BUTTON_ALT               alt was down during button state change
*/
#define BUTTON_PRESSED (BUTTON1_PRESSED | BUTTON2_PRESSED | BUTTON3_PRESSED)
#define BUTTON_RELEASED (BUTTON1_RELEASED | BUTTON2_RELEASED | BUTTON3_RELEASED)
#define BUTTON_DOUBLE_CLICKED (BUTTON1_DOUBLE_CLICKED | BUTTON2_DOUBLE_CLICKED | BUTTON3_DOUBLE_CLICKED)
#define BUTTON_CLICKED (BUTTON1_CLICKED | BUTTON2_CLICKED | BUTTON3_CLICKED)
#define BUTTON1 (BUTTON1_PRESSED | BUTTON1_RELEASED | BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | BUTTON1_TRIPLE_CLICKED)
#define BUTTON2 (BUTTON2_PRESSED | BUTTON2_RELEASED | BUTTON2_CLICKED | BUTTON2_DOUBLE_CLICKED | BUTTON2_TRIPLE_CLICKED)
#define BUTTON3 (BUTTON3_PRESSED | BUTTON3_RELEASED | BUTTON3_CLICKED | BUTTON3_DOUBLE_CLICKED | BUTTON3_TRIPLE_CLICKED)
#define BUTTON4 (BUTTON4_PRESSED | BUTTON4_RELEASED | BUTTON4_CLICKED | BUTTON4_DOUBLE_CLICKED | BUTTON4_TRIPLE_CLICKED)

	if (getmouse(&me) != OK)
		return;		/* exit if no event */

	event.mouse.controlKeyState = 0;
	if (me.bstate & BUTTON_SHIFT)
		event.mouse.controlKeyState |= kbLeftShift | kbRightShift;
	if (me.bstate & BUTTON_CTRL)
		event.mouse.controlKeyState |= kbLeftCtrl | kbRightCtrl;
	if (me.bstate & BUTTON_ALT)
		event.mouse.controlKeyState |= kbLeftAlt | kbRightAlt;
	me.x = (unsigned char)me.x;
	me.y = (unsigned char)me.y;
	event.mouse.where.x = range(me.x, 0, TScreen::screenWidth - 1);
	event.mouse.where.y = range(me.y, 0, TScreen::screenHeight - 1);

	/* convert button bits to TV standard */

	int buttons = mbLeftButton;
	if (me.bstate & BUTTON1)
		buttons = mbLeftButton;
	else if (me.bstate & BUTTON3)
		buttons = mbRightButton;

	if (me.bstate & BUTTON_CLICKED) {	/* is any button clicked ? */
		/* first generate a mouse down event */
		msPutEvent(event, buttons, 0, evMouseDown);
		msOldButtons = buttons;

		/* then generate a mouse up event */

		msAutoTimer.stop();
		msPutEvent(event, buttons, 0, evMouseUp);
		msOldButtons &= ~buttons;
	}
	if (me.bstate & BUTTON_DOUBLE_CLICKED) {	/* any double-click ? */
		msAutoTimer.stop();
		msPutEvent(event, buttons, meDoubleClick, evMouseDown);
		msOldButtons &= ~buttons;
	}
	if (event.mouse.where != msWhere) {	/* is mouse moved ? */
		if (me.bstate & BUTTON_PRESSED) {
			/* first generate a mouse move event */

			msPutEvent(event, buttons, meMouseMoved, evMouseMove);
			msWhere = event.mouse.where;

			/* then generate a mouse down event */

			msAutoTimer.start(DELAY_AUTOCLICK_FIRST);
			msPutEvent(event, buttons, 0, evMouseDown);
			msOldButtons = buttons;
		}
		if (me.bstate & BUTTON_RELEASED) {
			/* first generate a mouse move event */

			msPutEvent(event, buttons, meMouseMoved, evMouseMove);
			msWhere = event.mouse.where;

			/* then generate a mouse up event */

			msAutoTimer.stop();
			msPutEvent(event, buttons, 0, evMouseUp);
			msOldButtons &= ~buttons;
		}
	} else {		/* mouse is not moved */
	if (me.bstate & BUTTON_PRESSED) {
		/* generate a mouse down event */

		msAutoTimer.start(DELAY_AUTOCLICK_FIRST);
		msPutEvent(event, buttons, 0, evMouseDown);
		msOldButtons = buttons;
	}
	if (me.bstate & BUTTON_RELEASED) {
		/* generate a mouse up event */

		msAutoTimer.stop();
		msPutEvent(event, buttons, 0, evMouseUp);
		msOldButtons &= ~buttons;
	}
	}
}
#endif

/*
 * OTHER LOCAL FUNCTIONS
 */

/*
 * Select the best palette we can use.
 */

static void selectPalette()
{
	if (has_colors())
		TScreen::screenMode = TScreen::smCO80;
	else
		TScreen::screenMode = TScreen::smMono;

	/*
	 * This sets the standard attribute mapping to have 16 foreground
	 * colors and 8 background colors.
	 */
	if (TScreen::screenMode == TScreen::smCO80) {
		int back, fore, bold, i;
		/*
		 * This loop allocates all the available color pair entries in
		 * the following order:
		 *
		 * 7/0 7/1 7/2 ... ... 7/7
		 * 6/0 6/1 6/2         6/7
		 * 5/0 5/1 5/2         5/7
		 * ...                 ...
		 * ...                 ...
		 * 0/0 0/1 0/2 ... ... 0/7
		 *
		 * The first color pair must be 7/0 because it was hardcoded
		 * to WHITE foreground on BLACK background in the ncurses
		 * library.
		 */
		i = 0;
		for (fore = COLORS - 1; fore >= 0; fore--) {
			for (back = 0; back < COLORS; back++) {
				/*
				 * The first entry is hardcoded to WHITE on
				 * BLACK, so it does not allow redefinition.
				 */
				if (i != 0)
					init_pair(i, fore, back);
				i++;
			}
		}
		/*
		 * Now we map each possible physical color in a corresponding
		 * attribute.
		 */
		for (i = 0; i < 256; i++) {
			/*
			 * The following table allows conversion from TV
			 * colors indices to curses color indices.
			 */
			static char map[] = { 0, 4, 2, 6, 1, 5, 3, 7 };
			/*
			 * graphic card color   curses color
			 * ---------------------------------
			 *  0 black             0 black
			 *  1 blue              1 red
			 *  2 green             2 green
			 *  3 cyan              3 brown
			 *  4 red               4 blue
			 *  5 magenta           5 magenta
			 *  6 brown             6 cyan
			 *  7 white             7 white
			 *  8 gray              0 black + A_BOLD
			 *  9 light blue        1 red + A_BOLD
			 * 10 light green       2 green + A_BOLD
			 * 11 light cyan        3 brown + A_BOLD
			 * 12 light red         4 blue + A_BOLD
			 * 13 light magenta     5 magenta + A_BOLD
			 * 14 yellow            6 cyan + A_BOLD
			 * 15 light white       7 white + A_BOLD
			 */
			/*
			 * Extract color informations.
			 *
			 * - Bit 7 is set if blink is enabled.
			 * - The background color is stored in the bits 6, 5
			 *   and 4.
			 * - Bit 3 is set if the foreground color is
			 *   highlight.
			 * - The foreground color is stored in the 3 lower
			 *   bits (2, 1 and 0).
			 */
			back = (i >> 4) & 0x07;
			bold = i & 0x08;
			fore = i & 0x07;
			/*
			 * Now build the attribute value.
			 */
			attributeMap[i] = COLOR_PAIR((7 - map[fore]) * 8 +
						     map[back]);
			if (bold != 0)
				attributeMap[i] |= A_BOLD;
		}
	}
	/*
	 * This sets the standard attribute mapping for a monochrome output.
	 */
	else if (TScreen::screenMode == TScreen::smMono) {
		attributeMap[0x07] = A_NORMAL;
		attributeMap[0x0f] = A_BOLD;
		attributeMap[0x70] = A_REVERSE;
	}
}

static void startcurses()
{
	initscr();		/* initialize the curses library */
	keypad(stdscr, TRUE);	/* enable keyboard mapping */
	cbreak();		/* do not wait for \n */
	noecho();		/* do not echo input */
	if (has_colors())
		start_color();
	timeout(0);		/* set getch() in non-blocking mode */
	refresh();		/* fix for new ncurses packages */
	selectPalette();	/* select the more appropiate palette */
	TScreen::drawCursor(0);	/* hide the cursor */
	TScreen::drawMouse(1);	/* draw the mouse pointer */

	/* if possible we should use curses semigraphical characters */

#ifndef DISABLE_ACS
	pcToAscii[4] = ACS_DIAMOND;	/* 4 */
	pcToAscii[16] = ACS_RARROW;	/* 16 */
	pcToAscii[17] = ACS_LARROW;	/* 17 */
	pcToAscii[24] = ACS_UARROW;	/* 24 */
	pcToAscii[25] = ACS_DARROW;	/* 25 */
	pcToAscii[26] = ACS_RARROW;	/* 26 */
	pcToAscii[27] = ACS_LARROW;	/* 27 */
	pcToAscii[28] = ACS_LLCORNER;	/* 28 */
	pcToAscii[30] = ACS_UARROW;	/* 30 */
	pcToAscii[31] = ACS_DARROW;	/* 31 */
#ifdef ACS_STERLING
	pcToAscii[156] = ACS_STERLING;	/* 156 */
#endif
	pcToAscii[169] = ACS_ULCORNER;	/* 169 */
	pcToAscii[170] = ACS_URCORNER;	/* 170 */
	pcToAscii[174] = ACS_LARROW;	/* 174 */
	pcToAscii[175] = ACS_RARROW;	/* 175 */
	pcToAscii[176] = ACS_BOARD;	/* 176 */
	pcToAscii[177] = ACS_CKBOARD;	/* 177 */
	pcToAscii[178] = ACS_CKBOARD;	/* 178 */
	pcToAscii[179] = ACS_VLINE;	/* 179 */
	pcToAscii[180] = ACS_RTEE;	/* 180 */
	pcToAscii[181] = ACS_RTEE;	/* 181 */
	pcToAscii[182] = ACS_RTEE;	/* 182 */
	pcToAscii[183] = ACS_URCORNER;	/* 183 */
	pcToAscii[184] = ACS_URCORNER;	/* 184 */
	pcToAscii[185] = ACS_RTEE;	/* 185 */
	pcToAscii[186] = ACS_VLINE;	/* 186 */
	pcToAscii[187] = ACS_URCORNER;	/* 187 */
	pcToAscii[188] = ACS_LRCORNER;	/* 188 */
	pcToAscii[189] = ACS_LRCORNER;	/* 189 */
	pcToAscii[190] = ACS_LRCORNER;	/* 190 */
	pcToAscii[191] = ACS_URCORNER;	/* 191 */
	pcToAscii[192] = ACS_LLCORNER;	/* 192 */
	pcToAscii[193] = ACS_BTEE;	/* 193 */
	pcToAscii[194] = ACS_TTEE;	/* 194 */
	pcToAscii[195] = ACS_LTEE;	/* 195 */
	pcToAscii[196] = ACS_HLINE;	/* 196 */
	pcToAscii[197] = ACS_PLUS;	/* 197 */
	pcToAscii[198] = ACS_LTEE;	/* 198 */
	pcToAscii[199] = ACS_LTEE;	/* 199 */
	pcToAscii[200] = ACS_LLCORNER;	/* 200 */
	pcToAscii[201] = ACS_ULCORNER;	/* 201 */
	pcToAscii[202] = ACS_BTEE;	/* 202 */
	pcToAscii[203] = ACS_TTEE;	/* 203 */
	pcToAscii[204] = ACS_LTEE;	/* 204 */
	pcToAscii[205] = ACS_HLINE;	/* 205 */
	pcToAscii[206] = ACS_PLUS;	/* 206 */
	pcToAscii[207] = ACS_BTEE;	/* 207 */
	pcToAscii[208] = ACS_BTEE;	/* 208 */
	pcToAscii[209] = ACS_TTEE;	/* 209 */
	pcToAscii[210] = ACS_TTEE;	/* 210 */
	pcToAscii[211] = ACS_LLCORNER;	/* 211 */
	pcToAscii[212] = ACS_LLCORNER;	/* 212 */
	pcToAscii[213] = ACS_ULCORNER;	/* 213 */
	pcToAscii[214] = ACS_ULCORNER;	/* 214 */
	pcToAscii[215] = ACS_PLUS;	/* 215 */
	pcToAscii[216] = ACS_PLUS;	/* 216 */
	pcToAscii[217] = ACS_LRCORNER;	/* 217 */
	pcToAscii[218] = ACS_ULCORNER;	/* 218 */
	pcToAscii[219] = ACS_BLOCK;	/* 219 */
	pcToAscii[220] = ACS_BLOCK;	/* 220 */
	pcToAscii[221] = ACS_BLOCK;	/* 221 */
	pcToAscii[222] = ACS_BLOCK;	/* 222 */
	pcToAscii[223] = ACS_BLOCK;	/* 223 */
#ifdef ACS_PI
	pcToAscii[245] = ACS_PI;	/* 227 */
#endif
	pcToAscii[241] = ACS_PLMINUS;	/* 241 */
#ifdef ACS_GEQUAL
	pcToAscii[242] = ACS_GEQUAL;	/* 242 */
#endif
#ifdef ACS_LEQUAL
	pcToAscii[243] = ACS_LEQUAL;	/* 243 */
#endif
	pcToAscii[248] = ACS_DEGREE;	/* 248 */
	pcToAscii[254] = ACS_BULLET;	/* 254 */
#endif
}

static void stopcurses()
{
	/*
	 * The erase and werase routines copy blanks to every position in the
	 * window, clearing the screen.  The clear and wclear routines are
	 * like erase and werase, but they also call clearok, so that the
	 * screen is cleared completely on the next call to wrefresh for that
	 * window and repainted from scratch.
	 */
	clear();		/* blank the screen */
	refresh();		/* this is necessary */
	keypad(stdscr, FALSE);	/* disable keyboard mapping */
	nocbreak();		/* wait for \n */
	echo();			/* echo input */
	endwin();		/* close the curses library */
}

/*
 * Show a warning message.
 */

static int confirmExit()
{
	/* we need the buffer address */

	class MyBuffer:public TDrawBuffer {
	      public:
		ushort * getBufAddr() {
			return data;
	}};
	MyBuffer b;
	static char msg[] = "Warning: are you sure you want to quit ?";

	b.moveChar(0, ' ', 0x4f, TScreen::screenWidth);
	b.moveStr(max((TScreen::screenWidth - (int)(sizeof(msg) - 1)) / 2,
		      0), msg, 0x4f);
	TScreen::writeRow(0, b.getBufAddr(), TScreen::screenWidth);

	timeout(-1);		/* set getch() in blocking mode */
	int key = getch();
	timeout(0);		/* set getch() in non-blocking mode */
	return toupper(key) == 'Y';
}

static void freeResources()
{
	TScreen::drawMouse(0);
#ifdef ENABLE_GPM
	gpmClose();
#endif
	stopcurses();
#ifdef ENABLE_FBSDM
	fbsdmClose();
#endif
	delete[]TScreen::screenBuffer;
	LOG("terminated");
}

/*
 * General signal handler.
 */

static void sigHandler(int signo)
{
	struct sigaction dfl_handler;

	sigemptyset(&dfl_handler.sa_mask);
	dfl_handler.sa_flags = SA_RESTART;
	switch (signo) {
#ifdef ENABLE_FBSDM
	case FBSDM_SIGNAL:
		msFlag++;
		break;
#endif
	case SIGALRM:
		/*
		 * called every DELAY_SIGALRM ms
		 */
		currentTime += DELAY_SIGALRM;
		break;
	case SIGCONT:
		/*
		 * called when the user restart the process after a ctrl-z
		 */
		LOG("continuing process");
		TScreen::resume();

		/* re-enable SIGTSTP */

		dfl_handler.sa_handler = sigHandler;
		sigaction(SIGTSTP, &dfl_handler, nullptr);
		break;
	case SIGINT:
	case SIGQUIT:
		/*
		 * These signals are now trapped.
		 * Date: Wed, 12 Feb 1997 10:45:55 +0100 (MET)
		 *
		 * Ignore SIGINT and SIGQUIT to avoid recursive calls.
		 */
		dfl_handler.sa_handler = SIG_IGN;
		sigaction(SIGINT, &dfl_handler, nullptr);
		sigaction(SIGQUIT, &dfl_handler, nullptr);

		/* ask the user what to do */

		if (confirmExit()) {
			freeResources();	/* do a nice exit */
			exit(EXIT_FAILURE);
		}
		doRepaint++;

		/* re-enable SIGINT and SIGQUIT */

		dfl_handler.sa_handler = sigHandler;
		sigaction(SIGINT, &dfl_handler, nullptr);
		sigaction(SIGQUIT, &dfl_handler, nullptr);
		break;
	case SIGTSTP:
		/*
		 * called when the user presses ctrl-z
		 */
		TScreen::suspend();
		LOG("process stopped");

		/* use the default handler for SIGTSTP */

		dfl_handler.sa_handler = SIG_DFL;
		sigaction(SIGTSTP, &dfl_handler, nullptr);
		raise(SIGTSTP);	/* stop the process */
		break;
	case SIGWINCH:
		doResize++;
		break;
	default:
		;
	}
}

/*
 * CLASS FUNCTIONS
 */

/*
 * TScreen constructor.
 */

TScreen::TScreen()
{
	char *p = getenv("TVLOG");
	if (p != nullptr && *p != '\0') {
		xlog.open(p);
		LOG("using environment variable TVLOG=" << p);
	} else
		xlog.open("/dev/null");
	env[0] = '\0';
	if ((p = getenv("TVOPT")) != nullptr) {
		LOG("using environment variable TVOPT=" << p);
		for (char *d = env; *p != '\0'; p++)
			*d++ = tolower(*p);
	}

	/* acquire screen size */

	winsize win;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
	if (win.ws_col > 0 && win.ws_row > 0) {
		screenWidth = range(win.ws_col, 4, maxViewWidth);
		screenHeight = range(win.ws_row, 4, 80);
	} else {
		LOG("unable to detect screen size, using 80x25");
		screenWidth = 80;
		screenHeight = 25;
	}
#ifdef __FreeBSD__
	/*
	 * Kludge: until we find a right way to fix the "last-line" display
	 * problem, this is a solution.
	 */
	screenHeight--;
#endif
	LOG("screen size is " << (int)screenWidth << "x" << (int)screenHeight);
	screenBuffer = new ushort[screenWidth * screenHeight];

	/* internal stuff */

	curX = curY = 0;
	currentTime = doRepaint = doResize = evLength = 0;
	evIn = evOut = &evQueue[0];
	kbEscTimer.stop();
	msAutoTimer.stop();
	msOldButtons = msWhere.x = msWhere.y = 0;
	wakeupTimer.start(DELAY_WAKEUP);

	/* setup file descriptors */

	FD_ZERO(&fdSetRead);
	FD_ZERO(&fdSetWrite);
	FD_ZERO(&fdSetExcept);
	FD_SET(STDIN_FILENO, &fdSetRead);

#ifdef ENABLE_FBSDM
	fbsdmInit();
#endif
	/*
	 * We have to call startcurses() before the gpmInit(), otherwise we
	 * shall not have the mouse (see the curs_mouse manual page for more
	 * details).
	 */
	startcurses();		/* curses stuff */
#ifdef ENABLE_GPM
	gpmInit();
#endif
	/* catch useful signals */

	struct sigaction dfl_handler;

	dfl_handler.sa_handler = sigHandler;
	sigemptyset(&dfl_handler.sa_mask);
	dfl_handler.sa_flags = SA_RESTART;

#ifdef ENABLE_FBSDM
	sigaction(FBSDM_SIGNAL, &dfl_handler, nullptr);
#endif
	sigaction(SIGALRM, &dfl_handler, nullptr);
	sigaction(SIGCONT, &dfl_handler, nullptr);
	sigaction(SIGINT, &dfl_handler, nullptr);
	sigaction(SIGQUIT, &dfl_handler, nullptr);
	sigaction(SIGTSTP, &dfl_handler, nullptr);
	sigaction(SIGWINCH, &dfl_handler, nullptr);

	/* generates a SIGALRM signal every DELAY_SIGALRM ms */

	struct itimerval timer;
	timer.it_interval.tv_usec = timer.it_value.tv_usec =
	    DELAY_SIGALRM * 1000;
	timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
	setitimer(ITIMER_REAL, &timer, nullptr);
}

/*
 * TScreen destructor.
 */

TScreen::~TScreen()
{
	freeResources();
}

void TScreen::resume()
{
	startcurses();
#ifdef ENABLE_GPM
	gpmResume();
#endif
	doRepaint++;
}

void TScreen::suspend()
{
#ifdef ENABLE_GPM
	gpmSuspend();
#endif
	stopcurses();
}

/*
 * Gets an event from the queue.
 */

void TScreen::getEvent(TEvent & event)
{
	event.what = evNothing;
	if (doRepaint > 0) {
		doRepaint = 0;
		event.message.command = cmSysRepaint;
		event.what = evCommand;
	} else if (doResize > 0) {
		/*
		 * wresize(...) generates a segmentation fault.  Any idea ?
		 *
		 * Added the gpmSuspend and gpmResume function calls,
		 * otherwise we loose mouse.
		 */
#ifdef ENABLE_GPM
		gpmSuspend();
#endif
		clear();	/* blank the screen */
		refresh();	/* this is necessary */
		stopcurses();
		startcurses();
#ifdef ENABLE_GPM
		gpmResume();
#endif
		doResize = 0;
		winsize win;
		ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
		if (win.ws_col > 0 && win.ws_row > 0) {
			screenWidth = range(win.ws_col, 4, maxViewWidth);
			screenHeight = range(win.ws_row, 4, 80);
			delete[]screenBuffer;
			screenBuffer = new ushort[screenWidth * screenHeight];
			LOG("screen resized to " << (int)screenWidth << "x"
			    << (int)screenHeight);
		}
		event.message.command = cmSysResize;
		event.what = evCommand;
	} else if (evLength > 0) {	/* handles pending events */
		evLength--;
		event = *evOut;
		if (++evOut >= &evQueue[eventQSize])
			evOut = &evQueue[0];
	}
#ifdef ENABLE_FBSDM
	else if (msFlag > 0) {
		msFlag--;
		fbsdmHandle();
	}
#endif
	else if (msAutoTimer.isExpired()) {
		/*
		 * Now evMouseAuto works well.
		 * Date: Tue, 28 Jan 1997 19:35:31 +0100 (MET)
		 */
		msAutoTimer.start(DELAY_AUTOCLICK_NEXT);
		event.mouse.buttons = msOldButtons;
		event.mouse.where = msWhere;
		event.what = evMouseAuto;
	} else if (wakeupTimer.isExpired()) {
		wakeupTimer.start(DELAY_WAKEUP);
		event.message.command = cmSysWakeup;
		event.what = evCommand;
	} else {
		fdActualRead = fdSetRead;
		fdActualWrite = fdSetWrite;
		fdActualExcept = fdSetExcept;
		int kbReady = 0;
		int msReady = 0;

#ifdef ENABLE_GPM
		if (msFd >= 0 && !gpm_flag) {	/* check if mouse is alive */
			LOG("lost connection to gpm server");
			drawMouse(0);
			gpmClose();
		}
#endif
		/*
		 * suspend until there is a signal or some data in file
		 * descriptors
		 */
		if (select(FD_SETSIZE, &fdActualRead, &fdActualWrite,
			   &fdActualExcept, nullptr) > 0) {
			kbReady = FD_ISSET(STDIN_FILENO, &fdActualRead);
#ifdef ENABLE_GPM
			msReady = msFd >= 0 && FD_ISSET(msFd, &fdActualRead);
#endif
		}
		if (kbReady || kbEscTimer.isRunning())
			kbHandle();
#ifdef ENABLE_GPM
		if (msReady)
			gpmHandle();
#endif
		if (!(kbReady || msReady)) {
			/*
			 * We are here because something (neither mouse nor
			 * stdin) triggered select().  It should be a user
			 * defined file descriptor so we must respond with
			 * a call to TProgram::idle().  The user can redefine
			 * TProgram::idle() to check its own file descriptors.
			 */
			wakeupTimer.start(DELAY_WAKEUP);
			event.message.command = cmSysWakeup;
			event.what = evCommand;
		}
	}
}

/*
 * Generates a beep.
 */

void TScreen::makeBeep()
{
	beep();
	refresh();
}

/*
 * Puts an event in the queue.  If the queue is full the event will be
 * discarded.
 */

void TScreen::putEvent(TEvent & event)
{
	if (evLength < eventQSize) {
		evLength++;
		*evIn = event;
		if (++evIn >= &evQueue[eventQSize])
			evIn = &evQueue[0];
	}
}

/*
 * Hides or shows the cursor.
 */

void TScreen::drawCursor(int show)
{
	if (show)
		moveCursor(curX, curY);
	else
		moveCursor(screenWidth - 1, screenHeight - 1);
#ifdef NCURSES_VERSION
	if (show)
		curs_set(1);	/* cursor normal */
	else
		curs_set(0);	/* cursor invisible */
#endif
}

/*
 * Hides or shows the mouse pointer.
 */

void TScreen::drawMouse(int show)
{
#if defined(ENABLE_FBSDM) || defined(ENABLE_GPM)
#ifdef ENABLE_FBSDM
	if (msUseArrow) {
		mouse_info_t mi;

		mi.operation = MOUSE_HIDE;
		ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi);
		if (show) {
			mi.operation = MOUSE_SHOW;
			ioctl(STDOUT_FILENO, CONS_MOUSECTL, &mi);
		}
	} else
#endif
	{
#ifdef ENABLE_GPM
		if (msFd < 0)
			return;
#endif
		int addr = screenWidth * msWhere.y + msWhere.x;
		ushort cell = screenBuffer[addr];
		{
			int code = cell & 0xff;
			int color = (cell & 0xff00) >> 8;

			if (screenMode == smCO80) {
				/* invert the colors */

				if (show)
					color ^= 0x7f;
			} else if (screenMode == smMono) {
				/* rotate the colors */

				if (show)
					switch (color) {
					case 0x07:
						color = 0x70;
						break;
					case 0x0f:
						color = 0x70;
						break;
					case 0x70:
						color = 0x0f;
						break;
					default:
						;
					}
			}
			move(msWhere.y, msWhere.x);
			attrset(attributeMap[color]);
			addch(pcToAscii[code]);
			move(curY, curX);
			refresh();	/* this is necessary */
		}
	}
#endif
}

/*
 * Moves the cursor to another place.
 */

void TScreen::moveCursor(int x, int y)
{
	move(y, x);
	refresh();		/* this is necessary */
	curX = x;
	curY = y;
}

/*
 * Draws a line of text on the screen.
 */

void TScreen::writeRow(int dst, ushort *src, int len)
{
	int x = dst % TScreen::screenWidth;
	int y = dst / TScreen::screenWidth;

	move(y, x);
	while (len-- > 0) {
		int code = *src & 0xff;	/* character code */
		int color = (*src & 0xff00) >> 8;	/* color code */

		attrset(attributeMap[color]);
		addch(pcToAscii[code]);
		src++;
	}
	move(curY, curX);
	/* refresh(); *//* not really necessary */
}

/*
 * Returns the length of a file.
 */

long int filelength(int fd)
{
	struct stat s;

	/*
	 * This should handle any regular file.
	 */
	if (fstat(fd, &s) == 0 && s.st_size > 0)
		return s.st_size;
	else {
		char buf[1024];
		int len, size = 0;

		/*
		 * This works with special files which are not empty, even if
		 * their size is zero, like those in the `/proc' directory.
		 */
		off_t old = lseek(fd, 0, SEEK_CUR);	/* save position */
		lseek(fd, 0, SEEK_SET);	/* go to the beginning */
		while ((len = read(fd, buf, sizeof(buf))) > 0) {
			size += len;
		}
		lseek(fd, old, SEEK_SET);	/* restore old position */
		return size;
	}
}

/*
 * Expands a path into its directory and file components.
 */

void expandPath(const char *path, char *dir, char *file)
{
	const char *tag = strrchr(path, '/');

	/* the path is in the form /dir1/dir2/file ? */

	if (tag != nullptr) {
		strcpy(file, tag + 1);
		strncpy(dir, path, tag - path + 1);
		dir[tag - path + 1] = '\0';
	} else {
		/* there is only the file name */

		strcpy(file, path);
		dir[0] = '\0';
	}
}

void fexpand(char *path)
{
	char dir[PATH_MAX];
	char file[PATH_MAX];
	char oldPath[PATH_MAX];

	expandPath(path, dir, file);
	getcwd(oldPath, sizeof(oldPath));
	chdir(dir);
	getcwd(dir, sizeof(dir));
	chdir(oldPath);
	if (strcmp(dir, "/") == 0)
		sprintf(path, "/%s", file);
	else
		sprintf(path, "%s/%s", dir, file);
}
