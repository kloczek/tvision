/*
 * TStrListMaker.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TStringList
#define Uses_TStrIndexRec
#define Uses_TStrListMaker
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <string.h>

const int MAXKEYS = 16;

TStrIndexRec::TStrIndexRec():
count(0)
{
}

TStrListMaker::TStrListMaker(ushort aStrSize, ushort aIndexSize):
strPos(0),
strSize(aStrSize),
strings(new char[aStrSize]),
indexPos(0), indexSize(aIndexSize), index(new TStrIndexRec[aIndexSize])
{
}

TStrListMaker::~TStrListMaker()
{
	delete strings;

	/* SS: prevent anachronistic stuff */

	delete[]index;
}

void TStrListMaker::closeCurrent()
{
	if (cur.count != 0) {
		index[indexPos++] = cur;
		cur.count = 0;
	}
}

void TStrListMaker::put(ushort key, char *str)
{
	if (cur.count == MAXKEYS || key != cur.key + cur.count)
		closeCurrent();
	if (cur.count == 0) {
		cur.key = key;
		cur.offset = strPos;
	}
	int len = strlen(str);
	strings[strPos] = len;
	memcpy(strings + strPos + 1, str, len);
	strPos += len + 1;
	cur.count++;
}

#if !defined(NO_STREAMABLE)

TStringList::TStringList(StreamableInit):
basePos(0), indexSize(0), index(nullptr)
{
}
#endif

TStringList::~TStringList()
{
	/* SS: prevent anachronistic stuff */

	delete[]index;
}

void TStringList::get(char *dest, ushort key)
{
	if (indexSize == 0) {
		*dest = EOS;
		return;
	}

	TStrIndexRec *cur = index;
	while (cur->key + cur->count - 1 < key && cur - index < indexSize)
		cur++;
#ifndef __UNPATCHED
	if (cur->key + cur->count - 1 < key || cur->key > key)
#else
	if (cur->key + cur->count - 1 < key)
#endif
	{
		*dest = EOS;
		return;
	}
	ip->seekg(basePos + cur->offset);
	int count = key - cur->key;
	do {
		uchar sz = ip->readByte();
		ip->readBytes(dest, sz);
		dest[sz] = EOS;
	} while (count-- > 0);
}

#if !defined(NO_STREAMABLE)

void TStrListMaker::write(opstream & os)
{
	closeCurrent();
	os << strPos;
	os.writeBytes(strings, strPos);
	os << indexPos;
	os.writeBytes(index, indexPos * sizeof(TStrIndexRec));
}

void *TStringList::read(ipstream & is)
{
	ip = &is;

	ushort strSize;
	is >> strSize;

	basePos = is.tellg();
	is.seekg(basePos + strSize);
	is >> indexSize;
	index = new TStrIndexRec[indexSize];
	is.readBytes(index, indexSize * sizeof(TStrIndexRec));
	return this;
}

TStreamable *TStringList::build()
{
	return new TStringList(streamableInit);
}

#endif
