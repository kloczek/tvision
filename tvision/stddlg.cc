/*
 * stddlg.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_MsgBox
#define Uses_TKeys
#define Uses_TFileInputLine
#define Uses_TEvent
#define Uses_TSortedListBox
#define Uses_TSearchRec
#define Uses_TFileInfoPane
#define Uses_TDrawBuffer
#define Uses_TFileDialog
#define Uses_TSortedCollection
#include <tv.h>

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cpInfoPane "\x1E"

TFileInputLine::TFileInputLine( const TRect& bounds, short aMaxLen ) :
    TInputLine( bounds, aMaxLen )
{
    eventMask |= evBroadcast;
}

void TFileInputLine::handleEvent( TEvent& event )
{
    TInputLine::handleEvent(event);
    if( event.what == evBroadcast &&
        event.message.command == cmFileFocused &&
        !(state & sfSelected)
      )
        {
#ifndef __UNPATCHED
    // Prevents incorrect display in the input line if wildCard has
    // already been expanded.
    if( (((TSearchRec *)event.message.infoPtr)->attr & FA_DIREC) != 0 )
    {
        strcpy( data, ((TFileDialog *)owner)->wildCard );
        if(!strchr(data, ':') && !strchr(data, '/'))
        {
            strcpy( data, ((TSearchRec *)event.message.infoPtr)->name );
            strcat( data, "/" );
            strcat( data, ((TFileDialog *)owner)->wildCard );
        }
        else
        {
            // Insert "<name>\\" between last name or wildcard and last '/'
            fexpand(data);    // Insure complete expansion to begin with
            char *tmp = strrchr(data, '/') + 1;
            char *nm = ((TSearchRec *)event.message.infoPtr)->name;
            memmove(tmp + strlen(nm) + 1, tmp, strlen(tmp) + 1);
            memcpy(tmp, nm, strlen(nm));
            *(tmp + strlen(nm)) = '/';
            fexpand(data);    // Expand again incase it was '..'.
        }
    }
#else
        if( (((TSearchRec *)event.message.infoPtr)->attr & FA_DIREC) != 0 )
            {
            strcpy( data, ((TSearchRec *)event.message.infoPtr)->name );
            strcat( data, "/" );
            strcat( data, ((TFileDialog *)owner)->wildCard );
            }
#endif
        else
            strcpy( data, ((TSearchRec *)event.message.infoPtr)->name );
        drawView();
        }
}

TSortedListBox::TSortedListBox( const TRect& bounds,
                                ushort aNumCols,
                                TScrollBar *aScrollBar) :
    TListBox(bounds, aNumCols, aScrollBar),
    shiftState( 0 ),
    searchPos( -1 )
{
    showCursor();
    setCursor(1, 0);
}

static Boolean equal( const char *s1, const char *s2, ushort count)
{
	/* SS: use a standard function */

	return Boolean( strncasecmp( s1, s2, count ) == 0 );
}

void TSortedListBox::handleEvent(TEvent& event)
{
    char curString[256], newString[256];
    void* k;
    int value;
    short oldPos, oldValue;

    oldValue = focused;
    TListBox::handleEvent( event );
    if( oldValue != focused ||
        ( event.what == evBroadcast &&
          event.message.command == cmReleasedFocus )
      )
        searchPos = -1;
    if( event.what == evKeyDown )
        {
        if( event.keyDown.charScan.charCode != 0 )
            {
            value = focused;
            if( value < range )
                getText( curString, (short) value, 255 );
            else
                *curString = EOS;
            oldPos = searchPos;
            if( event.keyDown.keyCode == kbBack )
                {
                if( searchPos == -1 )
                    return;
                searchPos--;
                if( searchPos == -1 )
                    shiftState = (ushort) event.keyDown.controlKeyState;
                curString[searchPos+1] = EOS;
                }
            else if( (event.keyDown.charScan.charCode == '.') )
                {
                char *loc = strchr( curString, '.' );
                if( loc == nullptr )
                    searchPos = -1;
                else
                    searchPos = short(loc - curString);
                }
            else
                {
                searchPos++;
                if( searchPos == 0 )
                    shiftState = (ushort) event.keyDown.controlKeyState;
                curString[searchPos] = event.keyDown.charScan.charCode;
                curString[searchPos+1] = EOS;
                }
            k = getKey(curString);

	    /* SS: this makes g++ happy */

	    ccIndex c = (ccIndex) value;
            list()->search( k, c );
		value = (int) c;

            if( value < range )
                {
                getText( newString, (short) value, 255 );
                if( equal( curString, newString, (ushort) (searchPos+1) ) )
                    {
                    if( value != oldValue )
                        {
                        focusItem( (short) value );
                        setCursor( cursor.x+searchPos+1, cursor.y );
                        }
                    else
                        setCursor(cursor.x+(searchPos-oldPos), cursor.y );
                    }
                else
                    searchPos = oldPos;
                }
            else
                searchPos = oldPos;
            if( searchPos != oldPos ||
                isalpha( event.keyDown.charScan.charCode )
              )
                clearEvent(event);
            }
        }
}

