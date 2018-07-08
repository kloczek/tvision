/*
 * TMenuView.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TMenuItem
#define Uses_TMenu
#define Uses_TMenuView
#define Uses_TKeys
#define Uses_TRect
#define Uses_TEvent
#define Uses_TGroup
#define Uses_TMenuBox
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <assert.h>
#include <ctype.h>
#include <string.h>

#define cpMenuView "\x02\x03\x04\x05\x06\x07"

TMenuItem::TMenuItem(   const char *aName,
                        ushort aCommand,
                        ushort aKeyCode,
                        ushort aHelpCtx,
                        const char *p,
                        TMenuItem *aNext
             )
{
    name = newStr( aName );
    command = aCommand;
    disabled = Boolean(!TView::commandEnabled(command));
    keyCode = aKeyCode;
    helpCtx = aHelpCtx;
    if( p == nullptr )
        param = nullptr;
    else
        param = newStr( p );
    next = aNext;
}

TMenuItem::TMenuItem( const char *aName,
                      ushort aKeyCode,
                      TMenu *aSubMenu,
                      ushort aHelpCtx,
                      TMenuItem *aNext
                    )
{
    name = newStr( aName );
    command = 0;
    disabled = Boolean(!TView::commandEnabled(command));
    keyCode = aKeyCode;
    helpCtx = aHelpCtx;
    subMenu = aSubMenu;
    next = aNext;
}

TMenuItem::~TMenuItem()
{
    delete (char *)name;
    if( command == 0 )
        delete subMenu;
    else
        delete (char *)param;
}

TMenu::~TMenu()
{
    while( items != nullptr )
        {
        TMenuItem *temp = items;
        items = items->next;
        delete temp;
        }
}

void TMenuView::trackMouse( TEvent& e, Boolean& mouseActive )
{
    TPoint mouse = makeLocal( e.mouse.where );
    for( current = menu->items; current != nullptr; current = current->next )
        {
        TRect r = getItemRect( current );
        if( r.contains(mouse) )
        {
        mouseActive = True;
            return;
        }
        }
}

void TMenuView::nextItem()
{
    if( (current = current->next) == nullptr )
        current = menu->items;
}

void TMenuView::prevItem()
{
    TMenuItem *p;

    if( (p = current) == menu->items)
        p = nullptr;

    do  {
        nextItem();
        } while( current->next != p );
}

void TMenuView::trackKey( Boolean findNext )
{
    if( current == nullptr )
        return;

    do  {
        if( findNext )
            nextItem();
        else
            prevItem();
        } while( current->name == nullptr );
}

Boolean TMenuView::mouseInOwner( TEvent& e )
{
    if( parentMenu == nullptr || parentMenu->size.y != 1 )
        return False;
    else
        {
        TPoint mouse = parentMenu->makeLocal( e.mouse.where );
        TRect r = parentMenu->getItemRect( parentMenu->current );
        return r.contains( mouse );
        }
}

Boolean TMenuView::mouseInMenus( TEvent& e )
{
    TMenuView *p =  parentMenu;
    while( p != nullptr && !p->mouseInView(e.mouse.where) )
        p = p->parentMenu;

    return Boolean( p != nullptr );
}

TMenuView *TMenuView::topMenu()
{
    TMenuView *p = this;
    while( p->parentMenu != nullptr )
        p = p->parentMenu;
    return p;
}

enum menuAction { doNothing, doSelect, doReturn };

ushort TMenuView::execute()
{
    Boolean    autoSelect = False;
    menuAction action;
    char   ch;
    ushort result = 0;
    TMenuItem *itemShown = nullptr;
    TMenuItem *p;
    TMenuView *target;
    TRect  r;
    TEvent e;
    Boolean mouseActive;

    current = menu->deflt;
    mouseActive = False;
    do  {
        action = doNothing;
        getEvent(e);
        switch (e.what)
            {
            case  evMouseDown:
                if( mouseInView(e.mouse.where) || mouseInOwner(e) )
                    {
                    trackMouse(e, mouseActive);
                    if( size.y == 1 )
                        autoSelect = True;
                    }
                else
                    action =  doReturn;
                break;
            case  evMouseUp:
                trackMouse(e, mouseActive);
                if( mouseInOwner(e) )
                    current = menu->deflt;
                else if( current != nullptr && current->name != nullptr )
                    action = doSelect;
                else if (mouseActive)
                    action = doReturn;
        else
            {
            current = menu->deflt;
            if (current == nullptr)
                current = menu->items;
            action = doNothing;
            }
                break;
            case  evMouseMove:
                if( e.mouse.buttons != 0 )
                    {
                    trackMouse(e, mouseActive);
                    if( !(mouseInView(e.mouse.where) || mouseInOwner(e)) &&
                        mouseInMenus(e) )
                        action = doReturn;
                    }
                break;
            case  evKeyDown:
                switch( ctrlToArrow(e.keyDown.keyCode) )
                    {
                    case  kbUp:
                    case  kbDown:
                        if( size.y != 1 )
                            trackKey(Boolean(ctrlToArrow(e.keyDown.keyCode) == kbDown));
                        else if( e.keyDown.keyCode == kbDown )
                            autoSelect =  True;
                        break;
                    case  kbLeft:
                    case  kbRight:
                        if( parentMenu == nullptr )
                            trackKey(Boolean(ctrlToArrow(e.keyDown.keyCode) == kbRight));
                        else
                            action =  doReturn;
                        break;
                    case  kbHome:
                    case  kbEnd:
                        if( size.y != 1 )
                            {
                            current = menu->items;
                            if( e.keyDown.keyCode == kbEnd )
                                trackKey(False);
                            }
                        break;
                    case  kbEnter:
                        if( size.y == 1 )
                            autoSelect =  True;
                        action = doSelect;
                        break;
                    case  kbEsc:
                        action = doReturn;
                        if( parentMenu == nullptr || parentMenu->size.y != 1 )
                            clearEvent(e);
                        break;
                    default:
                        target = this;
                        ch = getAltChar(e.keyDown.keyCode);
                        if( ch == 0 )
                            ch = e.keyDown.charScan.charCode;
                        else
                            target = topMenu();
                        p = target->findItem(ch);
                        if( p == nullptr )
                            {
                            p = topMenu()->hotKey(e.keyDown.keyCode);
                            if( p != nullptr && commandEnabled(p->command) )
                                {
                                result = p->command;
                                action = doReturn;
                                }
                            }
                        else if( target == this )
                            {
                            if( size.y == 1 )
                                autoSelect = True;
                            action = doSelect;
                            current = p;
                            }
                        else if( parentMenu != target ||
                                 parentMenu->current != p )
                                action = doReturn;
                    }
                break;
            case  evCommand:
                if( e.message.command == cmMenu )
                    {
                    autoSelect = False;
                    if (parentMenu != nullptr )
                        action = doReturn;
                    }
                else
                    action = doReturn;
                break;
            }

        if( itemShown != current )
            {
            itemShown =  current;
            drawView();
            }

        if( (action == doSelect || (action == doNothing && autoSelect)) &&
            current != nullptr &&
            current->name != nullptr )
            {
                if( current->command == 0 )
                    {
                    if( (e.what & (evMouseDown | evMouseMove)) != 0 )
                        putEvent(e);
                    r = getItemRect( current );
                    r.a.x = r.a.x + origin.x;
                    r.a.y = r.b.y + origin.y;
                    r.b = owner->size;
                    if( size.y == 1 )
                        r.a.x--;
                    target = topMenu()->newSubView(r, current->subMenu,this);
                    result = owner->execView(target);
                    destroy( target );
                    }
                else if( action == doSelect )
                    result = current->command;
            }

        if( result != 0 && commandEnabled(result) )
            {
            action =  doReturn;
            clearEvent(e);
            }
        else
            result = 0;
        } while( action != doReturn );

    if( e.what != evNothing &&
        (parentMenu != nullptr || e.what == evCommand))
            putEvent(e);
    if( current != nullptr )
        {
        menu->deflt = current;
        current = nullptr;
        drawView();
        }
    return result;
}

TMenuItem *TMenuView::findItem( char ch )
{
    ch = toupper(ch);
    TMenuItem *p = menu->items;
    while( p != nullptr )
        {
        if( p->name != nullptr && !p->disabled )
            {
            char *loc = strchr( (char *) p->name, '~' );
            if( loc != nullptr && (uchar)ch == toupper( loc[1] ) )
                return p;
            }
        p =  p->next;
        }
    return nullptr;
}

TRect TMenuView::getItemRect( TMenuItem * )
{
    return TRect( 0, 0, 0, 0 );
}

ushort TMenuView::getHelpCtx()
{
    TMenuView *c = this;

    while( c != nullptr &&
                (c->current == nullptr ||
                 c->current->helpCtx == hcNoContext ||
                 c->current->name == nullptr )
         )
        c = c->parentMenu;

    if( c != nullptr )
        return c->current->helpCtx;
    else
        return hcNoContext;
}

TPalette& TMenuView::getPalette() const
{
    static TPalette palette( cpMenuView, sizeof( cpMenuView )-1 );
    return palette;
}

Boolean TMenuView::updateMenu( TMenu *menu )
{
    Boolean res = False;
    if( menu != nullptr )
        {
        for( TMenuItem *p = menu->items; p != nullptr; p = p->next )
            {
            if( p->name != nullptr )
        	{
                if( p->command == 0 )
                    {
                    if( p->subMenu && updateMenu(p->subMenu) == True )
                        res = True;
                    }
                else
                    {
                    Boolean commandState = commandEnabled(p->command);
                    if( p->disabled == commandState )
                        {
                        p->disabled = Boolean(!commandState);
                        res = True;
                        }
                    }
                }
            }
        }
    return res;
}

void TMenuView::do_a_select( TEvent& event )
{
    putEvent( event );
    event.message.command = owner->execView(this);
    if( event.message.command != 0 && commandEnabled(event.message.command) )
        {
        event.what = evCommand;
        event.message.infoPtr = nullptr;
        putEvent(event);
        }
    clearEvent(event);
}

void TMenuView::handleEvent( TEvent& event )
{
    if( menu != nullptr )
        switch (event.what)
            {
            case  evMouseDown:
                do_a_select(event);
                break;
            case  evKeyDown:
                if( findItem(getAltChar(event.keyDown.keyCode)) != nullptr )
                    do_a_select(event);
                else
                    {
                    TMenuItem *p = hotKey(event.keyDown.keyCode);
                    if( p != nullptr && commandEnabled(p->command))
                        {
                        event.what = evCommand;
                        event.message.command = p->command;
                        event.message.infoPtr = nullptr;
                        putEvent(event);
                        clearEvent(event);
                        }
                    }
                break;
            case  evCommand:
                if( event.message.command == cmMenu )
                    do_a_select(event);
                break;
            case  evBroadcast:
                if( event.message.command == cmCommandSetChanged )
                    {
                    if( updateMenu(menu) )
                        drawView();
                    }
                break;
            }
}


TMenuItem *TMenuView::findHotKey( TMenuItem *p, ushort keyCode )
{

    while( p != nullptr )
        {
        if( p->name != nullptr )
            {
            if( p->command == 0 )
                {
                TMenuItem *T;
                if( (T = findHotKey( p->subMenu->items, keyCode )) != nullptr )
                    return T;
                }
            else if( !p->disabled &&
                     p->keyCode != kbNoKey &&
                     p->keyCode == keyCode
                   )
                return p;
            }
        p =  p->next;
        }
    return nullptr;
}

TMenuItem *TMenuView::hotKey( ushort keyCode )
{
    return findHotKey( menu->items, keyCode );
}

TMenuView *TMenuView::newSubView( const TRect& bounds,
                                  TMenu *aMenu,
                                  TMenuView *aParentMenu
                               )
{
    return new TMenuBox( bounds, aMenu, aParentMenu );
}

#if !defined(NO_STREAMABLE)

void TMenuView::writeMenu( opstream& os, TMenu *menu )
{
    uchar tok = 0xFF;

    assert( menu != nullptr );

    for( TMenuItem *item = menu->items; item != nullptr; item = item->next )
        {
        os << tok;
        os.writeString( item->name );
        os << item->command << (int)(item->disabled)
           << item->keyCode << item->helpCtx;
        if( item->name != nullptr )
            {
            if( item->command == 0 )
                writeMenu( os, item->subMenu );
            else
                os.writeString( item->param );
            }
        }

    tok = 0;
    os << tok;
}

void TMenuView::write( opstream& os )
{
    TView::write( os );
    writeMenu( os, menu );
}

TMenu *TMenuView::readMenu( ipstream& is )
{
    TMenu *menu = new TMenu;
    TMenuItem **last = &(menu->items);
#ifndef __UNPATCHED
    TMenuItem *item;
#else
    TMenuItem *item = 0;
#endif

    uchar tok;
    is >> tok;

    while( tok != 0 )
        {
        assert( tok == 0xFF );

	/* SS: this line gave problems with egcs-1.0.3 */

        item = new TMenuItem( nullptr, 0, nullptr );
        *last = item;
        last = &(item->next);
        item->name = is.readString();
        int temp;
        is >> item->command >> temp
           >> item->keyCode >> item->helpCtx;
        item->disabled = Boolean( temp );
        if( item->name != nullptr )
            {
            if( item->command == 0 )
                item->subMenu = readMenu( is );
            else
                item->param = is.readString();
            }
        is >> tok;
        }
    *last = nullptr;
    menu->deflt = menu->items;
    return menu;
}

void *TMenuView::read( ipstream& is )
{
    TView::read( is );
    menu = readMenu( is );
    parentMenu = nullptr;
    current = nullptr;
    return this;
}

TStreamable *TMenuView::build()
{
    return new TMenuView( streamableInit );
}

TMenuView::TMenuView( StreamableInit ) : TView( streamableInit )
{
}


#endif
