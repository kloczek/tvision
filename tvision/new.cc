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

TBufListEntry * TBufListEntry::bufList = nullptr;

TBufListEntry::TBufListEntry( void*& o ) : owner( o )
{
    next = bufList;
    prev = nullptr;
    bufList = this;
    if( next != nullptr )
        next->prev = this;
}

TBufListEntry::~TBufListEntry()
{
    owner = nullptr;
    if( prev == nullptr )
        bufList = next;
    else
        prev->next = next;
    if( next != nullptr )
        next->prev = prev;
}

void *TBufListEntry::operator new( size_t sz, size_t extra )
{
    return malloc( sz + extra*sizeof( unsigned ) );
}

void *TBufListEntry::operator new( size_t )
{
    return nullptr;
}

void TBufListEntry::operator delete( void *b )
{
    free( b );
}

Boolean TBufListEntry::freeHead()
{
    if( bufList == nullptr )
        return False;
    else
        {
        delete bufList;
        return True;
        }
}

void * TVMemMgr::safetyPool = nullptr;
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
        safetyPool = nullptr;
    else
        safetyPool = malloc( sz );
    safetyPoolSize = sz;
}

int TVMemMgr::safetyPoolExhausted()
{
    return inited && (safetyPool == nullptr);
}

void TVMemMgr::allocateDiscardable( void *&adr, size_t sz )
{
    if( safetyPoolExhausted() )
        adr = nullptr;
    else
        {
        TBufListEntry *newEntry = new( sz ) TBufListEntry( adr );
        if( newEntry == nullptr )
            adr = nullptr;
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
    while( temp == nullptr && TBufListEntry::freeHead() == True )
        temp = malloc( sz );
    if( temp == nullptr )
	{
        if( TVMemMgr::safetyPoolExhausted() )
            abort();
        else
            {
            TVMemMgr::resizeSafetyPool( 0 );
            temp = malloc( sz );
            if( temp == nullptr )
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
    if( blk == nullptr )
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
