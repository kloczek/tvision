/*
 * helpbase.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TStreamableClass
#define Uses_TPoint
#define Uses_TStreamable
#define Uses_ipstream
#define Uses_opstream
#define Uses_fpstream
#define Uses_TRect
#include <tv.h>

#if !defined( TV_INC_HELP_H )
#include "helpbase.h"
#endif // TV_INC_HELP_H

#if !defined( TV_INC_UTIL_H )
#include "util.h"
#endif // TV_INC_UTIL_H

#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

TCrossRefHandler crossRefHandler = notAssigned;

// THelpTopic

const char *const THelpTopic::name = "THelpTopic";

void THelpTopic::write(opstream & os)
{
	writeParagraphs(os);
	writeCrossRefs(os);

}

void *THelpTopic::read(ipstream & is)
{
	readParagraphs(is);
	readCrossRefs(is);
	width = 0;
	lastLine = INT_MAX;
	return this;
}

TStreamable *THelpTopic::build()
{
	return new THelpTopic(streamableInit);
}

TStreamableClass RHelpTopic(THelpTopic::name,
			    THelpTopic::build, __DELTA(THelpTopic)
    );

THelpTopic::THelpTopic():TObject()
{
	paragraphs = nullptr;
	numRefs = 0;
	crossRefs = nullptr;
	width = 0;
	lastOffset = 0;
	lastLine = INT_MAX;
	lastParagraph = nullptr;
};

void THelpTopic::readParagraphs(ipstream & s)
{
	int i;
	ushort size;
	TParagraph **pp;
	int temp;

	s >> i;
	pp = &paragraphs;
	while (i > 0) {
		s >> size;
		*pp = new TParagraph;
		(*pp)->text = new char[size];
		(*pp)->size = (ushort) size;
		s >> temp;
		(*pp)->wrap = Boolean(temp);
		s.readBytes((*pp)->text, (*pp)->size);
		pp = &((*pp)->next);
		--i;
	}
	*pp = nullptr;
}

void THelpTopic::readCrossRefs(ipstream & s)
{
	int i;
	TCrossRef *crossRefPtr;

	s >> numRefs;
	crossRefs = new TCrossRef[numRefs];
	for (i = 0; i < numRefs; ++i) {
		crossRefPtr = (TCrossRef *) crossRefs + i;

		/*
		 * SS: TCrossRef size is 9 bytes (int, int, char), but
		 * sizeof(TCrossRef) is rounded to 12.
		 */
		s >> crossRefPtr->ref;	/* int */
		s >> crossRefPtr->offset;	/* int */
		s >> crossRefPtr->length;	/* char */
//        s.readBytes(crossRefPtr, sizeof(TCrossRef));
	}
}

void THelpTopic::disposeParagraphs()
{
	TParagraph *p, *t;

	p = paragraphs;
	while (p != nullptr) {
		t = p;
		p = p->next;
		delete t->text;
		delete t;
	}
}

THelpTopic::~THelpTopic()
{
	TCrossRef *crossRefPtr;

	disposeParagraphs();
	if (crossRefs != nullptr) {
		crossRefPtr = (TCrossRef *) crossRefs;
		delete[]crossRefPtr;
	}
}

void THelpTopic::addCrossRef(TCrossRef ref)
{
	TCrossRef *p;
	TCrossRef *crossRefPtr;

	p = new TCrossRef[numRefs + 1];
	if (numRefs > 0) {
		crossRefPtr = crossRefs;
		memmove(p, crossRefPtr, numRefs * sizeof(TCrossRef));
		delete[]crossRefPtr;
	}
	crossRefs = p;
	crossRefPtr = crossRefs + numRefs;
	*crossRefPtr = ref;
	++numRefs;
}

void THelpTopic::addParagraph(TParagraph *p)
{
	TParagraph *pp, *back;

	if (paragraphs == nullptr)
		paragraphs = p;
	else {
		pp = paragraphs;
		back = pp;
		while (pp != nullptr) {
			back = pp;
			pp = pp->next;
		}
		back->next = p;
	}
	p->next = nullptr;
}

void THelpTopic::getCrossRef(int i, TPoint & loc, uchar & length, int &ref)
{
	int oldOffset, curOffset, offset, paraOffset;
	TParagraph *p;
	int line;
	TCrossRef *crossRefPtr;

	paraOffset = 0;
	curOffset = 0;
	oldOffset = 0;
	line = 0;
	crossRefPtr = crossRefs + i;
	offset = crossRefPtr->offset;
	p = paragraphs;
	while (paraOffset + curOffset < offset) {
		char lbuf[256];

		oldOffset = paraOffset + curOffset;
		wrapText(p->text, p->size, curOffset, p->wrap, lbuf,
			 sizeof(lbuf));
		++line;
		if (curOffset >= p->size) {
			paraOffset += p->size;
			p = p->next;
			curOffset = 0;
		}
	}
	loc.x = offset - oldOffset - 1;
	loc.y = line;
	length = crossRefPtr->length;
	ref = crossRefPtr->ref;
}

