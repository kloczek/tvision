/*
 * TApplication.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TEventQueue
#define Uses_TScreen
#define Uses_TObject
#define Uses_TMouse
#define Uses_TApplication
#ifndef __UNPATCHED
#define Uses_TVMemMgr
#endif
#include <tv.h>

void initHistory();
void doneHistory();

TApplication::TApplication() :
    TProgInit( &TApplication::initStatusLine,
                  &TApplication::initMenuBar,
                  &TApplication::initDeskTop
                )
{
    initHistory();
}

TApplication::~TApplication()
{
    doneHistory();
}

void TApplication::suspend()
{
    /* SS: changed */

    TScreen::suspend();
#ifndef __UNPATCHED
    TVMemMgr::suspend();       // Release discardable memory.
#endif
}

void TApplication::resume()
{
    /* SS: changed */

    TScreen::resume();
}