void* TSortedListBox::getKey( const char *s )
{
    return (void *)s;
}

void TSortedListBox::newList( TSortedCollection *aList )
{
    TListBox::newList( aList );
    searchPos = -1;
}

TFileInfoPane::TFileInfoPane( const TRect& bounds ) :
    TView(bounds)
{
    eventMask |= evBroadcast;
}

void TFileInfoPane::draw()
{
    Boolean PM;
    TDrawBuffer b;
    ushort  color;
    ftime *time;
    char path[PATH_MAX];

#ifndef __UNPATCHED
    // Prevents incorrect directory name display in info pane if wildCard
    // has already been expanded.
    strcpy(path, ((TFileDialog *)owner)->wildCard );
    if(!strchr(path, ':') && !strchr(path, '/'))
    {
        strcpy( path, ((TFileDialog *)owner)->directory );
        strcat( path, ((TFileDialog *)owner)->wildCard );
        fexpand( path );
    }
#else
    strcpy( path, ((TFileDialog *)owner)->directory );
    strcat( path, ((TFileDialog *)owner)->wildCard );
    fexpand( path );
#endif

    color = getColor(0x01);
    b.moveChar( 0, ' ', color, (ushort) size.x );
    b.moveStr( 1, path, color );
    writeLine( 0, 0, (ushort) size.x, 1, b );

    b.moveChar( 0, ' ', color, (ushort) size.x );
    b.moveStr( 1, file_block.name, color );

    if( *(file_block.name) != EOS )
        {

        char buf[10];

	/* SS: little fix */

	sprintf(buf, "%ld", file_block.size);
        b.moveStr( 14, buf, color );

        time = (ftime *) &file_block.time;
        b.moveStr( 25, months[time->ft_month], color );

	sprintf(buf, "%02u", time->ft_day);
        b.moveStr( 29, buf, color );

        b.putChar( 31, ',' );

	sprintf(buf, "%d", time->ft_year+1980);
        b.moveStr( 32, buf, color );

        PM = Boolean(time->ft_hour >= 12 );
        time->ft_hour %= 12;

        if( time->ft_hour == 0 )
            time->ft_hour = 12;

	sprintf(buf, "%02u", time->ft_hour);
        b.moveStr( 38, buf, color );
        b.putChar( 40, ':' );

	sprintf(buf, "%02u", time->ft_min);
        b.moveStr( 41, buf, color );

        if( PM )
            b.moveStr( 43, pmText, color );
        else
            b.moveStr( 43, amText, color );
        }

    writeLine(0, 1, (ushort) size.x, 1, b );
    b.moveChar( 0, ' ', color, (ushort) size.x );
    writeLine( 0, 2, (ushort) size.x, (ushort) (size.y-2), b);
}

TPalette& TFileInfoPane::getPalette() const
{
    static TPalette palette( cpInfoPane, sizeof( cpInfoPane )-1 );
    return palette;
}

void TFileInfoPane::handleEvent( TEvent& event )
{
    TView::handleEvent(event);
    if( event.what == evBroadcast && event.message.command == cmFileFocused )
        {
        file_block = *((TSearchRec *)(event.message.infoPtr));
        drawView();
        }
}

#if !defined(NO_STREAMABLE)

TStreamable *TFileInfoPane::build()
{
    return new TFileInfoPane( streamableInit );
}

#ifndef __UNPATCHED
TStreamable *TSortedListBox::build()
{
    return new TSortedListBox( streamableInit );
}

void *TSortedListBox::read( ipstream& is )
{
    TListBox::read(is);

    // Must initialize these or serious memory overwrite
    // problems can occur!
    searchPos = -1;
    shiftState = 0;

    return this;
}
#endif

#endif
