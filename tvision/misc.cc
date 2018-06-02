/*
 * misc.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 *
 * Little files are merged here.
 */

#define Uses_TGroup
#define Uses_TEvent
#define Uses_TObject
#define Uses_TVMemMgr
#define Uses_TView
#define Uses_pstream
#include <tv.h>

#include <string.h>

/* from MISC.CPP */

void *message( TView *receiver, ushort what, ushort command, void *infoPtr)
{
    if( receiver == 0 )
        return 0;

    TEvent event;
    event.what = what;
    event.message.command = command;
    event.message.infoPtr = infoPtr;
    receiver->handleEvent( event );
    if( event.what == evNothing )
        return event.message.infoPtr;
    else
        return 0;
}

Boolean lowMemory()
{
    return Boolean(TVMemMgr::safetyPoolExhausted());
}

/* from NEWSTR.CPP */

char *newStr( const char *s )
{
    if( s == 0 )
        return 0;
    char *temp = new char[ strlen(s)+1 ];
    strcpy( temp, s );
    return temp;
}

/* from GRP.CPP */

TView *TGroup::at( short index )
{
    TView *temp = last;
    while( index-- > 0 )
        temp = temp->next;
    return temp;
}

TView *TGroup::firstThat( Boolean (*func)(TView *, void *), void *args )
{
    TView *temp = last;
    if( temp == 0 )
        return 0;

    do  {
        temp = temp->next;
        if( func( temp, args ) == True )
            return temp;
        } while( temp != last );
    return 0;
}

void TGroup::forEach( void (*func)(TView*, void *), void *args )
{
    TView *term = last;
    TView *temp = last;
    if( temp == 0 )
        return;

    TView *next = temp->next;
    do  {
        temp = next;
        next = temp->next;
        func( temp, args );
        } while( temp != term );

}

short TGroup::indexOf( TView *p )
{
    if( last == 0 )
        return 0;

    short index = 0;
    TView *temp = last;
    do  {
        index++;
        temp = temp->next;
        } while( temp != p && temp != last );
    if( temp != p )
        return 0;
    else
        return index;
}

/* from MAPCOLOR.CPP */

uchar TView::mapColor( uchar color )
{
    if( color == 0 )
        return errorAttr;
    TView *cur = this;
    do  {
        TPalette& p = cur->getPalette();
        if( p[0] != 0 )
            {
            if( color > p[0] )
                return errorAttr;
            color = p[color];
            if( color == 0 )
                return errorAttr;
            }
        cur = cur->owner;
        } while( cur != 0 );
    return color;
}

/* from STRMSTAT.CPP */

TStreamableTypes * pstream::types;
