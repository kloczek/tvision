/*
 * drawbuf.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TDrawBuffer ) && !defined( TV_TDrawBuffer )
#define TV_TDrawBuffer

/** \file drawbuf.h
 * drawbuf.h
 */

/**
 * This class implements a video buffer.
 *
 * TDrawBuffer implements a simple, non-view buffer class with member
 * functions for moving characters, attributes, and strings to and from a draw
 * buffer.
 *
 * Every view uses at least one istance of this class in its draw() method.
 * The view draws itself using a TDrawBuffer object. Just before returning
 * from draw(), a call to one of the writeXXXX methods will write the video
 * buffer on the screen.
 * @see TView::draw
 *
 * Each member of the buffer is an attribute & character pair. The attribute
 * is a byte which stores information about foreground and background colors.
 *
 * The contents of a draw buffer are typically used with
 * @ref TView::writeBuf() or @ref TView::writeLine() to display text.
 * @see TView::writeChar
 * @see TView::writeStr
 *
 * Note: pay attention to the size of the buffer! It usually stores only a
 * line of the picture. Its default size is @ref maxViewWidth = 132 pairs.
 * @short Implements a video buffer
 */
class TDrawBuffer {
	friend class TFrame;
	friend class TView;
      public:
    /**
     * Fills the buffer or part of the buffer with an uniform pattern.
     *
     * `indent' is the character position within the buffer where the data is
     * to go. `c' is the character to be put into the buffer. If `c' is 0 the
     * character is not written and the old character is preserved. `attr' is
     * the attribute to be put into the buffer. If `attr' is 0 the attribute
     * is not written and the old attribute is preserved. `count' is the
     * number of character/attribute pairs to put into the buffer.
     */
	void moveChar(ushort indent, char c, ushort attr, ushort count);
    /**
     * Writes a string in the buffer.
     *
     * `indent' is the character position within the buffer where the data is
     * to go. `str' is a pointer to a 0-terminated string of characters to be
     * moved into the buffer. `attr' is the text attribute to be put into the
     * buffer with each character in the string. If `attr' is 0 the attribute
     * is not written and the old attribute is preserved. The characters in
     * `str' are set in the low bytes of each buffer word.
     */
	void moveStr(ushort indent, const char *str, ushort attrs);
    /**
     * Writes a string in the buffer.
     *
     * `indent' is the character position within the buffer where the data is
     * to go. `str' is a pointer to a 0-terminated string of characters to be
     * moved into the buffer. `attrs' is a pair of text attributes to be put
     * into the buffer with each character in the string. Initially the low
     * byte is used, and a `~' in the string toggles between the low byte and
     * the high byte.
     */
	void moveCStr(ushort indent, const char *str, ushort attrs);
    /**
     * Writes a text buffer in this video buffer.
     *
     * `indent' is the character position within the buffer where the data is
     * to go. `source' is a pointer to an array of characters. `attr' is the
     * attribute to be used for all characters (0 to retain the old
     * attribute). `count' is the number of characters to move.
     */
	void moveBuf(ushort indent, const void *source, ushort attr,
		     ushort count);
    /**
     * Writes an attribute.
     *
     * `ident' is the character position within the buffer where the attribute
     * is to go. `attr' is the attribute to write.
     */
	void putAttribute(ushort indent, ushort attr);
    /**
     * Writes a character.
     *
     * `ident' is the character position within the buffer where the character
     * is to go. `c' is the character to write. This call inserts `c' into the
     * lower byte of the calling buffer.
     */
	void putChar(ushort indent, ushort c);
      protected:
    /**
     * Defines the array for this draw buffer.
     */
	 ushort data[maxViewWidth];
};

#define loByte(w)    (((uchar *)&w)[0])
#define hiByte(w)    (((uchar *)&w)[1])

inline void TDrawBuffer::putAttribute(ushort indent, ushort attr)
{
	hiByte(data[indent]) = uchar(attr);
}

inline void TDrawBuffer::putChar(ushort indent, ushort c)
{
	loByte(data[indent]) = uchar(c);
}

#endif // Uses_TDrawBuffer
