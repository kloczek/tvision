/*
 * histlist.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#include <tv.h>

#include <stdlib.h>
#include <string.h>

class HistRec
{

public:

    HistRec( uchar nId, const char *nStr );

    void *operator new( size_t );
    void *operator new( size_t, HistRec * );

    uchar id;
    uchar len;
    char str[1];

};

void *HistRec::operator new( size_t, HistRec *hr )
{
    return hr;
}

void *HistRec::operator new( size_t )
{
    abort();
    return nullptr;
}

inline HistRec::HistRec( uchar nId, const char *nStr ) :
    id( nId ),
    len( strlen( nStr ) + 3 )
{
    strcpy( str, nStr );
}


inline HistRec *advance( HistRec *ptr, size_t s )
{
    return (HistRec *)((char *)ptr + s);
}

inline HistRec *backup( HistRec *ptr, size_t s )
{
    return (HistRec *)((char *)ptr - s);
}

inline HistRec *next( HistRec *ptr )
{
    return advance( ptr, ptr->len );
}

inline HistRec *prev( HistRec *ptr )
{
    return backup( ptr, ptr->len );
}

/**
 * Specifies the size of the history block used by the history list manager
 * to store values entered into input lines. The size is fixed by
 * initHistory() at program startup.
 *
 * The default size of the block is 1K, but may be changed before
 * initHistory() is called. The value should not be changed after the call to
 * initHistory().
 * @see initHistory
 */
ushort historySize = 1024;  // initial size of history block

static uchar curId;
static HistRec *curRec;

/**
 * Points to a buffer called the history block used to store history strings.
 * The size of the block is defined by historySize. The pointer is 0 until set
 * by the THistory constructor, and its value should not be altered.
 * @see THistory
 */
static HistRec *historyBlock;

static HistRec *lastRec;

void advanceStringPointer()
{
    curRec = next( curRec );
    while( curRec < lastRec && curRec->id != curId )
        curRec = next( curRec );
    if( curRec >= lastRec )
        curRec = nullptr;
}

void deleteString()
{
	size_t len = curRec->len;

#ifndef __UNPATCHED
    // BUG FIX - EFW - Mon 10/30/95
    // This insures that if n = lastRec, no bytes are copied and
    // a GPF is prevented.
    HistRec *n = next(curRec);
    memcpy(curRec, n, size_t((char *)lastRec - (char *)n));
#else
	memcpy(curRec, next(curRec),    size_t( (char *)lastRec - (char *)curRec ) );
#endif
    lastRec = backup( lastRec, len );
}

void insertString( uchar id, const char *str )
{
    ushort len = strlen( str ) + 3;
    while( len > historySize - ( (char *)lastRec - (char *)historyBlock ) )
        {
        ushort firstLen = historyBlock->len;
        HistRec *dst = historyBlock;
        HistRec *src = next( historyBlock );
		memcpy( dst, src,  size_t( (char *)lastRec - (char *)src ) );
        lastRec = backup( lastRec, firstLen );
        }
    new( lastRec ) HistRec( id, str );
    lastRec = next( lastRec );
}

void startId( uchar id )
{
    curId = id;
    curRec = historyBlock;
}

ushort historyCount( uchar id )
{
    startId( id );
    ushort count =  0;
    advanceStringPointer();
    while( curRec != nullptr )
        {
        count++;
        advanceStringPointer();
        }
    return count;
}

void historyAdd( uchar id, const char *str )
{
    if( str[0] == EOS )
        return;
    startId( id );
    advanceStringPointer();
    while( curRec != nullptr )
        {
        if( strcmp( str, curRec->str ) == 0 )
            deleteString();
        advanceStringPointer();
        }
    insertString( id, str );
}

const char *historyStr( uchar id, int index )
{
    startId( id );
    for( short i = 0; i <= index; i++ )
        advanceStringPointer();
    if( curRec != nullptr )
        return curRec->str;
    else
        return nullptr;
}

void clearHistory()
{
    new (historyBlock) HistRec( 0, "" );
    lastRec = next( historyBlock );
}

/**
 * Called by TApplication constructor to allocate a block of memory on the
 * heap for use by the history list manager. The size of the block is
 * determined by the historySize variable. After initHistory() is called, the
 * historyBlock variable points to the beginning of the block.
 * @see TApplication
 */
void initHistory()
{
    historyBlock = (HistRec *) new char[historySize];
    clearHistory();
}

void doneHistory()
{
    delete historyBlock;
}
