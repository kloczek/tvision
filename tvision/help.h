/*
 * help.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_INC_HELP_H )
#define TV_INC_HELP_H

/** \file help.h
 * help.h
 */

#define Uses_TStreamable
#define Uses_ipstream
#define Uses_opstream
#define Uses_fpstream
#define Uses_TObject
#define Uses_TPoint
#define Uses_TRect
#define Uses_TEvent
#define Uses_TScroller
#define Uses_TScrollBar
#define Uses_TWindow
#include <tv.h>

#include <helpbase.h>

// THelpViewer

/**
 * Part of the help system.
 */
class THelpViewer : public TScroller
{
public:
    /**
     * Undocumented.
     */
    THelpViewer( const TRect&, TScrollBar*, TScrollBar*, THelpFile*, ushort );
    /**
     * Undocumented.
     */
    ~THelpViewer();
    /**
     * Undocumented.
     */
    virtual void changeBounds( const TRect& );
    /**
     * Undocumented.
     */
    virtual void draw();
    /**
     * Undocumented.
     */
    virtual TPalette& getPalette() const;
    /**
     * Undocumented.
     */
    virtual void handleEvent( TEvent& );
    /**
     * Undocumented.
     */
    void makeSelectVisible( int, TPoint&, uchar&, int& );
    /**
     * Undocumented.
     */
    void switchToTopic( int );
    /**
     * Undocumented.
     */
    THelpFile *hFile;
    /**
     * Undocumented.
     */
    THelpTopic *topic;
    /**
     * Undocumented.
     */
    int selected;
};

// THelpWindow

/**
 * Part of the help system.
 */
class THelpWindow : public TWindow
{
    static const char * helpWinTitle;
public:
    /**
     * Undocumented.
     */
    THelpWindow( THelpFile*, ushort );
    /**
     * Undocumented.
     */
    virtual TPalette& getPalette() const;
};

/**
 * Undocumented.
 */
extern void notAssigned( opstream& s, int value );

/**
 * Undocumented.
 */
extern TCrossRefHandler crossRefHandler;

#endif  // TV_INC_HELP_H
