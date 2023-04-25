/*
 * ttypes.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_INC_TTYPES_H )
#define TV_INC_TTYPES_H

/** \file ttypes.h
 * ttypes.h
 */

/** \enum Boolean
 * Assigns the integer values 0 to False and 1 to True. Note that the tests
 *
 * if (tesfunc()) {...}
 *
 * and
 *
 * if (testfunc() == True) {...}
 *
 * may not be equivalent.
 */
enum Boolean { False, True };

/** \var ushort
 * Provides a synonym for unsigned short.
 */
typedef unsigned short ushort;

/** \var uchar
 * Provides a synonym for unsigned char.
 */
typedef unsigned char uchar;

/** \var uint
 * Provides a synonym for unsigned int.
 */
typedef unsigned int uint;

/** \var ulong
 * Provides a synonym for unsigned long.
 */
typedef unsigned long ulong;

/** \var EOS
 * A synonym for the end-of-string null character.
 */
const char EOS = '\0';

/** \enum StreamableInit
 * Each streamable class has a special "builder" constructor that takes
 * argument streamableInit. This is defined in this enumeration to provide a
 * unique data type for the constructor argument.
 */
enum StreamableInit { streamableInit };

class ipstream;
class opstream;
class TStreamable;
class TStreamableTypes;

/** \enum ccIndex
 * Used to index and count the items in a collection.
 */
typedef int ccIndex;

/** \enum ccTestFunc
 * Used in iterator functions to provide a test function and argument list to
 * be applied to a range of items in a collection.
 */
typedef Boolean(*ccTestFunc) (void *item, void *arg);

/** \enum ccAppFunc
 * Used in iterator functions to provide an action function and argument list
 * to be applied to a range of items in a collection.
 */
typedef void (*ccAppFunc)(void *item, void *arg);

/** \var ccNotFound
 * The ccIndex value returned by various collection-search functions if the
 * search fails.
 */
const int ccNotFound = -1;

/** \var specialChars
 * Defines the indicator characters used to highlight the focused view in
 * monochrome video mode. These characters are displayed if the showMarkers
 * variable is True.
 * @see TView::showMarkers
 */
extern const uchar specialChars[];

#endif // TV_INC_TTYPES_H
