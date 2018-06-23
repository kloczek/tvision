/*
 * TProgram.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TProgram
#define Uses_TEvent
#define Uses_TScreen
#define Uses_TStatusLine
#define Uses_TMenu
#define Uses_TGroup
#define Uses_TDeskTop
#define Uses_TEventQueue
#define Uses_TMenuBar
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDialog
#ifndef __UNPATCHED
#define Uses_TVMemMgr
#endif
#include <tv.h>

// Public variables

TStatusLine * TProgram::statusLine = 0;
TMenuBar * TProgram::menuBar = 0;
TDeskTop * TProgram::deskTop = 0;
TProgram * TProgram::application = 0;
int TProgram::appPalette = apColor;
TEvent TProgram::pending;

extern TPoint shadowSize;

TProgInit::TProgInit( TStatusLine *(*cStatusLine)( TRect ),
                            TMenuBar *(*cMenuBar)( TRect ),
                            TDeskTop *(*cDeskTop )( TRect )
                          ) :
    createStatusLine( cStatusLine ),
    createMenuBar( cMenuBar ),
    createDeskTop( cDeskTop )
{
}


TProgram::TProgram() :
    TProgInit( &TProgram::initStatusLine,
                  &TProgram::initMenuBar,
                  &TProgram::initDeskTop
                ),
    TGroup( TRect( 0,0,TScreen::screenWidth,TScreen::screenHeight ) )
{
    application = this;
    initScreen();
    state = sfVisible | sfSelected | sfFocused | sfModal | sfExposed;
    options = 0;
    buffer = TScreen::screenBuffer;

    if( createDeskTop != 0 &&
        (deskTop = createDeskTop( getExtent() )) != 0
      )
        insert(deskTop);

    if( createStatusLine != 0 &&
        (statusLine = createStatusLine( getExtent() )) != 0
      )
        insert(statusLine);

    if( createMenuBar != 0 &&
        (menuBar = createMenuBar( getExtent() )) != 0
      )
        insert(menuBar);

}

TProgram::~TProgram()
{
    application = 0;
}

void TProgram::shutDown()
{
    statusLine = 0;
    menuBar = 0;
    deskTop = 0;
    TGroup::shutDown();
#ifndef __UNPATCHED
    TVMemMgr::clearSafetyPool();    // Release the safety pool buffer.
#endif
}

Boolean TProgram::canMoveFocus()
{
    return deskTop->valid(cmReleasedFocus);
}

ushort TProgram::executeDialog( TDialog* pD, void* data )
{
    ushort c=cmCancel;

    if (validView(pD))
        {
        if (data)
        pD->setData(data);
        c = deskTop->execView(pD);
        if ((c != cmCancel) && (data))
            pD->getData(data);
        destroy(pD);
        }

    return c;
}

/*
 * This patch works around a "GOT relocation burb" under FreeBSD.  The actual
 * bug is in gcc-2.7.2.1, but there is no easy fix for it there.
 * Patch from: John Polstra <jdp@polstra.com>
 * Date: Wed, 28 May 1997 22:08:59 -0700
 */
#ifdef __FreeBSD__
static Boolean hasMouse( TView *p, void *s )
#else
inline Boolean hasMouse( TView *p, void *s )
#endif
{
    return Boolean( (p->state & sfVisible) != 0 &&
                     p->mouseInView( ((TEvent *)s)->mouse.where ));
}

void TProgram::getEvent(TEvent& event)
{
    if( pending.what != evNothing )
        {
        event = pending;
        pending.what = evNothing;
        }
    else
        {
		/* SS: changed */

		TScreen::getEvent(event);
		if (event.what == evCommand) switch (event.message.command)
		{
		case cmSysRepaint:
			/*
			 * This command redraws the screen.  Useful when the
			 * user restarts the process after a ctrl-z.
			 */
			redraw();
			clearEvent(event);
			break;
		case cmSysResize:
			/*
			 * Generated after a SIGWINCH signal.
			 */
			buffer = TScreen::screenBuffer;
			changeBounds(TRect(0, 0, TScreen::screenWidth,
				TScreen::screenHeight));
			setState(sfExposed, False);
			setState(sfExposed, True);
			redraw();
			clearEvent(event);
			break;
		case cmSysWakeup:
			idle();
			clearEvent(event);
		}
        }

    if( statusLine != 0 )
        {
        if( (event.what & evKeyDown) != 0 ||
            ( (event.what & evMouseDown) != 0 &&
              firstThat( hasMouse, &event ) == statusLine
            )
          )
            statusLine->handleEvent( event );
        }
}

