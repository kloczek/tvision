/*
 * util.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( __UTIL_H )
#define __UTIL_H

/** \file util.h
 * util.h
 */

/**
 * Undocumented.
 */
inline int min( int a, int b )
{
    return (a>b) ? b : a;
}

/**
 * Undocumented.
 */
inline int max( int a, int b )
{
    return (a<b) ? b : a;
}

/**
 * Undocumented.
 */
void fexpand( char * );
/**
 * Undocumented.
 */
char hotKey( const char *s );

/** \fn ctrlToArrow( ushort keyCode )
 * Converts a WordStar-compatible control key code to the corresponding cursor
 * key code. If the low byte of `keyCode' matches one of the following key
 * values, the result is the corresponding constant. Otherwise, `keyCode' is
 * returned unchanged.
 *
 * <pre>
 * Keystroke Lo(keyCode) Result
 *
 * Ctrl-A    0x01        kbHome
 * Ctrl-C    0x03        kbPgDn
 * Ctrl-D    0x04        kbRight
 * Ctrl-E    0x05        kbUp
 * Ctrl-F    0x06        kbEnd
 * Ctrl-G    0x07        kbDel
 * Ctrl-H    0x08        kbBack
 * Ctrl-R    0x12        kbPgUp
 * Ctrl-S    0x13        kbLeft
 * Ctrl-V    0x16        kbIns
 * Ctrl-X    0x18        kbDown
 * </pre>
 */
ushort ctrlToArrow( ushort keyCode );

/** \fn getAltChar( ushort keyCode )
 * Returns the character ch for which Alt-ch produces the 2-byte scan code
 * given by the argument `keyCode'. This function gives the reverse mapping to
 * getAltCode().
 * @see getAltCode
 */
char getAltChar( ushort keyCode );

/** \fn getAltCode( char ch )
 * Returns the 2-byte scan code (key code) corresponding to Alt-ch. This
 * function gives the reverse mapping to getAltChar().
 * @see getAltChar
 */
ushort getAltCode( char ch );

/** \fn getCtrlChar( ushort keyCode )
 * Returns the character, ch, for which Ctrl+ch produces the 2-byte scan code
 * given by the argument `keyCode'. Gives the reverse mapping to
 * getCtrlCode().
 * @see getCtrlCode
 */
char getCtrlChar( ushort keyCode );

/** \fn getCtrlCode( uchar ch )
 * Returns the 2-byte scan code (keycode) corresponding to Ctrl+ch, where `ch'
 * is the argument. This function gives the reverse mapping to getCtrlChar().
 * @see getCtrlChar
 */
ushort getCtrlCode( uchar ch );

/** \fn historyCount( uchar id )
 * Returns the number of strings in the history list corresponding to ID
 * number `id'.
 * @see THistInit
 * @see THistory
 * @see THistoryViewer
 * @see THistoryWindow
 * @see historyAdd
 * @see historyStr
 */
ushort historyCount( uchar id );

/** \fn historyStr( uchar id, int index )
 * Returns the index'th string in the history list corresponding to ID number
 * `id'.
 * @see THistInit
 * @see THistory
 * @see THistoryViewer
 * @see THistoryWindow
 * @see historyAdd
 * @see historyCount
 */
const char *historyStr( uchar id, int index );

/** \fn historyAdd( uchar id, const char *str )
 * Adds the string `str' to the history list indicated by `id'.
 * @see THistInit
 * @see THistory
 * @see THistoryViewer
 * @see THistoryWindow
 * @see historyCount
 * @see historyStr
 */
void historyAdd( uchar id, const char *str );

/** \fn cstrlen( const char *s )
 * Returns the length of string `s', where `s' is a control string using tilde
 * characters (`~') to designate hot keys. The tildes are excluded from the
 * length of the string, as they will not appear on the screen. For example,
 * given the argument "~B~roccoli", cstrlen() returns 8.
 */
int cstrlen( const char *s );

class TView;

/** \fn message( TView *receiver, ushort what, ushort command, void *infoPtr )
 * message() sets up a command event with the arguments `event', `command',
 * and `infoPtr', and then, if possible, invokes receiver->handleEvent() to
 * handle this event.
 * @see TView::handleEvent
 *
 * message() returns 0 if receiver is 0, or if the event is not handled
 * successfully. If the event is handled successfully (that is, if
 * @ref TView::handleEvent() returns event.what as evNothing), the function
 * returns event.infoPtr.
 *
 * The latter can be used to determine which view actually handled the
 * dispatched event. The event argument is usually set to evBroadcast.
 *
 * For example, the default @ref TScrollBar::scrollDraw() sends the following
 * message to the scroll bar's owner:
 *
 * <pre>
 * message(owner, @ref evBroadcast, cmScrollBarChanged, this);
 * </pre>
 *
 * The above message ensures that the appropriate views are redrawn whenever
 * the scroll bar's value changes.
 */
void *message( TView *receiver, ushort what, ushort command, void *infoPtr );

/** \fn lowMemory()
 * Calls TVMemMgr::safetyPoolExhausted() to check the state of the safety
 * pool.
 * @see TVMemMgr::safetyPoolExhausted
 */
Boolean lowMemory();

/** \fn newStr( const char *s )
 * Dynamic string creation. If `s' is a null pointer, newStr() returns a 0
 * pointer; otherwise, strlen(s)+ 1 bytes are allocated, containing a copy of
 * `s' (with a terminating '\0'), and a pointer to the first byte is returned.
 *
 * You can use delete to dispose of such strings.
 */
char *newStr( const char *s );

/**
 * Undocumented.
 */
Boolean isDir( const char *str );
/**
 * Undocumented.
 */
Boolean pathValid( const char *path );
/**
 * Undocumented.
 */
Boolean validFileName( const char *fileName );
/**
 * Undocumented.
 */
void getCurDir( char *dir );
/**
 * Undocumented.
 */
Boolean isWild( const char *f );

//from system.cc

/**
 * Undocumented.
 */
void expandPath(const char *path, char *dir, char *file);
/**
 * Undocumented.
 */
long int filelength(int fd);

#endif  // __UTIL_H
