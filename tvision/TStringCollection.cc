/*
 * TStringCollection.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TStringCollection
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <string.h>

TStringCollection::TStringCollection(short aLimit,
				     short aDelta):TSortedCollection(aLimit,
								     aDelta)
{
}

int TStringCollection::compare(void *key1, void *key2)
{
	return strcmp((char *)key1, (char *)key2);
}

void TStringCollection::freeItem(void *item)
{
	delete[](char *)item;
}

TStreamable *TStringCollection::build()
{
	return new TStringCollection(streamableInit);
}

void TStringCollection::writeItem(void *obj, opstream & os)
{
	os.writeString((const char *)obj);
}

void *TStringCollection::readItem(ipstream & is)
{
	return is.readString();
}
