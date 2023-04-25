/*
 * TParamText.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TParamText
#include <tv.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

TParamText::TParamText(const TRect & bounds):TStaticText(bounds, nullptr),
str(new char[256])
{
	str[0] = EOS;
}

TParamText::~TParamText()
{
	delete str;
}

void TParamText::getText(char *s)
{
	if (str != nullptr)
		strcpy(s, str);
	else
		*s = EOS;
}

int TParamText::getTextLen()
{
	return (str != nullptr) ? strlen(str) : 0;
}

void TParamText::setText(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	drawView();
}

#if !defined(NO_STREAMABLE)

#ifndef __UNPATCHED
void TParamText::write(opstream & os)
{
	TStaticText::write(os);
	os.writeString(str);
}

void *TParamText::read(ipstream & is)
{
	TStaticText::read(is);
	str = new char[256];
	is.readString(str, 256);
	return this;
}
#endif

TStreamable *TParamText::build()
{
	return new TParamText(streamableInit);
}

TParamText::TParamText(StreamableInit):TStaticText(streamableInit)
{
}

#endif