TPalette& TProgram::getPalette() const
{
    static TPalette color ( cpAppColor, sizeof( cpAppColor )-1 );
    static TPalette blackwhite(cpAppBlackWhite, sizeof( cpAppBlackWhite )-1 );
    static TPalette monochrome(cpAppMonochrome, sizeof( cpAppMonochrome )-1 );
    static TPalette *palettes[] =
        {
        &color,
        &blackwhite,
        &monochrome
        };
    return *(palettes[appPalette]);
}

void TProgram::handleEvent( TEvent& event )
{
    if( event.what == evKeyDown )
        {
        char c = getAltChar( event.keyDown.keyCode );
        if( c >= '1' && c <= '9' )
            {
#ifndef __UNPATCHED
            if(canMoveFocus())      // Check valid first.
            {
                if( message(deskTop, evBroadcast, cmSelectWindowNum,
                  (void *)(ptrdiff_t)(c - '0')) != 0 )
                    clearEvent( event );
            }
            else
                clearEvent(event);
#else
            if( message( deskTop,
                         evBroadcast,
                         cmSelectWindowNum,
                         (void *)(c - '0')
                       ) != 0 )
                clearEvent( event );
#endif
            }
        }
    TGroup::handleEvent( event );
    if( event.what == evCommand && event.message.command == cmQuit )
        {
        endModal( cmQuit );
        clearEvent( event );
        }
}

void TProgram::idle()
{
    if( statusLine != 0 )
        statusLine->update();

    if( commandSetChanged == True )
        {
        message( this, evBroadcast, cmCommandSetChanged, 0 );
        commandSetChanged = False;
        }
}

TDeskTop *TProgram::initDeskTop( TRect r )
{
    r.a.y++;
    r.b.y--;
    return new TDeskTop( r );
}

TMenuBar *TProgram::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;
    return new TMenuBar( r, (TMenu *)0 );
}

void TProgram::initScreen()
{
    if( (TScreen::screenMode & 0x00FF) != TDisplay::smMono )
        {
        if( (TScreen::screenMode & TDisplay::smFont8x8) != 0 )
            shadowSize.x = 1;
        else
            shadowSize.x = 2;
        shadowSize.y = 1;
        showMarkers = False;
        if( (TScreen::screenMode & 0x00FF) == TDisplay::smBW80 )
            appPalette = apBlackWhite;
        else
            appPalette = apColor;
        }
    else
        {

        shadowSize.x = 0;
        shadowSize.y = 0;
        showMarkers = True;
        appPalette = apMonochrome;
        }
}

TStatusLine *TProgram::initStatusLine( TRect r )
{
    r.a.y = r.b.y - 1;
    return new TStatusLine( r,
        *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( exitText, kbAltX, cmQuit ) +
            *new TStatusItem( 0, kbF10, cmMenu ) +
            *new TStatusItem( 0, kbAltF3, cmClose ) +
            *new TStatusItem( 0, kbF5, cmZoom ) +
            *new TStatusItem( 0, kbCtrlF5, cmResize )
            );
}

TWindow* TProgram::insertWindow(TWindow* pWin)
{
    if (validView(pWin))
        {
        if (canMoveFocus())
            {
            deskTop->insert(pWin);
            return pWin;
            }
        else
            destroy(pWin);
        }

   return NULL;
}


void TProgram::outOfMemory()
{
}

void TProgram::putEvent( TEvent & event )
{
    pending = event;
}

void TProgram::run()
{
    execute();
}

void TProgram::setScreenMode( ushort )
{
    /* SS: changed */

    TRect r = TRect( 0, 0, TScreen::screenWidth, TScreen::screenHeight );
    changeBounds( r );
    setState(sfExposed, False);
    setState(sfExposed, True);
    redraw();
}

TView* TProgram::validView(TView* p)
{
    if( p == 0 )
        return 0;
    if( lowMemory() )
        {
        destroy( p );
        outOfMemory();
        return 0;
        }
    if( !p->valid( cmValid ) )
        {
        destroy( p );
        return 0;
        }
    return p;
}