char *THelpTopic::getLine(int line, char *buffer, int buflen)
{
	int offset, i;
	TParagraph *p;

	if (lastLine < line) {
		i = line;
		line -= lastLine;
		lastLine = i;
		offset = lastOffset;
		p = lastParagraph;
	} else {
		p = paragraphs;
		offset = 0;
		lastLine = line;
	}
	buffer[0] = 0;
	while (p != nullptr) {
		while (offset < p->size) {
			char lbuf[256];

			--line;
			strncpy(buffer,
				wrapText(p->text, p->size, offset, p->wrap,
					 lbuf, sizeof(lbuf)), buflen);
			if (line == 0) {
				lastOffset = offset;
				lastParagraph = p;
				return buffer;
			}
		}
		p = p->next;
		offset = 0;
	}
	buffer[0] = 0;
	return buffer;
}

int THelpTopic::getNumCrossRefs()
{
	return numRefs;
}

int THelpTopic::numLines()
{
	int offset, lines;
	TParagraph *p;

	offset = 0;
	lines = 0;
	p = paragraphs;
	while (p != nullptr) {
		offset = 0;
		while (offset < p->size) {
			char lbuf[256];
			++lines;
			wrapText(p->text, p->size, offset, p->wrap, lbuf,
				 sizeof(lbuf));
		}
		p = p->next;
	}
	return lines;
}

void THelpTopic::setCrossRef(int i, TCrossRef & ref)
{
	TCrossRef *crossRefPtr;

	if (i < numRefs) {
		crossRefPtr = crossRefs + i;
		*crossRefPtr = ref;
	}
}

void THelpTopic::setNumCrossRefs(int i)
{
	TCrossRef *p, *crossRefPtr;

	if (numRefs == i)
		return;
	p = new TCrossRef[i];
	if (numRefs > 0) {
		crossRefPtr = crossRefs;
		if (i > numRefs)
			memmove(p, crossRefPtr, numRefs * sizeof(TCrossRef));
		else
			memmove(p, crossRefPtr, i * sizeof(TCrossRef));
		delete[]crossRefPtr;
	}
	crossRefs = p;
	numRefs = i;
}

void THelpTopic::setWidth(int aWidth)
{
	width = aWidth;
}

void THelpTopic::writeParagraphs(opstream & s)
{
	int i;
	TParagraph *p;
	int temp;

	p = paragraphs;
	for (i = 0; p != nullptr; ++i)
		p = p->next;
	s << i;
	for (p = paragraphs; p != nullptr; p = p->next) {
		s << p->size;
		temp = int (p->wrap);
		s << temp;
		s.writeBytes(p->text, p->size);
	}
}

void THelpTopic::writeCrossRefs(opstream & s)
{
	int i;
	TCrossRef *crossRefPtr;

	s << numRefs;
	if (crossRefHandler == notAssigned) {
		for (i = 0; i < numRefs; ++i) {
			crossRefPtr = crossRefs + i;
			s << crossRefPtr->ref << crossRefPtr->
			    offset << crossRefPtr->length;
		}
	} else
		for (i = 0; i < numRefs; ++i) {
			crossRefPtr = crossRefs + i;
			(*crossRefHandler) (s, crossRefPtr->ref);
			s << crossRefPtr->offset << crossRefPtr->length;
		}
}

Boolean isBlank(char ch)
{
	if (isspace((uchar) ch))
		return True;
	else
		return False;
}

int scan(char *p, int offset, char c)
{
	char *temp1, *temp2;

	temp1 = p + offset;
	temp2 = strchr(temp1, c);
	if (temp2 == nullptr)
		return 256;
	else {
		if ((int)(temp2 - temp1) <= 256)
			return (int)(temp2 - temp1) + 1;
		else
			return 256;
	}
}

void textToLine(void *text, int offset, int length, char *line)
{
	strncpy(line, (char *)text + offset, length);
	line[length] = 0;
}

