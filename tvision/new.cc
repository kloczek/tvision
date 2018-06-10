/*
 * new.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define NDEBUG
#include <assert.h>
#include <stdlib.h>

#define Uses_TVMemMgr
#include <tv.h>

TBufListEntry * TBufListEntry::bufList = 0;

TBufListEntry::TBufListEntry( void*& o ) : owner( o )
{
    next = bufList;
    prev = 0;
    bufList = this;
    if( next != 0 )
        next->prev = this;
}

TBufListEntry::~TBufListEntry()
{
    owner = 0;
    if( prev == 0 )
        bufList = next;
    else
        prev->next = next;
    if( next != 0 )
        next->prev = prev;
}

void *TBufListEntry::operator new( size_t sz, size_t extra )
{
    return malloc( sz + extra*sizeof( unsigned ) );
}

void *TBufListEntry::operator new( size_t ) throw()
{
    return NULL;
}

void TBufListEntry::operator delete( void *b )
{
    free( b );
}

Boolean TBufListEntry::freeHead()
{
    if( bufList == 0 )
        return False;
    else
        {
        delete bufList;
        return True;
        }
}

void * TVMemMgr::safetyPool = 0;
size_t TVMemMgr::safetyPoolSize = 0;
int TVMemMgr::inited = 0;

TVMemMgr memMgr;

TVMemMgr::TVMemMgr()
{
    if( !inited )
        resizeSafetyPool();
};

void TVMemMgr::resizeSafetyPool( size_t sz )
{
    inited = 1;
    free( safetyPool );
    if( sz == 0 )
        safetyPool = 0;
    else
        safetyPool = malloc( sz );
    safetyPoolSize = sz;
}

int TVMemMgr::safetyPoolExhausted()
{
    return inited && (safetyPool == 0);
}

void TVMemMgr::allocateDiscardable( void *&adr, size_t sz )
{
    if( safetyPoolExhausted() )
        adr = 0;
    else
        {
        TBufListEntry *newEntry = new( sz ) TBufListEntry( adr );
        if( newEntry == 0 )
            adr = 0;
        else
            adr = (char *)newEntry + sizeof(TBufListEntry);
        }
}

void TVMemMgr::freeDiscardable( void *block )
{
    delete (TBufListEntry *)((char *)block - sizeof(TBufListEntry));
}

#ifndef __UNPATCHED
void TVMemMgr::clearSafetyPool()
{
    resizeSafetyPool( 0 );
    inited = 0;
}

void TVMemMgr::suspend()
{
    while(TBufListEntry::freeHead())
	;
}
#endif

#if !defined( NDEBUG )
const int BLK_SIZE = 16;
const int BLK_DATA = 0xA6;
#else
const int BLK_SIZE = 0;
#endif

/* SS: this makes g++ happy */

void * allocBlock( size_t sz )
{
    assert( heapcheck() >= 0 );

    sz += BLK_SIZE;
    if( sz == 0 )
        sz = 1;

    void *temp = malloc( sz );
    while( temp == 0 && TBufListEntry::freeHead() == True )
        temp = malloc( sz );
    if( temp == 0 )
	{
        if( TVMemMgr::safetyPoolExhausted() )
            abort();
        else
            {
            TVMemMgr::resizeSafetyPool( 0 );
            temp = malloc( sz );
            if( temp == 0 )
                abort();
            }
        }
#if !defined( NDEBUG )
    memset( temp, BLK_DATA, BLK_SIZE );
#endif
    return (char *)temp + BLK_SIZE;
}

void * operator new[] ( size_t sz )
{
   return allocBlock(sz);
}

void * operator new ( size_t sz )
{
   return allocBlock(sz);
}

#if !defined( NDEBUG )
static void check( void *blk )
{
    for( int i = 0; i < BLK_SIZE; i++ )
        assert( *((unsigned char *)blk + i) == BLK_DATA );
}
#endif

static void deleteBlock( void *blk )
{
    assert( heapcheck() >= 0 );
    if( blk == 0 )
        return;
    void *tmp = (char *)blk - BLK_SIZE;
#if !defined( NDEBUG )
    check( tmp );
#endif
    free( tmp );
    if( TVMemMgr::safetyPoolExhausted() )
        TVMemMgr::resizeSafetyPool();
}

void operator delete ( void *blk )
{
   deleteBlock(blk);
}

void operator delete[] ( void *blk )
{
   deleteBlock(blk);
}
