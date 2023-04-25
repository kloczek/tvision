/*
 * TCollection.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TNSCollection
#define Uses_opstream
#define Uses_ipstream
#define Uses_TCollection
#include <tv.h>

#include <stdlib.h>
#include <string.h>

TNSCollection::TNSCollection(ccIndex aLimit, ccIndex aDelta):
items(nullptr), count(0), limit(0), delta(aDelta), shouldDelete(True)
{
	setLimit(aLimit);
}

TNSCollection::TNSCollection():
items(nullptr), count(0), limit(0), delta(0), shouldDelete(True)
{
}

TNSCollection::~TNSCollection()
{
	delete items;
}

void TNSCollection::shutDown()
{
	if (shouldDelete)
		freeAll();
	else
		removeAll();
	setLimit(0);
	TObject::shutDown();
}

void *TNSCollection::at(ccIndex index)
{
	if (index < 0 || index >= count)
		error(1, 0);
	return items[index];
}

void TNSCollection::atRemove(ccIndex index)
{
	if (index < 0 || index >= count)
		error(1, 0);

	count--;
	memmove(&items[index], &items[index + 1],
		(count - index) * sizeof(void *));
}

void TNSCollection::atFree(ccIndex index)
{
	void *item = at(index);
	atRemove(index);
	freeItem(item);
}

void TNSCollection::atInsert(ccIndex index, void *item)
{
	if (index < 0)
		error(1, 0);
	if (count == limit)
		setLimit(count + delta);

	memmove(&items[index + 1], &items[index],
		(count - index) * sizeof(void *));
	count++;

	items[index] = item;
}

void TNSCollection::atPut(ccIndex index, void *item)
{
	if (index >= count)
		error(1, 0);

	items[index] = item;
}

void TNSCollection::remove(void *item)
{
	atRemove(indexOf(item));
}

void TNSCollection::removeAll()
{
	count = 0;
}

void TNSCollection::error(ccIndex code, ccIndex)
{
	exit(212 - code);
}

void *TNSCollection::firstThat(ccTestFunc Test, void *arg)
{
	for (ccIndex i = 0; i < count; i++) {
		if (Test(items[i], arg) == True)
			return items[i];
	}
	return nullptr;
}

void *TNSCollection::lastThat(ccTestFunc Test, void *arg)
{
	for (ccIndex i = count; i > 0; i--) {
		if (Test(items[i - 1], arg) == True)
			return items[i - 1];
	}
	return nullptr;
}

void TNSCollection::forEach(ccAppFunc action, void *arg)
{
	for (ccIndex i = 0; i < count; i++)
		action(items[i], arg);
}

void TNSCollection::free(void *item)
{
	remove(item);
	freeItem(item);
}

void TNSCollection::freeAll()
{
	for (ccIndex i = 0; i < count; i++)
		freeItem(at(i));
	count = 0;
}

void TNSCollection::freeItem(void *item)
{
	delete[]static_cast < char *>(item);
}

ccIndex TNSCollection::indexOf(void *item)
{
	for (ccIndex i = 0; i < count; i++)
		if (item == items[i])
			return i;

	error(1, 0);
	return static_cast < ccIndex > (0);
}

ccIndex TNSCollection::insert(void *item)
{
	ccIndex loc = count;
	atInsert(count, item);
	return loc;
}

void TNSCollection::pack()
{
	void **curDst = items;
	void **curSrc = items;
	void **last = items + count;
	while (curSrc < last) {
		if (*curSrc != nullptr)
			*curDst++ = *curSrc;
//        *curSrc++;    /* XXX */
		curSrc++;	/* XXX */
	}
}

void TNSCollection::setLimit(ccIndex aLimit)
{
	if (aLimit < count)
		aLimit = count;
	if (aLimit > maxCollectionSize)
		aLimit = maxCollectionSize;
	if (aLimit != limit) {
		void **aItems;
		if (aLimit == 0)
			aItems = nullptr;
		else {
			aItems = new void *[aLimit];
			if (count != 0 && aItems != nullptr && items != nullptr)
				memcpy(aItems, items, count * sizeof(void *));
		}
		delete items;
		items = aItems;
		limit = aLimit;
	}
}

void TCollection::write(opstream & os)
{
	os << count << limit << delta;
	for (ccIndex idx = 0; idx < count; idx++)
		writeItem(items[idx], os);
}

void *TCollection::read(ipstream & is)
{
	int savedLimit;
	is >> count >> savedLimit >> delta;
	setLimit(savedLimit);
	for (ccIndex idx = 0; idx < count; idx++)
		items[idx] = readItem(is);
	return this;
}

TCollection::TCollection(StreamableInit)
{
}
