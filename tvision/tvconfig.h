/*
 * tvconfig.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_INC_CONFIG_H )
#define TV_INC_CONFIG_H

#include <limits.h>

/** \file tvconfig.h
 * tvconfig.h
 */

/** \var eventQSize
 * Specifies the size of the event queue.
 */
const int eventQSize = 16;

/** \var maxCollectionSize
 * maxCollectionSize determines the maximum number of elements that may be
 * contained in a collection, which is essentially the number of pointers that
 * can fit in INT_MAX (= 2^31-1) bytes.
 */
const int maxCollectionSize = INT_MAX / sizeof(void *);

/** \var maxViewWidth
 * Sets the maximum width of a view.
 */
const int maxViewWidth = 1024;

/** \var maxFindStrLen
 * Gives the maximum length for a find string in TEditor applications.
 * @see TEditor
 */
const int maxFindStrLen = 80;

/** \var maxReplaceStrLen
 * Gives the maximum length for a replacement string in TEditor applications.
 * @see TEditor
 */
const int maxReplaceStrLen = 80;

#endif // TV_INC_CONFIG_H
