/*
 * TLabel.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TLabel
#define Uses_TEvent
#define Uses_TDrawBuffer
#define Uses_TGroup
#define Uses_TView
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <ctype.h>

#define cpLabel "\x07\x08\x09\x09"

TLabel::TLabel( const TRect& bounds, const char *aText, TView* aLink) :
    TStaticText( bounds, aText ),
    link( aLink ),
    light( False )
{
    options |= ofPreProcess | ofPostProcess;
    eventMask |= evBroadcast;
}

void TLabel::shutDown()
{
    link = nullptr;
    TStaticText::shutDown();
}

void TLabel::draw()
{
    ushort color;
    TDrawBuffer b;
    uchar scOff;

    if( light )
        {
        color = getColor(0x0402);
        scOff = 0;
        }
    else
        {
        color = getColor(0x0301);
        scOff = 4;
        }

    b.moveChar( 0, ' ', color, size.x );
    if( text != nullptr )
        b.moveCStr( 1, text, color );
    if( showMarkers )
        b.putChar( 0, specialChars[scOff] );
    writeLine( 0, 0, size.x, 1, b );
}

TPalette& TLabel::getPalette() const
{
    static TPalette palette( cpLabel, sizeof( cpLabel )-1 );
    return palette;
}

void TLabel::focusLink(TEvent& event)
{
	if (link && (link->options & ofSelectable))
		link->focus();
	clearEvent(event);
}

void TLabel::handleEvent( TEvent& event )
{
    TStaticText::handleEvent(event);
    if( event.what == evMouseDown )
    	focusLink(event);

    else if( event.what == evKeyDown )
        {
        char c = hotKey( text );
        if( getAltCode(c) == event.keyDown.keyCode ||
                ( c != 0 && owner->phase == TGroup::phPostProcess &&
                toupper(event.keyDown.charScan.charCode) ==  c )
          )
	    focusLink(event);
        }
    else if( event.what == evBroadcast && link &&
            ( event.message.command == cmReceivedFocus ||
              event.message.command == cmReleasedFocus )
           )
            {
            light = Boolean( (link->state & sfFocused) != 0 );
            drawView();
            }
}

#if !defined(NO_STREAMABLE)

void TLabel::write( opstream& os )
{
    TStaticText::write( os );
    os << link;
}

void *TLabel::read( ipstream& is )
{
    TStaticText::read( is );
    is >> link;
    light = False;
    return this;
}

TStreamable *TLabel::build()
{
    return new TLabel( streamableInit );
}

TLabel::TLabel( StreamableInit ) : TStaticText( streamableInit )
{
}


#endif
