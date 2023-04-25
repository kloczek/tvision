/*
 * TDeskTop.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TDeskTop
#define Uses_TRect
#define Uses_TPoint
#define Uses_TEvent
#define Uses_TBackground
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <stdlib.h>

TDeskInit::TDeskInit(TBackground *(*cBackground) (TRect)):
createBackground(cBackground)
{
}

TDeskTop::TDeskTop(const TRect & bounds):TDeskInit(&TDeskTop::initBackground),
TGroup(bounds)
{
	growMode = gfGrowHiX | gfGrowHiY;
	tileColumnsFirst = False;

	if (createBackground != nullptr
	    && (background = createBackground(getExtent())) != nullptr)
		insert(background);
}

void TDeskTop::shutDown()
{
	background = nullptr;
	TGroup::shutDown();
}

inline Boolean Tileable(TView *p)
{
	return Boolean((p->options & ofTileable) != 0
		       && (p->state & sfVisible) != 0);
}

static short cascadeNum;
static TView *lastView;

void doCount(TView *p, void *)
{
	if (Tileable(p)) {
		cascadeNum++;
		lastView = p;
	}
}

void doCascade(TView *p, void *r)
{
	if (Tileable(p) && cascadeNum >= 0) {
		TRect NR = *static_cast < TRect * >(r);
		NR.a.x += cascadeNum;
		NR.a.y += cascadeNum;
		p->locate(NR);
		cascadeNum--;
	}
}

void TDeskTop::cascade(const TRect & r)
{
	TPoint min, max;
	cascadeNum = 0;
	forEach(doCount, nullptr);
	if (cascadeNum > 0) {
		lastView->sizeLimits(min, max);
		if ((min.x > r.b.x - r.a.x - cascadeNum) ||
		    (min.y > r.b.y - r.a.y - cascadeNum))
			tileError();
		else {
			cascadeNum--;
			lock();
			forEach(doCascade, (void *)&r);
			unlock();
		}
	}
}

void TDeskTop::handleEvent(TEvent & event)
{
	TGroup::handleEvent(event);
	if (event.what == evCommand) {
		switch (event.message.command) {
		case cmNext:
#ifndef __UNPATCHED
			if (valid(cmReleasedFocus))	// <-- Check valid.
				selectNext(False);
#else
			selectNext(False);
#endif
			break;
		case cmPrev:
#ifndef __UNPATCHED
			if (valid(cmReleasedFocus))	// <-- Check valid.
				current->putInFrontOf(background);
#else
			current->putInFrontOf(background);
#endif
			break;
		default:
			return;
		}
		clearEvent(event);
	}
}

TBackground *TDeskTop::initBackground(TRect r)
{
	return new TBackground(r, defaultBkgrnd);
}

short iSqr(short i)
{
	short res1 = 2;
	short res2 = i / res1;
	while (abs(static_cast < int >(res1 - res2)) > 1) {
		res1 = (res1 + res2) / 2;
		res2 = i / res1;
	}
	return res1 < res2 ? res1 : res2;
}

void mostEqualDivisors(short n, short &x, short &y, Boolean favorY)
{
	short i;

	i = iSqr(n);
	if (n % i != 0)
		if (n % (i + 1) == 0)
			i++;
	if (i < (n / i))
		i = n / i;

	if (favorY) {
		x = n / i;
		y = i;
	} else {
		y = n / i;
		x = i;
	}
}

static short numCols, numRows, numTileable, leftOver, tileNum;

void doCountTileable(TView *p, void *)
{
	if (Tileable(p))
		numTileable++;
}

int dividerLoc(int lo, int hi, int num, int pos)
{
	return int (long (hi - lo) * pos / long (num) + lo);
}

TRect calcTileRect(short pos, const TRect & r)
{
	short x, y;
	TRect nRect;

	short d = (numCols - leftOver) * numRows;
	if (pos < d) {
		x = pos / numRows;
		y = pos % numRows;
	} else {
		x = (pos - d) / (numRows + 1) + (numCols - leftOver);
		y = (pos - d) % (numRows + 1);
	}
	nRect.a.x = dividerLoc(r.a.x, r.b.x, numCols, x);
	nRect.b.x = dividerLoc(r.a.x, r.b.x, numCols, x + 1);
	if (pos >= d) {
		nRect.a.y = dividerLoc(r.a.y, r.b.y, numRows + 1, y);
		nRect.b.y = dividerLoc(r.a.y, r.b.y, numRows + 1, y + 1);
	} else {
		nRect.a.y = dividerLoc(r.a.y, r.b.y, numRows, y);
		nRect.b.y = dividerLoc(r.a.y, r.b.y, numRows, y + 1);
	}
	return nRect;
}

void doTile(TView *p, void *lR)
{
	if (Tileable(p)) {
		TRect r =
		    calcTileRect(tileNum, *static_cast < const TRect * >(lR));
		p->locate(r);
		tileNum--;
	}
}

void TDeskTop::tile(const TRect & r)
{
	numTileable = 0;
	forEach(doCountTileable, nullptr);
	if (numTileable > 0) {
		mostEqualDivisors(numTileable, numCols, numRows,
				  Boolean(!tileColumnsFirst));
		if (((r.b.x - r.a.x) / numCols == 0)
		    || ((r.b.y - r.a.y) / numRows == 0))
			tileError();
		else {
			leftOver = numTileable % numCols;
			tileNum = numTileable - 1;
			lock();
			forEach(doTile, (void *)&r);
			unlock();
		}
	}
}

void TDeskTop::tileError()
{
}

#if !defined(NO_STREAMABLE)

TStreamable *TDeskTop::build()
{
	return new TDeskTop(streamableInit);
}

TDeskTop::TDeskTop(StreamableInit):
TDeskInit(nullptr /*streamableInit */ ),
    TGroup(streamableInit)
{
	tileColumnsFirst = False;
}

#endif
