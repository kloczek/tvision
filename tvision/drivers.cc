/*
 * drivers.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TDrawBuffer
#include <tv.h>

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveBuf                                                  */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent - character position within the buffer where the data      */
/*               is to go                                                 */
/*                                                                        */
/*      source - far pointer to an array of character/attribute pairs     */
/*                                                                        */
/*      attr   - attribute to be used for all characters (0 to retain     */
/*               the attribute from 'source')                             */
/*                                                                        */
/*      count   - number of character/attribute pairs to move             */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveBuf(ushort indent, const void *source,
			  ushort attr, ushort count)
{
	ushort *dest = &data[indent];
	uchar *s = (uchar *) source;

	if (attr != 0)
		for (; count; --count, ++s, ++dest) {
			*dest = *s | ((attr & 0xff) << 8);
	} else
		while (count--)
			*(uchar *) dest++ = *s++;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveChar                                                 */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      c       - character to be put into the buffer                     */
/*                                                                        */
/*      attr    - attribute to be put into the buffer                     */
/*                                                                        */
/*      count   - number of character/attribute pairs to put into the     */
/*                buffer                                                  */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveChar(ushort indent, char c, ushort attr, ushort count)
{
	ushort *dest = &data[indent];

	if (attr != 0)
		for (; count; --count, ++dest) {
			if (c != 0)
				*dest = (*dest & 0xff00) | c;
			*dest = (*dest & 0x00ff) | ((attr & 0xff) << 8);
	} else
		while (count--)
			*(uchar *) dest++ = c;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveCStr                                                 */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      str     - pointer to a 0-terminated string of characters to       */
/*                be moved into the buffer                                */
/*                                                                        */
/*      attrs   - pair of text attributes to be put into the buffer       */
/*                with each character in the string.  Initially the       */
/*                low byte is used, and a '~' in the string toggles       */
/*                between the low byte and the high byte.                 */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveCStr(ushort indent, const char *str, ushort attrs)
{
	ushort *dest = &data[indent];
	int toggle;
	uchar c, curAttr;

	for (curAttr = attrs & 0xff, toggle = 1; (c = *str) != 0; str++) {
		if (c == '~') {
			if (toggle == 0)
				curAttr = attrs & 0xff;
			else
				curAttr = (attrs & 0xff00) >> 8;
			toggle = 1 - toggle;
		} else {
			*dest = (curAttr << 8) | c;
			dest++;
		}
	}
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveStr                                                  */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      str     - pointer to a 0-terminated string of characters to       */
/*                be moved into the buffer                                */
/*                                                                        */
/*      attr    - text attribute to be put into the buffer with each      */
/*                character in the string.                                */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveStr(ushort indent, const char *str, ushort attr)
{
	attr <<= 8;
	while (indent < (sizeof(data) / sizeof((data)[0])) && *str != '\0')
		data[indent++] = (unsigned char)(*str++) + attr;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  ctrlToArrow                                                           */
/*                                                                        */
/*  argument:                                                             */
/*                                                                        */
/*      keyCode - scan code to be mapped to keypad arrow code             */
/*                                                                        */
/*  returns:                                                              */
/*                                                                        */
/*      scan code for arrow key corresponding to Wordstar key,            */
/*      or original key code if no correspondence exists                  */
/*                                                                        */
/*------------------------------------------------------------------------*/
ushort ctrlToArrow(ushort keyCode)
{

#ifndef __UNPATCHED
	static const uchar ctrlCodes[] = {
		kbCtrlS, kbCtrlD, kbCtrlE, kbCtrlX, kbCtrlA,
		kbCtrlF, kbCtrlG, kbCtrlV, kbCtrlR, kbCtrlC, kbCtrlH
	};

	static const ushort arrowCodes[] = {
		kbLeft, kbRight, kbUp, kbDown, kbHome,
		kbEnd, kbDel, kbIns, kbPgUp, kbPgDn, kbBack
	};
#else
	const uchar ctrlCodes[] = {
		kbCtrlS, kbCtrlD, kbCtrlE, kbCtrlX, kbCtrlA,
		kbCtrlF, kbCtrlG, kbCtrlV, kbCtrlR, kbCtrlC, kbCtrlH
	};

	const ushort arrowCodes[] = {
		kbLeft, kbRight, kbUp, kbDown, kbHome,
		kbEnd, kbDel, kbIns, kbPgUp, kbPgDn, kbBack
	};
#endif

	for (int i = 0; i < (int)sizeof(ctrlCodes); i++)
		if ((keyCode & 0x00ff) == ctrlCodes[i])
			return arrowCodes[i];
	return keyCode;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  cstrlen                                                               */
/*                                                                        */
/*  argument:                                                             */
/*                                                                        */
/*      s       - pointer to 0-terminated string                          */
/*                                                                        */
/*  returns                                                               */
/*                                                                        */
/*      length of string, ignoring '~' characters.                        */
/*                                                                        */
/*  Comments:                                                             */
/*                                                                        */
/*      Used in determining the displayed length of command strings,      */
/*      which use '~' to toggle between display attributes                */
/*                                                                        */
/*------------------------------------------------------------------------*/

int cstrlen(const char *s)
{
	int len = 0;
	while (*s != EOS) {
		if (*s++ != '~')
			len++;
	}
	return len;
}
