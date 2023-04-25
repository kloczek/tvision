/*
 * TPoint.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TPoint
#include <tv.h>

TPoint operator -(const TPoint & one, const TPoint & two)
{
	TPoint result;
	result.x = one.x - two.x;
	result.y = one.y - two.y;
	return result;
}

TPoint operator +(const TPoint & one, const TPoint & two)
{
	TPoint result;
	result.x = one.x + two.x;
	result.y = one.y + two.y;
	return result;
}

int operator ==(const TPoint & one, const TPoint & two)
{
	return one.x == two.x && one.y == two.y;
}

int operator!=(const TPoint & one, const TPoint & two)
{
	return one.x != two.x || one.y != two.y;
}