char *THelpTopic::wrapText(char *text, int size, int &offset, Boolean wrap,
			   char *lineBuf, int lineBufLen)
{
	int i;

	i = scan(text, offset, '\n');
	if (i + offset > size)
		i = size - offset;
	if ((i >= width) && (wrap == True)) {
		i = offset + width;
		if (i > size)
			i = size;
		else {
			while ((i > offset) && !(isBlank(text[i])))
				--i;
/*
            if (i == offset)
                i = offset + width;
            else
                ++i;
*/
			if (i == offset) {
				i = offset + width;
				while ((i < size) && !isBlank(text[i]))
					++i;
				if (i < size)
					++i;
			} else
				++i;
		}
		if (i == offset)
			i = offset + width;
		i -= offset;
	}
	textToLine(text, offset, min(i, lineBufLen), lineBuf);
	if (lineBuf[min(strlen(lineBuf) - 1, lineBufLen)] == '\n')
		lineBuf[min(strlen(lineBuf) - 1, lineBufLen)] = 0;
	offset += min(i, lineBufLen);
	return lineBuf;
}

// THelpIndex

const char *const THelpIndex::name = "THelpIndex";

void THelpIndex::write(opstream & os)
{
	long *indexArrayPtr;

	os << size;
	for (int i = 0; i < size; ++i) {
		indexArrayPtr = index + i;
		os << *indexArrayPtr;
	}
}

void *THelpIndex::read(ipstream & is)
{
	long *indexArrayPtr;

	is >> size;
	if (size == 0)
		index = nullptr;
	else {
		index = new long[size];
		for (int i = 0; i < size; ++i) {
			indexArrayPtr = index + i;
			is >> *indexArrayPtr;
		}
	}
	return this;
}

TStreamable *THelpIndex::build()
{
	return new THelpIndex(streamableInit);
}

TStreamableClass RHelpIndex(THelpIndex::name,
			    THelpIndex::build, __DELTA(THelpIndex)
    );

THelpIndex::~THelpIndex()
{
	delete[]index;
}

THelpIndex::THelpIndex(void):TObject()
{
	size = 0;
	index = nullptr;
}

long THelpIndex::position(int i)
{
	long *indexArrayPtr;

	if (i < size) {
		indexArrayPtr = index + i;
		return (*indexArrayPtr);
	} else
		return -1;
}

void THelpIndex::add(int i, long val)
{
	int delta = 10;
	long *p;
	int newSize;
	long *indexArrayPtr;

	if (i >= size) {
		newSize = (i + delta) / delta * delta;
		p = new long[newSize];
		if (p != nullptr) {
			memmove(p, index, size * sizeof(long));
			memset(p + size, 0xFF, (newSize - size) * sizeof(long));
		}
		if (size > 0) {
			delete[]index;
		}
		index = p;
		size = newSize;
	}
	indexArrayPtr = index + i;
	*indexArrayPtr = val;
}

// THelpFile

THelpFile::THelpFile(fpstream & s)
{
	long magic;
	long size;

	magic = 0;
	s.seekg(0, std::ios::end);
	size = s.tellg();
	s.seekg(0);
	if (size > (long)sizeof(magic))
		s >> magic;
	if (magic != magicHeader) {
		indexPos = 12;
		s.seekg(indexPos);
		index = new THelpIndex;
		modified = True;
	} else {
		s.seekg(8);
		s >> indexPos;
		s.seekg(indexPos);
		s >> index;
		modified = False;
	}
	stream = &s;
}

THelpFile::~THelpFile(void)
{
	long magic, size;

	if (modified == True) {
		stream->seekp(indexPos);
		*stream << index;
		stream->seekp(0);
		magic = magicHeader;
//
// note: at this time, a bug in filelength leaves the seek pointer at
//       the end of file, so we must save and restore the seek pointer
//       around the call; this can be removed when filelength is fixed.
//
		std::streampos sp = stream->tellp();
		stream->seekg(0, std::ios::end);
		size = (long)stream->tellg() - 8;
		stream->seekp(sp);
		*stream << magic;
		*stream << size;
		*stream << indexPos;
	}
	delete stream;
	delete index;
}

THelpTopic *THelpFile::getTopic(int i)
{
	long pos;
	THelpTopic *topic = nullptr;

	pos = index->position(i);
	if (pos > 0) {
		stream->seekg(pos);
		*stream >> topic;
		return topic;
	} else
		return (invalidTopic());
}

THelpTopic *THelpFile::invalidTopic()
{
	THelpTopic *topic;
	TParagraph *para;

	topic = new THelpTopic;
	para = new TParagraph;
	para->text = newStr(invalidContext);
	para->size = strlen(invalidContext);
	para->wrap = False;
	para->next = nullptr;
	topic->addParagraph(para);
	return topic;
}

void THelpFile::recordPositionInIndex(int i)
{
	index->add(i, indexPos);
	modified = True;
}

void THelpFile::putTopic(THelpTopic *topic)
{
	stream->seekp(indexPos);
	*stream << topic;
	indexPos = stream->tellp();
	modified = True;
}

void notAssigned(opstream &, int)
{
}
