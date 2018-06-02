/*
 * TFileCollection.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TFileCollection
#define Uses_TSearchRec
#include <tv.h>

#include <string.h>

inline const char *getName( void *k )
{
    return ((TSearchRec *)k)->name;
}

inline int attr( void *k )
{
    return ((TSearchRec *)k)->attr;
}

int TFileCollection::compare(void *key1, void *key2)
{
    if( strcmp( getName( key1 ), getName( key2 ) ) == 0 )
        return 0;

    if( strcmp( getName( key1 ), ".." ) == 0 )
        return 1;
    if( strcmp( getName( key2 ), ".." ) == 0 )
        return -1;

    if( (attr( key1 ) & FA_DIREC) != 0 && (attr( key2 ) & FA_DIREC) == 0 )
        return 1;
    if( (attr( key2 ) & FA_DIREC) != 0 && (attr( key1 ) & FA_DIREC) == 0 )
        return -1;

    return strcmp( getName( key1 ), getName( key2 ) );
}


TStreamable *TFileCollection::build()
{
    return new TFileCollection( streamableInit );
}

void TFileCollection::writeItem( void *obj, opstream& os )
{
    TSearchRec *item = (TSearchRec *)obj;
    os << item->attr << item->time << item->size;
    os.writeString( item->name );
}

void *TFileCollection::readItem( ipstream& is )
{
    TSearchRec *item = new TSearchRec;
    is >> item->attr >> item->time >> item->size;
    is.readString( item->name, sizeof(item->name) );
    return item;
}
