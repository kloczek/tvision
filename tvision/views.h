/*
 * views.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( __COMMAND_CODES )
#define __COMMAND_CODES

    /** \file views.h
     * views.h
     */

const ushort

//  Standard command codes

    cmValid         = 0,
    cmQuit          = 1,
    cmError         = 2,
    cmMenu          = 3,
    cmClose         = 4,
    cmZoom          = 5,
    cmResize        = 6,
    cmNext          = 7,
    cmPrev          = 8,
    cmHelp          = 9,

//  TDialog standard commands

    cmOK            = 10,
    cmCancel        = 11,
    cmYes           = 12,
    cmNo            = 13,
    cmDefault       = 14,

// Standard application commands

    cmNew           = 30,
    cmOpen          = 31,
    cmSave          = 32,
    cmSaveAs        = 33,
    cmSaveAll       = 34,
    cmChDir         = 35,
    cmDosShell      = 36,
    cmCloseAll      = 37,

    // SS: some new internal commands.

    cmSysRepaint    = 38,
    cmSysResize     = 39,
    cmSysWakeup     = 40,

//  TView State masks

    /** \var sfVisible
     * Set if the view is visible on its owner. Views are by default
     * sfVisible. Use @ref TView::show() and @ref TView::hide() to modify
     * sfVisible. An sfVisible view is not necessarily visible on the screen,
     * since its owner might not be visible. To test for visibility on the
     * screen, examine the @ref sfExposed bit or call @ref TView::exposed().
     * @see TView::state
     */
    sfVisible       = 0x001,

    /** \var sfCursorVis
     * Set if a view's cursor is visible. Clear is the default. You can
     * use @ref TView::showCursor() and @ref TView::hideCursor() to modify
     * sfCursorVis.
     * @see TView::state
     */
    sfCursorVis     = 0x002,

    /** \var sfCursorIns
     * Set if the view's cursor is a solid block; clear if the view's cursor
     * is an underline (the default). Use @ref TView::blockCursor() and
     * @ref TView::normalCursor() to modify this bit.
     * @see TView::state
     */
    sfCursorIns     = 0x004,

    /** \var sfShadow
     * Set if the view has a shadow.
     * @see TView::state
     */
    sfShadow        = 0x008,

    /** \var sfActive
     * Set if the view is the active window or a subview in the active window.
     * @see TView::state
     */
    sfActive        = 0x010,

    /** \var sfSelected
     * Set if the view is the currently selected subview within its owner.
     * Each @ref TGroup object has a @ref TGroup::current data member that
     * points to the currently selected subview (or is 0 if no subview is
     * selected). There can be only one currently selected subview in a
     * @ref TGroup.
     * @see TView::state
     */
    sfSelected      = 0x020,

    /** \var sfFocused
     * Set if the view is focused. A view is focused if it is selected and
     * all owners above it are also selected. The last view on the focused
     * chain is the final target of all focused events.
     * @see TView::state
     */
    sfFocused       = 0x040,

    /** \var sfDragging
     * Set if the view is being dragged.
     * @see TView::state
     */
    sfDragging      = 0x080,

    /** \var sfDisabled
     * Set if the view is disabled. A disabled view will ignore all events
     * sent to it.
     * @see TView::state
     */
    sfDisabled      = 0x100,

    /** \var sfModal
     * Set if the view is modal. There is always exactly one modal view in
     * a running TVision application, usually a @ref TApplication or
     * @ref TDialog object. When a view starts executing (through an
     * @ref TGroup::execView() call), that view becomes modal. The modal
     * view represents the apex (root) of the active event tree, getting
     * and handling events until its @ref TView::endModal() method is called.
     * During this "local" event loop, events are passed down to lower
     * subviews in the view tree. Events from these lower views pass back
     * up the tree, but go no further than the modal view. See also
     * @ref TView::setState(), @ref TView::handleEvent() and
     * @ref TGroup::execView().
     * @see TView::state
     */
    sfModal         = 0x200,

    /** \var sfDefault
     * This is a spare flag, available to specify some user-defined default
     * state.
     * @see TView::state
     */
    sfDefault       = 0x400,

    /** \var sfExposed
     * Set if the view is owned directly or indirectly by the application
     * object, and therefore possibly visible on the. @ref TView::exposed()
     * uses this flag in combination with further clipping calculations to
     * determine whether any part of the view is actually visible on the
     * screen.
     * @see TView::state
     */
    sfExposed       = 0x800,

// TView Option masks

    /** \var ofSelectable
     * Set if the view should select itself automatically (see
     * @ref sfSelected); for example, by a mouse click in the view, or a Tab
     * in a dialog box.
     * @see TView::options
     */
    ofSelectable    = 0x001,

    /** \var ofTopSelect
     * Set if the view should move in front of all other peer views when
     * selected. When the ofTopSelect bit is set, a call to
     * @ref TView::select() corresponds to a call to @ref TView::makeFirst().
     * @ref TWindow and descendants by default have the ofTopSelect bit set,
     * which causes them to move in front of all other windows on the desktop
     * when selected.
     * @see TView::options
     */
    ofTopSelect     = 0x002,

    /** \var ofFirstClick
     * If clear, a mouse click that selects a view will have no further
     * effect. If set, such a mouse click is processed as a normal mouse
     * click after selecting the view. Has no effect unless @ref ofSelectable
     * is also set. See also @ref TView::handleEvent(), @ref sfSelected and
     * @ref ofSelectable.
     * @see TView::options
     */
    ofFirstClick    = 0x004,

    /** \var ofFramed
     * Set if the view should have a frame drawn around it. A @ref TWindow
     * and any class derived from @ref TWindow, has a @ref TFrame as its last
     * subview. When drawing itself, the @ref TFrame will also draw a frame
     * around any other subviews that have the ofFramed bit set.
     * @see TView::options
     */
    ofFramed        = 0x008,

    /** \var ofPreProcess
     * Set if the view should receive focused events before they are sent to
     * the focused view. Otherwise clear. See also @ref sfFocused,
     * @ref ofPostProcess, and @ref TGroup::phase.
     * @see TView::options
     */
    ofPreProcess    = 0x010,

    /** \var ofPostProcess
     * Set if the view should receive focused events whenever the focused
     * view fails to handle them. Otherwise clear. See also @ref sfFocused,
     * @ref ofPreProcess and @ref TGroup::phase.
     * @see TView::options
     */
    ofPostProcess   = 0x020,

    /** \var ofBuffered
     * Used for @ref TGroup objects and classes derived from @ref TGroup
     * only. Set if a cache buffer should be allocated if sufficient memory
     * is available. The group buffer holds a screen image of the whole
     * group so that group redraws can be speeded up. In the absence of a
     * buffer, @ref TGroup::draw() calls on each subview's
     * @ref TView::drawView() method. If subsequent memory allocation calls
     * fail, group buffers will be deallocated to make memory available.
     * @see TView::options
     */
    ofBuffered      = 0x040,

    /** \var ofTileable
     * Set if the desktop can tile (or cascade) this view. Usually used
     * only with @ref TWindow objects.
     * @see TView::options
     */
    ofTileable      = 0x080,

    /** \var ofCenterX
     * Set if the view should be centered on the x-axis of its owner when
     * inserted in a group using @ref TGroup::insert().
     * @see TView::options
     */
    ofCenterX       = 0x100,

    /** \var ofCenterY
     * Set if the view should be centered on the y-axis of its owner when
     * inserted in a group using @ref TGroup::insert().
     * @see TView::options
     */
    ofCenterY       = 0x200,

    /** \var ofCentered
     * Set if the view should be centered on both axes of its owner when
     * inserted in a group using @ref TGroup::insert().
     * @see TView::options
     */
    ofCentered      = 0x300,

    /** \var ofValidate
     * Undocumented.
     * @see TView::options
     */
    ofValidate      = 0x400,

// TView GrowMode masks

    /** \var gfGrowLoX
     * If set, the left-hand side of the view will maintain a constant
     * distance from its owner's right-hand side. If not set, the movement
     * indicated won't occur.
     * @see TView::growMode
     */
    gfGrowLoX       = 0x01,

    /** \var gfGrowLoY
     * If set, the top of the view will maintain a constant distance from
     * the bottom of its owner.
     * @see TView::growMode
     */
    gfGrowLoY       = 0x02,

    /** \var gfGrowHiX
     * If set, the right-hand side of the view will maintain a constant
     * distance from its owner's right side.
     * @see TView::growMode
     */
    gfGrowHiX       = 0x04,

    /** \var gfGrowHiY
     * If set, the bottom of the view will maintain a constant distance
     * from the bottom of its owner's.
     * @see TView::growMode
     */
    gfGrowHiY       = 0x08,

    /** \var gfGrowAll
     * If set, the view will move with the lower-right corner of its owner.
     * @see TView::growMode
     */
    gfGrowAll       = 0x0f,

    /** \var gfGrowRel
     * For use with @ref TWindow objects that are in the desktop. The view
     * will change size relative to the owner's size, maintaining that
     * relative size with respect to the owner even when screen is resized.
     * @see TView::growMode
     */
    gfGrowRel       = 0x10,

    /** \var gfFixed
     * Undocumented.
     * @see TView::growMode
     */
    gfFixed         = 0x20,

// TView DragMode masks

    /** \var dmDragMove
     * Allow the view to move.
     * @see TView::dragMode
     */
    dmDragMove      = 0x01,

    /** \var dmDragGrow
     * Allow the view to change size.
     * @see TView::dragMode
     */
    dmDragGrow      = 0x02,

    /** \var dmLimitLoX
     * The view's left-hand side cannot move outside limits.
     * @see TView::dragMode
     */
    dmLimitLoX      = 0x10,

    /** \var dmLimitLoY
     * The view's top side cannot move outside limits.
     * @see TView::dragMode
     */
    dmLimitLoY      = 0x20,

    /** \var dmLimitHiX
     * The view's right-hand side cannot move outside limits.
     * @see TView::dragMode
     */
    dmLimitHiX      = 0x40,

    /** \var dmLimitHiY
     * The view's bottom side cannot move outside limits.
     * @see TView::dragMode
     */
    dmLimitHiY      = 0x80,

    /** \var dmLimitAll
     * No part of the view can move outside limits.
     * @see TView::dragMode
     */
    dmLimitAll      = dmLimitLoX | dmLimitLoY | dmLimitHiX | dmLimitHiY,

// TView Help context codes

    /** \var hcNoContext
     * No context specified.
     * @see TView::helpCtx
     */
    hcNoContext     = 0,

    /** \var hcDragging
     * Object is being dragged.
     * @see TView::helpCtx
     */
    hcDragging      = 1,

// TScrollBar part codes

    /** \var sbLeftArrow
     * Left arrow of horizontal scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbLeftArrow     = 0,

    /** \var sbRightArrow
     * Right arrow of horizontal scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbRightArrow    = 1,

    /** \var sbPageLeft
     * Left paging area of horizontal scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbPageLeft      = 2,

    /** \var sbPageRight
     * Right paging area of horizontal scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbPageRight     = 3,

    /** \var sbUpArrow
     * Top arrow of vertical scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbUpArrow       = 4,

    /** \var sbDownArrow
     * Bottom arrow of vertical scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbDownArrow     = 5,

    /** \var sbPageUp
     * Upper paging area of vertical scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbPageUp        = 6,

    /** \var sbPageDown
     * Lower paging area of vertical scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbPageDown      = 7,

    /** \var sbIndicator
     * Position indicator on scroll bar.
     * @see TScrollBar::scrollStep
     */
    sbIndicator     = 8,

// TScrollBar options for TWindow.StandardScrollBar

    /** \var sbHorizontal
     * The scroll bar is horizontal.
     * @see TWindow::standardScrollBar
     */
    sbHorizontal    = 0x000,

    /** \var sbVertical
     * The scroll bar is vertical.
     * @see TWindow::standardScrollBar
     */
    sbVertical      = 0x001,

    /** \var sbHandleKeyboard
     * The scroll bar responds to keyboard commands.
     * @see TWindow::standardScrollBar
     */
    sbHandleKeyboard = 0x002,

// TWindow Flags masks

    /** \var wfMove
     * Window can be moved.
     * @see TWindow::flags.
     */
    wfMove          = 0x01,

    /** \var wfGrow
     * Window can be resized and has a grow icon in the lower-right corner.
     * @see TWindow::flags.
     */
    wfGrow          = 0x02,

    /** \var wfClose
     * Window frame has a close icon that can be mouse-clicked to close the
     * window.
     * @see TWindow::flags.
     */
    wfClose         = 0x04,

    /** \var wfZoom
     * Window frame has a zoom icon that can be mouse-clicked.
     * @see TWindow::flags.
     */
    wfZoom          = 0x08,

//  TView inhibit flags

    noMenuBar       = 0x0001,
    noDeskTop       = 0x0002,
    noStatusLine    = 0x0004,
    noBackground    = 0x0008,
    noFrame         = 0x0010,
    noViewer        = 0x0020,
    noHistory       = 0x0040,

// TWindow number constants

    /** \var wnNoNumber
     * Use the constant wnNoNumber to indicate that the window is not to be
     * numbered and cannot be selected via the Alt+number key.
     * @see TWindow::TWindow
     */
    wnNoNumber      = 0,

// TWindow palette entries

    /** \var wpBlueWindow
     * Window text is yellow on blue.
     * @see TWindow::palette
     */
    wpBlueWindow    = 0,

    /** \var wpCyanWindow
     * Window text is blue on cyan.
     * @see TWindow::palette
     */
    wpCyanWindow    = 1,

    /** \var wpGrayWindow
     * Window text is black on gray.
     * @see TWindow::palette
     */
    wpGrayWindow    = 2,

//  Application command codes

    cmCut           = 20,
    cmCopy          = 21,
    cmPaste         = 22,
    cmUndo          = 23,
    cmClear         = 24,
    cmTile          = 25,
    cmCascade       = 26,

// Standard messages

    cmReceivedFocus     = 50,
    cmReleasedFocus     = 51,
    cmCommandSetChanged = 52,

// TScrollBar messages

    cmScrollBarChanged  = 53,
    cmScrollBarClicked  = 54,

// TWindow select messages

    cmSelectWindowNum   = 55,

//  TListViewer messages

    cmListItemSelected  = 56,

//  Event masks

    /** \var positionalEvents
     * Defines the event classes that are positional events.
     * The focusedEvents and positionalEvents masks are used by
     * @ref TGroup::handleEvent() to determine how to dispatch an event to the
     * group's subviews. If an event class isn't contained in
     * @ref focusedEvents or positionalEvents, it is treated as a broadcast
     * event.
     */
    positionalEvents    = evMouse,

    /** \var focusedEvents
     * Defines the event classes that are focused events.
     * The focusedEvents and positionalEvents values are used by
     * @ref TGroup::handleEvent() to determine how to dispatch an event to the
     * group's subviews. If an event class isn't contained in
     * focusedEvents or @ref positionalEvents, it is treated as a broadcast
     * event.
     */
    focusedEvents       = evKeyboard | evCommand;

#endif  // __COMMAND_CODES

#if defined( Uses_TCommandSet ) && !defined( __TCommandSet )
#define __TCommandSet

/**
 * TCommandSet is a non-view class for handling command sets.
 *
 * Member functions are provided for enabling and disabling commands and for
 * testing for the presence of a given command.
 * Several operators are overloaded to allow natural testing for equality and
 * so on.
 *
 * Note: this object can only handle commands whose code is within 0 and 255.
 * Only commands in this range may be disabled.
 * @short Implements a non-view class for handling command sets
 */
class TCommandSet
{
public:
    /**
     * Constructor.
     *
     * Creates and clears a command set.
     */
    TCommandSet();
    /**
     * Constructor.
     *
     * This form creates a command set and initializes it from the `tc'
     * argument.
     */
    TCommandSet( const TCommandSet& tc );
    /**
     * Returns True if command `cmd' is in the set.
     */
    Boolean has( int cmd );
    /**
     * Removes command `cmd' from the set.
     */
    void disableCmd( int cmd );
    /**
     * Adds command `cmd' to the set.
     */
    void enableCmd( int cmd );
    /**
     * Adds command `cmd' to the set.
     */
    void operator += ( int cmd );
    /**
     * Removes command `cmd' from the set.
     */
    void operator -= ( int cmd );
    /**
     * Removes all commands in set `tc' from this command set.
     */
    void disableCmd( const TCommandSet& tc );
    /**
     * Adds all commands in set `tc' to this command set.
     */
    void enableCmd( const TCommandSet& tc );
    /**
     * Adds all commands in set `tc' to this command set.
     */
    void operator += ( const TCommandSet& tc );
    /**
     * Removes all commands in set `tc' from this command set.
     */
    void operator -= ( const TCommandSet& tc );
    /**
     * Returns True if the command set is empty.
     */
    Boolean isEmpty();
    /**
     * Calculates the intersection of this set and the `tc' set.
     *
     * The resulting set is the largest set which contains commands present in
     * both sets. Returns a reference to this object.
     */
    TCommandSet& operator &= ( const TCommandSet& tc);
    /**
     * Calculates the union of this set and the `tc' set.
     *
     * The resulting set is the smallest set which contains commands present
     * in either sets. Returns a reference to this object.
     */
    TCommandSet& operator |= ( const TCommandSet& tc);
    /**
     * Calculates the intersection of this set and the `tc' set.
     *
     * The resulting set is the largest set which contains commands present in
     * both sets. Returns the resulting set.
     */
    friend TCommandSet operator & ( const TCommandSet&, const TCommandSet& );
    /**
     * Calculates the union of this set and the `tc' set.
     *
     * The resulting set is the smallest set which contains commands present
     * in either sets. Returns the resulting set.
     */
    friend TCommandSet operator | ( const TCommandSet&, const TCommandSet& );
    /**
     * Returns 1 if the sets `tc1' and `tc2' are equal.
     *
     * Otherwise returns 0.
     */
    friend int operator == ( const TCommandSet& tc1, const TCommandSet& tc2 );
    /**
     * Returns 1 if the sets `tc1' and `tc2' are not equal.
     *
     * Otherwise returns 0.
     */
    friend int operator != ( const TCommandSet& tc1, const TCommandSet& tc2 );
private:
    /**
     * Undocumented.
     */
    int loc( int );
    /**
     * Undocumented.
     */
    int mask( int );
    /**
     * Undocumented.
     */
    static int masks[8];
    /**
     * Undocumented.
     */
    uchar cmds[32];
};

inline void TCommandSet::operator += ( int cmd )
{
    enableCmd( cmd );
}

inline void TCommandSet::operator -= ( int cmd )
{
    disableCmd( cmd );
}

inline void TCommandSet::operator += ( const TCommandSet& tc )
{
    enableCmd( tc );
}

inline void TCommandSet::operator -= ( const TCommandSet& tc )
{
    disableCmd( tc );
}

inline int operator != ( const TCommandSet& tc1, const TCommandSet& tc2 )
{
    return !operator == ( tc1, tc2 );
}

inline int TCommandSet::loc( int cmd )
{
    return cmd / 8;
}

inline int TCommandSet::mask( int cmd )
{
    return masks[ cmd & 0x07 ];
}

#endif  // Uses_TCommandSet

#if defined( Uses_TPalette ) && !defined( __TPalette )
#define __TPalette

/**
 * TPalette is a simple class used to create and manipulate palette arrays.
 *
 * Although palettes are arrays of char, and are often referred to as strings,
 * they are not the conventional null-terminated strings found in C. Normal C
 * string functions cannot be used.
 *
 * The first byte of a palette string holds its length (not counting the first
 * byte itself). Each basic view has a default palette that determines the
 * usual colors assigned to the various parts of a view, such as scroll bars,
 * frames, buttons, text, and so on.
 * @short Simple class used to create and manipulate palette arrays
 */
class TPalette
{
public:
    /**
     * Creates a TPalette object with string `d' and length `len'. The private
     * member @ref data is set with `len' in its first byte, following by the
     * array `d'.
     */
    TPalette( const char *d, ushort len );
    /**
     * Creates a new palette by copying the palette `tp'.
     */
    TPalette( const TPalette& tp );
    /**
     * Destroys the palette.
     */
    ~TPalette();
    /**
     * The code p = tp; copies the palette `tp' to the palette `p'.
     */
    TPalette& operator = ( const TPalette& tp );
    /**
     * The subscripting operator returns the character at the index'th
     * position.
     */
    uchar& operator[]( int index ) const;
    /**
     * Undocumented.
     */
    uchar *data;
};

#endif  // Uses_TPalette

#if defined( Uses_TView ) && !defined( __TView )
#define __TView

class TRect;
class TEvent;
class TGroup;

/**
 * The base of all visible objects.
 *
 * Most programs make use of the TView derivatives: @ref TFrame,
 * @ref TScrollBar, @ref TScroller, @ref TListViewer, @ref TGroup, and
 * @ref TWindow objects.
 *
 * TView objects are rarely instantiated in TVision programs. The TView
 * class exists to provide basic data and functionality for its derived
 * classes. You'll probably never need to construct an instance of TView
 * itself, but most of the common behavior of visible elements in TVision
 * applications comes from TView.
 * @short The base of all visible objects
 */
class TView : public TObject, public TStreamable
{
public:
    /**
     * @see TGroup::handleEvent
     * @see TGroup::phase
     */
    enum phaseType { phFocused, phPreProcess, phPostProcess };
    /**
     * Used internally by TVision.
     */
    enum selectMode{ normalSelect, enterSelect, leaveSelect };
    /**
     * Creates a TView object with the given `bounds' rectangle. TView
     * constructor calls the TObject constructor and then sets the data
     * members of the new TView to the following values:
     *
     * <pre>
     * Data member Value
     *
     * cursor      (0, 0)
     * dragMode    @ref dmLimitLoY
     * eventMask   @ref evMouseDown | @ref evKeyDown | @ref evCommand
     * growMode    0
     * helpCtx     @ref hcNoContext
     * next        0
     * options     0
     * origin      (bounds.A.x, bounds.A.y)
     * owner       0
     * size        (bounds.B.x - bounds.A.x, bounds.B.y - bounds.A.y)
     * state       @ref sfVisible
     * </pre>
     */
    TView( const TRect& bounds );
    /**
     * Hides the view and then, if it has an owner, removes it from the group.
     */
    ~TView();
    /**
     * Sets, in the `min' and `max' arguments, the minimum and maximum values
     * that @ref size data member may assume.
     *
     * The default TView::sizeLimits returns (0, 0) in `min' and owner->size
     * in `max'. If @ref owner data member is 0, `max.x' and `max.y' are both
     * set to INT_MAX.
     */
    virtual void sizeLimits( TPoint& min, TPoint& max );
    /**
     * Returns the current value of size, the bounding rectangle of the view
     * in its owner's coordinate system.
     *
     * -# `a' is set to @ref origin
     * -# `b' is set to @ref origin + @ref size
     */
    TRect getBounds();
    /**
     * Returns the extent rectangle of the view.
     *
     * -# `a' is set to (0, 0)
     * -# `b' is set to @ref size
     */
    TRect getExtent();
    /**
     * Returns the clipping rectangle: the smallest rectangle which needs
     * to be redrawn in a @ref draw() call.
     *
     * For complicated views, draw() can use getClipRect() to improve
     * performance noticeably.
     */
    TRect getClipRect();
    /**
     * Returns True if the `mouse' argument (given in global coordinates) is
     * within the calling view. Call @ref makeGlobal and @ref makeLocal to
     * convert one point between different coordinate systems.
     */
    Boolean mouseInView( TPoint mouse );
    /**
     * Returns True if a mouse event occurs inside the calling view, otherwise
     * returns False. Returns True if the view is visible and the mouse
     * coordinates (defined in `event.mouse.where') are within this view.
     *
     * The coordinate is defined in the global coordinate system.
     * @see TView::makeGlobal
     * @see TView::makeLocal
     */
    Boolean containsMouse( TEvent& event );
    /**
     * Changes the bounds of the view to those of the `bounds' argument.
     * The view is redrawn in its new location.
     *
     * locate() calls @ref sizeLimits() to verify that the given bounds are
     * valid, and then calls @ref changeBounds() to change the bounds and
     * redraw the view.
     */
    void locate( TRect& bounds );
    /**
     * Drags the view in the ways specified by the `mode' argument, that is
     * interpreted like the @ref growMode data member.
     *
     * `limits' specifies the rectangle (in the owner's coordinate system)
     * within which the view can be moved, and `min' and `max' specify the
     * minimum and maximum sizes the view can shrink or grow to.
     *
     * The event leading to the dragging operation is needed in `event' to
     * distinguish mouse dragging from use of the cursor keys.
     */
    virtual void dragView( TEvent& event, uchar mode,  //  temporary fix
      TRect& limits, TPoint minSize, TPoint maxSize ); //  for Miller's stuff
    /**
     * When a view's owner changes size, the owner repeatedly calls
     * calcBounds() and @ref changeBounds() for all its subviews.
     *
     * calcBounds() must calculate the new bounds of the view given that its
     * owner's size has changed by `delta', and return the new bounds in
     * `bounds'.
     *
     * calcBounds() calculates the new bounds using the flags specified
     * in @ref growMode data member.
     */
    virtual void calcBounds( TRect& bounds, TPoint delta );
    /**
     * changeBounds() must change the view's bounds (@ref origin and @ref size
     * data members) to the rectangle given by the `bounds' parameter.
     * Having changed the bounds, changeBounds() must then redraw the view.
     *
     * changeBounds() is called by various TView member functions, but should
     * never be called directly.
     *
     * changeBounds() first calls @ref setBounds(bounds) and then calls
     * @ref drawView().
     */
    virtual void changeBounds( const TRect& bounds );
    /**
     * Grows or shrinks the view to the given size using a call to
     * @ref locate().
     */
    void growTo( short x, short y );
    /**
     * Moves the origin to the point (x,y) relative to the owner's view. The
     * view's size is unchanged.
     */
    void moveTo( short x, short y );
    /**
     * Sets the bounding rectangle of the view to the value given by the
     * `bounds' parameter. The @ref origin data member is set to `bounds.a',
     * and the @ref size data member is set to the difference between
     * `bounds.b' and `bounds.a'.
     *
     * The setBounds() member function is intended to be called only from
     * within an overridden @ref changeBounds() member function. You should
     * never call setBounds() directly.
     */
    void setBounds( const TRect& bounds );
    /**
     * getHelpCtx() returns the view's help context. The default getHelpCtx()
     * returns the value in the @ref helpCtx data member, or returns
     * @ref hcDragging if the view is being dragged (see @ref sfDragging).
     */
    virtual ushort getHelpCtx();
    /**
     * Use this member function to check the validity of a view after it has
     * been constructed or at the point in time when a modal state ends (due
     * to a call to @ref endModal()).
     *
     * A `command' argument of cmValid (zero) indicates that the view should
     * check the result of its constructor: valid(cmValid) should return True
     * if the view was successfully constructed and is now ready to be used,
     * False otherwise.
     *
     * Any other (nonzero) `command' argument indicates that the current modal
     * state (such as a modal dialog box) is about to end with a resulting
     * value of `command'. In this case, valid() should check the validity of
     * the view.
     *
     * It is the responsibility of valid() to alert the user in case the view
     * is invalid. The default TView::valid() simply returns True.
     */
    virtual Boolean valid( ushort command );
    /**
     * Hides the view by calling @ref setState() to clear the @ref sfVisible
     * flag in the @ref state data member.
     */
    void hide();
    /**
     * If the view is @ref sfVisible, nothing happens. Otherwise, show()
     * displays the view by calling @ref setState() to set the @ref sfVisible
     * flag in @ref state data member.
     */
    void show();
    /**
     * Draws the view on the screen.
     *
     * Called whenever the view must draw (display) itself. draw() must cover
     * the entire area of the view.
     *
     * This member function must be overridden appropriately for each derived
     * class. draw() is seldom called directly, since it is more efficient to
     * use @ref drawView(), which draws only views that are exposed; that is,
     * some or all of the view is visible on the screen.
     *
     * If required, draw can call @ref getClipRect() to obtain the rectangle
     * that needs redrawing, and then only draw that area. For complicated
     * views, this can improve performance noticeably.
     *
     * To perform its task, draw() usually uses a @ref TDrawBuffer object.
     */
    virtual void draw();
    /**
     * Draws the view on the screen.
     *
     * Calls @ref draw() if @ref exposed() returns True, indicating that the
     * view is exposed (see @ref sfExposed). If @ref exposed() returns False,
     * drawView() does nothing.
     *
     * You should call drawView() (not draw()) whenever you need to redraw a
     * view after making a change that affects its visual appearance.
     */
    void drawView();
    /**
     * Checks if the view is exposed.
     *
     * Returns True if any part of the view is visible on the screen. The view
     * is exposed if:
     *
     * -# it has the @ref sfExposed bit set in @ref state data member
     * -# it has the @ref sfVisible bit set in @ref state data member
     * -# its coordinates make it fully or partially visible on the screen.
     */
    Boolean exposed();
    /**
     * Tries to grab the focus.
     *
     * The view can grab the focus if:
     *
     * -# the view is not selected (bit @ref sfSelected cleared in @ref state)
     * -# the view is not modal (bit @ref sfModal cleared in @ref state)
     * -# the owner exists and it is focused
     *
     * If all the above conditions are True, the focus() method calls
     * @ref select() to get the focus.
     */
    Boolean focus();
    /**
     * Hides the cursor by calling @ref setState() to clear the
     * @ref sfCursorVis flag in the @ref state data member.
     */
    void hideCursor();
    /**
     * Calls @ref drawCursor() followed by @ref drawUnderView(). The latter
     * redraws all subviews (with shadows if required) until the given
     * `lastView' is reached.
     */
    void drawHide( TView *lastView );
    /**
     * Calls @ref drawView(), then if @ref state data member has the
     * @ref sfShadow bit set, @ref drawUnderView() is called to draw the
     * shadow.
     */
    void drawShow( TView *lastView );
    /**
     * Calls owner->clip.intersect(r) to set the area that needs drawing.
     * Then, all the subviews from the next view to the given `lastView' are
     * drawn using @ref drawSubViews(). Finally, owner->clip is reset to
     * owner->getExtent().
     * @see TGroup::clip
     * @see TRect::intersect
     * @see TView::getExtent
     */
    void drawUnderRect( TRect& r, TView *lastView );
    /**
     * Calls drawUnderRect(r, lastView), where `r' is the calling view's
     * current bounds. If `doShadow' is True, the view's bounds are first
     * increased by shadowSize (see `TView.cc' for more).
     * @see drawUnderRect
     */
    void drawUnderView( Boolean doShadow, TView *lastView );
    /**
     * dataSize() must be used to return the size of the data read from and
     * written to data records by @ref setData() and @ref getData(). The data
     * record mechanism is typically used only in views that implement
     * controls for dialog boxes.
     *
     * TView::dataSize() returns zero to indicate that no data was
     * transferred.
     */
    virtual ushort dataSize();
    /**
     * getData() must copy @ref dataSize() bytes from the view to the data
     * record given by the `rec' pointer. The data record mechanism is
     * typically used only in views that implement controls for dialog boxes.
     * @see TView::setData
     *
     * The default TView::getData() does nothing.
     */
    virtual void getData( void *rec );
    /**
     * setData() must copy @ref dataSize() bytes from the data record given by
     * `rec' to the view. The data record mechanism is typically used
     * only in views that implement controls for dialog boxes.
     * @see TView::getData
     *
     * The default TView::setData() does nothing.
     */
    virtual void setData( void *rec );
    /**
     * The default awaken() does nothing. When a group is loaded from a
     * stream, the last thing the @ref TGroup::read() function does is call
     * the awaken() methods of all subviews, giving those views a chance to
     * initialize themselves once all subviews have loaded.
     *
     * If you create objects that depend on other subviews to initialize
     * themselves after being read from a stream, you can override awaken()
     * to perform that initialization.
     */
    virtual void awaken();
    /**
     * Sets @ref sfCursorIns in @ref state data member to change the cursor
     * to a solid block. The cursor will only be visible if @ref sfCursorVis
     * is also set (and the view is visible).
     */
    void blockCursor();
    /**
     * Clears the @ref sfCursorIns bit in @ref state data member, thereby
     * making the cursor into an underline. If @ref sfCursorVis is set, the
     * new cursor will be displayed.
     */
    void normalCursor();
    /**
     * Resets the cursor.
     */
    virtual void resetCursor();
    /**
     * Moves the hardware cursor to the point (x, y) using view-relative
     * (local) coordinates. (0, 0) is the top-left corner.
     */
    void setCursor( int x, int y );
    /**
     * Turns on the hardware cursor by setting the @ref sfCursorVis bit in
     * @ref state data member. Note that the cursor is invisible by default.
     */
    void showCursor();
    /**
     * If the view is @ref sfFocused, the cursor is reset with a call to
     * @ref resetCursor().
     * @see TView::state
     */
    void drawCursor();
    /**
     * Standard member function used in @ref handleEvent() to signal that the
     * view has successfully handled the event.
     *
     * Sets `event.what' to @ref evNothing and `event.message.infoPtr' to this.
     */
    void clearEvent( TEvent& event );
    /**
     * Calls @ref getEvent() and returns True if an event is available. Calls
     * @ref putEvent() to set the event as pending.
     * @see TProgram::pending
     */
    Boolean eventAvail();
    /**
     * Returns the next available event in the `event' argument. Returns
     * @ref evNothing if no event is available. By default, it calls the
     * view's owner's getEvent().
     * @see TGroup::getEvent
     * @see Program::getEvent
     */
    virtual void getEvent( TEvent& event );
    /**
     * handleEvent() is the central member function through which all
     * TVision event handling is implemented. The `what' data member of the
     * `event' parameter contains the event class (evXXXX), and the remaining
     * `event' data members further describe the event.
     *
     * To indicate that it has handled an event, handleEvent() should call
     * @ref clearEvent(). handleEvent() is almost always overridden in derived
     * classes.
     *
     * The default TView::handleEvent() handles @ref evMouseDown events as
     * follows:
     *
     * If the view is:
     *
     * -# not selected (see @ref sfSelected in @ref TView::state)
     * -# and not disabled (see @ref sfDisabled in @ref TView::state)
     * -# and if the view is selectable (see @ref ofSelectable in
     *    @ref TView::options)
     *
     * then the view selects itself by calling @ref select(). No other events
     * are handled by TView::handleEvent().
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Puts the event given by `event' into the event queue, causing it to be
     * the next event returned by @ref getEvent(). Only one event can be pushed
     * onto the event queue in this fashion.
     *
     * Often used by views to generate command events; for example,
     *
     * <pre>
     * event.what = @ref evCommand;
     * event.command = cmSaveAll;
     * event.infoPtr = 0;
     * putEvent(event);
     * </pre>
     *
     * The default TView::putEvent() calls the view's owner's putEvent().
     * @see TGroup::putEvent
     * @see TProgram::pending
     * @see TProgram::putEvent
     */
    virtual void putEvent( TEvent& event );
    /**
     * Returns True if the given command is currently enabled; otherwise it
     * returns False.
     *
     * Note that when you change a modal state, you can then disable and
     * enable commands as you wish; when you return to the previous modal
     * state, however, the original command set will be restored.
     */
    static Boolean commandEnabled( ushort command );
    /**
     * Disables the commands specified in the `commands' argument. If the
     * command set is changed by this call, @ref commandSetChanged is set True.
     */
    static void disableCommands( TCommandSet& commands );
    /**
     * Enables all the commands in the `commands' argument. If the
     * command set is changed by this call, @ref commandSetChanged is set True.
     */
    static void enableCommands( TCommandSet& commands );
    /**
     * Disables the given command. If the
     * command set is changed by the call, @ref commandSetChanged is set True.
     */
    static void disableCommand( ushort command );
    /**
     * Enables the given command. If the
     * command set is changed by this call, @ref commandSetChanged is set True.
     */
    static void enableCommand( ushort command );
    /**
     * Returns, in the `commands' argument, the current command set.
     */
    static void getCommands( TCommandSet& commands );
    /**
     * Changes the current command set to the given `commands' argument.
     */
    static void setCommands( TCommandSet& commands );
    /**
     * Undocumented.
     */
    static void setCmdState( TCommandSet& commands, Boolean enable);
    /**
     * Calls @ref TopView() to seek the top most modal view. If there is none
     * such (that is, if TopView() returns 0) no further action is taken. If
     * there is a modal view, that view calls endModal(), and so on.
     *
     * The net result is that endModal() terminates the current modal state.
     * The `command' argument is passed to the @ref TGroup::execView() that
     * created the modal state in the first place.
     */
    virtual void endModal( ushort command );
    /**
     * Is called from @ref TGroup::execView() whenever a view becomes modal.
     * If a view is to allow modal execution, it must override execute() to
     * provide an event loop. The value returned by execute() will be the
     * value returned by @ref TGroup::execView().
     *
     * The default TView::execute() simply returns cmCancel.
     */
    virtual ushort execute();
    /**
     * Maps the palette indices in the low and high bytes of `color' into
     * physical character attributes by tracing through the palette of the
     * view and the palettes of all its owners.
     */
    ushort getColor( ushort color );
    /**
     * getPalette() must return a string representing the view's palette.
     *
     * This can be 0 (empty string) if the view has no palette. getPalette()
     * is called by @ref writeChar() and @ref writeStr() when converting
     * palette indices to physical character attributes.
     *
     * The default return value of 0 causes no color translation to be
     * performed by this view. getPalette() is almost always overridden in
     * derived classes.
     */
    virtual TPalette& getPalette() const;
    /**
     * Maps the given color to an offset into the current palette. mapColor()
     * works by calling @ref getPalette() for each owning group in the chain.
     *
     * It succesively maps the offset in each palette until the ultimate
     * owning palette is reached.
     *
     * If `color' is invalid (for example, out of range) for any of the
     * palettes encountered in the chain, mapColor() returns @ref errorAttr.
     */
    uchar mapColor( uchar );
    /**
     * Returns True if the state given in `aState' is set in the data member
     * @ref state.
     */
    Boolean getState( ushort aState );
    /**
     * Selects the view (see @ref sfSelected). If the view's owner is focused,
     * then the view also becomes focused (see @ref sfFocused).
     * @see TView::state
     *
     * If the view has the ofTopSelect flag set in its @ref options data
     * member, then the view is moved to the top of its owner's subview list
     * (using a call to @ref makeFirst()).
     */
    void select();
    /**
     * Sets or clears a state flag in the @ref state data member.
     * The `aState' parameter specifies the state flag to modify, and the
     * `enable' parameter specifies whether to turn the flag off (False) or
     * on (True).
     *
     * setState() then carries out any appropriate action to reflect the new
     * state, such as redrawing views that become exposed when the view is
     * hidden (@ref sfVisible), or reprogramming the hardware when the cursor
     * shape is changed (@ref sfCursorVis and @ref sfCursorIns).
     *
     * setState() is sometimes overridden to trigger additional actions that
     * are based on state flags. Another common reason to override setState()
     * is to enable or disable commands that are handled by a particular view.
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Returns, in the `event' variable, the next @ref evKeyDown event.
     * It waits, ignoring all other events, until a keyboard event becomes
     * available.
     */
    void keyEvent( TEvent& event );
    /**
     * Sets the next mouse event in the `event' argument.
     * Returns True if this event is in the `mask' argument. Also returns
     * False if an @ref evMouseUp event occurs.
     *
     * This member function lets you track a mouse while its button is down;
     * for example, in drag block-marking operations for text editors.
     *
     * Here's an extract of a @ref handleEvent() routine that tracks the
     * mouse with the view's cursor:
     *
     * <pre>
     * void TMyView::handleEvent(TEvent& event)
     * {
     *     TView::handleEvent(event);
     *     switch (event.what)
     *     {
     *     case @ref evMouseDown:
     *         do
     *         {
     *             makeLocal(event.where, mouse);
     *             setCursor(mouse.x, mouse.y);
     *         }
     *         while (mouseEvent(event, evmouseMove));
     *         clearEvent(event);
     *     ...
     *     }
     *     ...
     * }
     * </pre>
     * @see TView::clearEvent
     * @see TView::makeLocal
     * @see TView::setCursor
     */
    Boolean mouseEvent( TEvent& event, ushort mask );
    /**
     * Converts the `source' point coordinates from local (view) to global
     * (screen) and returns the result.
     */
    TPoint makeGlobal( TPoint source );
    /**
     * Converts the `source' point coordinates from global (screen) to local
     * (view) and returns the result.
     *
     * Useful for converting the event.where data member of an evMouse event
     * from global coordinates to local coordinates.
     *
     * For example:
     * <pre>
     * mouseLoc = makeLocal(eventWhere);
     * </pre>
     */
    TPoint makeLocal( TPoint source );
    /**
     * Returns a pointer to the next subview in the owner's subview list.
     * A 0 is returned if the calling view is the last one in its owner's
     * list.
     */
    TView *nextView();
    /**
     * Returns a pointer to the previous subview in the owner's subview list.
     * A 0 is returned if the calling view is the first one in its owner's
     * list.
     *
     * Note that @ref prev() treats the list as circular, whereas prevView()
     * treats the list linearly.
     */
    TView *prevView();
    /**
     * Returns a pointer to the previous subview in the owner's subview list.
     * If the calling view is the first one in its owner's list, prev()
     * returns the last view in the list.
     *
     * Note that @ref prev() treats the list as circular, whereas prevView()
     * treats the list linearly.
     */
    TView *prev();
    /**
     * Pointer to next peer view in Z-order. If this is the last subview, next
     * points to owner's first subview.
     */
    TView *next;
    /**
     * Moves the view to the top of its owner's subview list. A call to
     * makeFirst() corresponds to putInFrontOf(owner->first()).
     * @see TGroup::first
     * @see TView::putInFrontOf
     */
    void makeFirst();
    /**
     * Moves the calling view in front of the `Target' view in the owner's
     * subview list. The call
     *
     * <pre>
     * MyView.putInFrontOf(owner->first);
     * </pre>
     *
     * is equivalent to MyView.makeFirst(). This member function works by
     * changing pointers in the subview list.
     * @see TView::makeFirst
     *
     * Depending on the position of the other views and their visibility
     * states, putInFrontOf() may obscure (clip) underlying views.
     *
     * If the view is selectable (see @ref ofSelectable) and is put in front
     * of all other subviews, then the view becomes selected.
     * @see TView::options
     */
    void putInFrontOf( TView *Target );
    /**
     * Returns a pointer to the current modal view, or 0 if none such.
     */
    TView *TopView();
    /**
     * Writes the given buffer to the screen starting at the coordinates
     * (x, y), and filling the region of width `w' and height `h'. Should only
     * be used in @ref draw() member functions.
     * @see TDrawBuffer
     */
    void writeBuf(  short x, short y, short w, short h, const void* b );
    /**
     * Writes the given buffer to the screen starting at the coordinates
     * (x, y), and filling the region of width `w' and height `h'. Should only
     * be used in @ref draw() member functions.
     * @see TDrawBuffer
     */
    void writeBuf(  short x, short y, short w, short h, const TDrawBuffer& b );
    /**
     * Beginning at the point (x, y), writes `count' copies of the character
     * `c' in the color determined by the color'th entry in the current view's
     * palette. Should only be used in @ref draw() functions.
     */
    void writeChar( short x, short y, char c, uchar color, short count );
    /**
     * Writes the line contained in the buffer `b' to the screen, beginning at
     * the point (x, y) within the rectangle defined by the width `w' and the
     * height `h'. If `h' is greater than 1, the line will be repeated `h'
     * times. Should only be used in @ref draw() member functions.
     * @see TDrawBuffer
     */
    void writeLine( short x, short y, short w, short h, const TDrawBuffer& b );
    /**
     * Writes the line contained in the buffer `b' to the screen, beginning at
     * the point (x, y) within the rectangle defined by the width `w' and the
     * height `h'. If `h' is greater than 1, the line will be repeated `h'
     * times. Should only be used in @ref draw() member functions.
     * @see TDrawBuffer
     */
    void writeLine( short x, short y, short w, short h, const void *b );
    /**
     * Writes the string `str' with the color attributes of the color'th entry
     * in the view's palette, beginning at the point (x, y). Should only be
     * used in @ref draw() member functions.
     */
    void writeStr( short x, short y, const char *str, uchar color );
    /**
     * The size of the view.
     */
    TPoint size;
    /**
     * The options word flags determine various behaviors of the view. The
     * following mnemonics are used to refer to the bit positions of the
     * options data member. Setting a bit position to 1 indicates that the
     * view has that particular attribute; clearing the bit position means
     * that the attribute is off or disabled.
     *
     * <pre>
     * Constant      Value  Meaning
     *
* @ref ofSelectable  0x0001 Set if the view should select itself automatically
     *                      (see @ref sfSelected); for example, by a mouse
     *                      click in the view, or a Tab in a dialog box.
     *
* @ref ofTopSelect   0x0002 Set if the view should move in front of all other
     *                      peer views when selected. When the
     *                      @ref ofTopSelect bit is set, a call to
     *                      @ref select() corresponds to a call to
     *                      @ref makeFirst(). @ref TWindow and descendants by
     *                      default have the @ref ofTopSelect bit set, which
     *                      causes them to move in front of all other windows
     *                      on the desktop when selected.
     *
* @ref ofFirstClick  0x0004 If clear, a mouse click that selects a view will
     *                      have no further effect. If set, such a mouse click
     *                      is processed as a normal mouse click after
     *                      selecting the view. Has no effect unless
     *                      @ref ofSelectable is also set. See also
     *                      @ref handleEvent(), @ref sfSelected and
     *                      @ref ofSelectable.
     *
* @ref ofFramed      0x0008 Set if the view should have a frame drawn around
     *                      it. A @ref TWindow and any class derived from
     *                      @ref TWindow, has a @ref TFrame as its last
     *                      subview. When drawing itself, the @ref TFrame
     *                      will also draw a frame around any other subviews
     *                      that have the @ref ofFramed bit set.
     *
* @ref ofPreProcess  0x0010 Set if the view should receive focused events
     *                      before they are sent to the focused view.
     *                      Otherwise clear. See also @ref sfFocused,
     *                      @ref ofPostProcess, and @ref TGroup::phase.
     *
* @ref ofPostProcess 0x0020 Set if the view should receive focused events
     *                      whenever the focused view fails to handle them.
     *                      Otherwise clear. See also @ref sfFocused,
     *                      @ref ofPreProcess and @ref TGroup::phase.
     *
* @ref ofBuffered    0x0040 Used for @ref TGroup objects and classes derived
     *                      from @ref TGroup only. Set if a cache buffer
     *                      should be allocated if sufficient memory is
     *                      available. The group buffer holds a screen image
     *                      of the whole group so that group redraws can be
     *                      speeded up. In the absence of a buffer,
     *                      @ref TGroup::draw() calls on each subview's
     *                      @ref drawView() method. If subsequent memory
     *                      allocation calls fail, group buffers will be
     *                      deallocated to make memory available.
     *
* @ref ofTileable    0x0080 Set if the desktop can tile (or cascade) this
     *                      view. Usually used only with @ref TWindow objects.
     *
* @ref ofCenterX     0x0100 Set if the view should be centered on the x-axis
     *                      of its owner when inserted in a group using
     *                      @ref TGroup::insert().
     *
* @ref ofCenterY     0x0200 Set if the view should be centered on the y-axis
     *                      of its owner when inserted in a group using
     *                      @ref TGroup::insert().
     *
* @ref ofCentered    0x0300 Set if the view should be centered on both axes of
     *                      its owner when inserted in a group using
     *                      @ref TGroup::insert().
     * </pre>
     */
    ushort options;
    /**
     * eventMask is a bit mask that determines which event classes will be
     * recognized by the view.
     *
     * The default eventMask enables @ref evMouseDown, @ref evKeyDown, and
     * @ref evCommand. Assigning 0xFFFF to eventMask causes the view to react
     * to all event classes; conversely, a value of zero causes the view to
     * not react to any events.
     */
    ushort eventMask;
    /**
     * The state of the view is represented by bits set or clear in the state
     * data member. The bits are represented mnemonically by constants as
     * follows.
     *
     * <pre>
     * Constant    Value Meaning
     *
* @ref sfVisible   0x001 Set if the view is visible on its owner. Views are by
     *                   default @ref sfVisible. Use @ref show() and
     *                   @ref hide() to modify @ref sfVisible. An
     *                   @ref sfVisible view is not necessarily visible on the
     *                   screen, since its owner might not be visible. To test
     *                   for visibility on the screen, examine the
     *                   @ref sfExposed bit or call @ref exposed().
     *
* @ref sfCursorVis 0x002 Set if a view's cursor is visible. Clear is the
     *                   default. You can use @ref showCursor() and
     *                   @ref hideCursor() to modify @ref sfCursorVis.
     *
* @ref sfCursorIns 0x004 Set if the view's cursor is a solid block; clear if
     *                   the view's cursor is an underline (the default). Use
     *                   @ref blockCursor() and @ref normalCursor() to modify
     *                   this bit.
     *
* @ref sfShadow    0x008 Set if the view has a shadow.
     *
* @ref sfActive    0x010 Set if the view is the active window or a subview in
     *                   the active window.
     *
* @ref sfSelected  0x020 Set if the view is the currently selected subview
     *                   within its owner. Each @ref TGroup object has a
     *                   @ref TGroup::current data member that points to the
     *                   currently selected subview (or is 0 if no subview is
     *                   selected). There can be only one currently selected
     *                   subview in a @ref TGroup.
     *
* @ref sfFocused   0x040 Set if the view is focused. A view is focused if it
     *                   is selected and all owners above it are also
     *                   selected. The last view on the focused chain is the
     *                   final target of all focused events.
     *
* @ref sfDragging  0x080 Set if the view is being dragged.
     *
* @ref sfDisabled  0x100 Set if the view is disabled. A disabled view will
     *                   ignore all events sent to it.
     *
* @ref sfModal     0x200 Set if the view is modal. There is always exactly one
     *                   modal view in a running TVision application, usually
     *                   a @ref TApplication or @ref TDialog object. When a
     *                   view starts executing (through an
     *                   @ref TGroup::execView() call), that view becomes
     *                   modal. The modal view represents the apex (root) of
     *                   the active event tree, getting and handling events
     *                   until its @ref endModal() method is called. During
     *                   this "local" event loop, events are passed down to
     *                   lower subviews in the view tree. Events from these
     *                   lower views pass back up the tree, but go no further
     *                   than the modal view. See also @ref setState(),
     *                   @ref handleEvent() and @ref TGroup::execView().
     *
* @ref sfDefault   0x400 This is a spare flag, available to specify some
     *                   user-defined default state.
     *
* @ref sfExposed   0x800 Set if the view is owned directly or indirectly by
     *                   the application object, and therefore possibly
     *                   visible on the. @ref exposed() uses this flag in
     *                   combination with further clipping calculations to
     *                   determine whether any part of the view is actually
     *                   visible on the screen.
     * </pre>
     *
     * Many TView member functions test and/or alter the state data member by
     * calling @ref getState() and/or @ref setState().
     */
    ushort state;
    /**
     * The (x, y) coordinates, relative to the owner's origin, of the top-left
     * corner of the view.
     */
    TPoint origin;
    /**
     * The location of the hardware cursor within the view. The cursor is
     * visible only if the view is focused (@ref sfFocused) and the cursor
     * turned on (@ref sfCursorVis).
     * @see TView::state
     *
     * The shape of the cursor is either an underline or block (determined by
     * @ref sfCursorIns).
     */
    TPoint cursor;
    /**
     * Determines how the view will grow when its owner view is resized.
     * To growMode is assigned one or more of the following growMode masks:
     *
     * <pre>
     * Constant  Value Meaning
     *
* @ref gfGrowLoX 0x01  If set, the left-hand side of the view will maintain a
     *                 constant distance from its owner's right-hand side. If
     *                 not set, the movement indicated won't occur.
     *
* @ref gfGrowLoY 0x02  If set, the top of the view will maintain a constant
     *                 distance from the bottom of its owner.
     *
* @ref gfGrowHiX 0x04  If set, the right-hand side of the view will maintain a
     *                  constant distance from its owner's right side.
     *
* @ref gfGrowHiY 0x08  If set, the bottom of the view will maintain a
     *                 constant distance from the bottom of its owner's.
     *
* @ref gfGrowRel 0x10  For use with @ref TWindow objects that are in the
     *                 desktop. The view will change size relative to the
     *                 owner's size, maintaining that relative size with
     *                 respect to the owner even when screen is resized.
     *
* @ref gfGrowAll 0x0F  If set, the view will move with the lower-right corner
     *                 of its owner.
     * </pre>
     *
     * Note that LoX = left side; LoY = top side; HiX = right side and
     * HiY = bottom side.
     *
     * Example:
     * <pre>
     * growMode = @ref gfGrowLoX | @ref gfGrowLoY;
     * </pre>
     */
    uchar growMode;
    /**
     * Determines how the view should behave when mouse-dragged. The dragMode
     * bits are defined as follows:
     *
     * <pre>
     * Constant   Value Meaning
     *
* @ref dmDragMove 0x01  Allow the view to move
* @ref dmDragGrow 0x02  Allow the view to change size
* @ref dmLimitLoX 0x10  The view's left-hand side cannot move outside limits
* @ref dmLimitLoY 0x20  The view's top side cannot move outside limits
* @ref dmLimitHiX 0x40  The view's right-hand side cannot move outside limits
* @ref dmLimitHiY 0x80  The view's bottom side cannot move outside limits
* @ref dmLimitAll 0xF0  No part of the view can move outside limits
     * </pre>
     *
     * By default, the TView constructor sets the dragMode data member to
     * @ref dmLimitLoY. Currently, these constants and dragMode are only used
     * to compose the `mode' argument of @ref TView::dragView() calls when a
     * view is moved or resized.
     */
    uchar dragMode;
    /**
     * The help context of the view. When the view is focused, this data
     * member will represent the help context of the application, unless the
     * context number is @ref hcNoContext, in which case there is no help
     * context for the view.
     *
     * The following default help context constants are defined:
     *
     * <pre>
     * Constant    Value Meaning
     *
* @ref hcNoContext 0     No context specified
* @ref hcDragging  1     Object is being dragged
     * </pre>
     *
     * The default value of helpCtx is @ref hcNoContext. @ref getHelpCtx()
     * returns @ref hcDragging whenever the view is being dragged (as
     * indicated by the @ref sfDragging @ref state flag).
     *
     * TVision reserves help context values 0 through 999 for its own use.
     * Programmers may define their own constants in the range 1,000 to
     * 65,535.
     */
    ushort helpCtx;
    /**
     * Set to True whenever the view's command set is changed via an enable or
     * disable command call.
     * @see TView::disableCommand
     * @see TView::disableCommands
     * @see TView::enableCommand
     * @see TView::enableCommands
     * @see TView::setCommands
     */
    static Boolean commandSetChanged;
#ifndef GENINC
    /**
     * Holds the set of commands currently enabled for this view. Initially,
     * the following commands are disabled: cmZoom, cmClose, cmResize, cmNext,
     * cmPrev.
     *
     * This data member is constantly monitored by @ref handleEvent() to
     * determine which of the received command events needs to be serviced.
     *
     * curCommandSet should not be altered directly: use the appropriate set,
     * enable, or disable calls.
     * @see TView::disableCommand
     * @see TView::disableCommands
     * @see TView::enableCommand
     * @see TView::enableCommands
     * @see TView::setCommands
     */
    static TCommandSet curCommandSet;
#endif
    /**
     * Points to the TGroup object that owns this view. If 0, the view has
     * no owner. The view is displayed within its owner's view and will be
     * clipped by the owner's bounding rectangle.
     */
    TGroup *owner;
    /**
     * Used to indicate whether indicators should be placed around focused
     * controls. @ref TProgram::initScreen() sets showMarkers to True if the
     * video mode is monochrome; otherwise it is False. The value may,
     * however, be set on in color and black and white modes if desired.
     */
    static Boolean showMarkers;
    /**
     * Attribute used to signal an invalid palette selection. For example,
     * @ref mapColor() returns errorAttr if it is called with an invalid color
     * argument.
     *
     * By default, errorAttr is set to 0xCF, which shows as flashing red on
     * white.
     */
    static uchar errorAttr;
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when destroy() is called.
     */
    virtual void shutDown();
private:
    /**
     * Undocumented.
     */
    void moveGrow( TPoint p,
                   TPoint s,
                   TRect& limits,
                   TPoint minSize,
                   TPoint maxSize,
                   uchar mode
                 );
    /**
     * Undocumented.
     */
    void change( uchar, TPoint delta, TPoint& p, TPoint& s, ulong ctrlState );
    /**
     * Undocumented.
     */
    int exposedRec1(short int, short int, class TView *);
    /**
     * Undocumented.
     */
    int exposedRec2(short int, short int, class TView *);
    /**
     * Undocumented.
     */
    void writeView(short int, short int, short int, const void *);
    /**
     * Undocumented.
     */
    void writeViewRec1(short int, short int, class TView *, int);
    /**
     * Undocumented.
     */
    void writeViewRec2(short int, short int, class TView *, int);
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TView( StreamableInit );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
protected:
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TView& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TView*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TView& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TView* cl )
    { return os << (TStreamable *)cl; }

inline void TView::writeBuf( short x, short y, short w, short h,
                             const TDrawBuffer& b )
{
    writeBuf( x, y, w, h, b.data );
}

inline void TView::writeLine( short x, short y, short w, short h,
                              const TDrawBuffer& b )
{
    writeLine( x, y, w, h, b.data );
}

#endif  // Uses_TView

/* ---------------------------------------------------------------------- */
/*      class TFrame                                                      */
/*                                                                        */
/*      Palette layout                                                    */
/*        1 = Passive frame                                               */
/*        2 = Passive title                                               */
/*        3 = Active frame                                                */
/*        4 = Active title                                                */
/*        5 = Icons                                                       */
/* ---------------------------------------------------------------------- */

#if defined( Uses_TFrame ) && !defined( __TFrame )
#define __TFrame

class TRect;
class TEvent;
class TDrawBuffer;

/**
 * TFrame provides the distinctive frames around windows and dialog boxes.
 * Users will probably never need to deal with frame objects directly, as they
 * are added to window objects by default.
 * @short The frame around the windows
 */
class TFrame : public TView
{
public:
    /**
     * Calls TView constructor TView(bounds), then sets @ref growMode to
     * @ref gfGrowHiX | @ref gfGrowHiY and sets @ref eventMask to
     * @ref eventMask | @ref evBroadcast, so TFrame objects default to
     * handling broadcast events.
     * `bounds' is the bounding rectangle of the frame.
     */
    TFrame( const TRect& bounds );
    /**
     * Draws the frame with color attributes and icons appropriate to the
     * current state flags: active, inactive, being dragged. Adds zoom, close
     * and resize icons depending on the owner window's flags. Adds the title,
     * if any, from the owning window's title data member.
     *
     * Active windows are drawn with a double-lined frame and any icons;
     * inactive windows are drawn with a single-lined frame and no icons.
     * @see TView::draw
     */ 
    virtual void draw();
    /**
     * Returns a reference to the default frame palette string.
     * @see TView::getPalette
     */
    virtual TPalette& getPalette() const;
    /**
     * Calls @ref TView::handleEvent(), then handles mouse events.
     *
     * If the mouse is clicked on the close icon, TFrame::handleEvent()
     * generates a cmClose event. Clicking on the zoom icon or double-clicking
     * on the top line of the frame generates a cmZoom event.
     *
     * Dragging the top line of the frame moves the window, and dragging the
     * resize icon moves the lower right corner of the view and therefore
     * changes its size.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Changes the state of the frame.
     * Calls TView::setState(aState, enable). If the new state is
     * @ref sfActive or @ref sfDragging, calls @ref TView::drawView() to
     * redraw the view.
     * @see TView::setState
     * @see TView::state
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Undocumented.
     */
    static char frameChars[33];
    /**
     * The character showed in the close box.
     */
    static const char * closeIcon;
    /**
     * The character showed in the lower right corner of the screen.
     */
    static const char * dragIcon;
private:
    /**
     * Undocumented.
     */
    void frameLine( TDrawBuffer& frameBuf, short y, short n, uchar color );
    /**
     * Undocumented.
     */
    void dragWindow( TEvent& event, uchar dragMode );
    /**
     * Undocumented.
     */
    friend class TDisplay;
    /**
     * Undocumented.
     */
    static const char initFrame[19];
    /**
     * Undocumented.
     */
    static const char * zoomIcon;
    /**
     * Undocumented.
     */
    static const char * unZoomIcon;
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Used to recover the view from a stream.
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized vtable pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TFrame( StreamableInit );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Used to recover the view from a stream.
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFrame& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFrame*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFrame& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFrame* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFrame

/* ---------------------------------------------------------------------- */
/*      class TScrollBar                                                  */
/*                                                                        */
/*      Palette layout                                                    */
/*        1 = Page areas                                                  */
/*        2 = Arrows                                                      */
/*        3 = Indicator                                                   */
/* ---------------------------------------------------------------------- */

#if defined( Uses_TScrollBar ) && !defined( __TScrollBar )
#define __TScrollBar

class TRect;
class TEvent;

/**
 * An array representing the characters used to draw a TScrollBar.
 * @see TScrollBar
 * @see TScrollBar::draw
 */
typedef char TScrollChars[5];

/**
 * @short Implements a scroll bar
 */
class TScrollBar : public TView
{
public:
    /**
     * Creates and initializes a scroll bar with the given bounds by calling
     * the TView constructor. Sets @ref value, @ref maxVal and @ref minVal to
     * zero. Sets @ref pgStep and @ref arStep to 1.
     *
     * The shapes of the scroll bar parts are set to the defaults in
     * @ref chars data member.
     *
     * If `bounds' produces size.x = 1, scroll bar is vertical; otherwise, it
     * is horizontal. Vertical scroll bars have the @ref growMode data member
     * set to @ref gfGrowLoX | @ref gfGrowHiX | @ref gfGrowHiY; horizontal
     * scroll bars have the @ref growMode data member set to @ref gfGrowLoY |
     * @ref gfGrowHiX | @ref gfGrowHiY.
     */
    TScrollBar( const TRect& bounds );
    /**
     * Draws the scroll bar depending on the current bounds, value, and
     * palette.
     */
    virtual void draw();
    /**
     * Returns the default palette.
     */
    virtual TPalette& getPalette() const;
    /**
     * Handles scroll bar events by calling @ref TView::handleEvent(). Mouse
     * events are broadcast to the scroll bar's owner, which must handle the
     * implications of the scroll bar changes.
     *
     * handleEvent() also determines which scroll bar part has received a
     * mouse click (or equivalent keystroke). Data member @ref value is
     * adjusted according to the current @ref arStep or @ref pgStep values.
     * The scroll bar indicator is redrawn.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Is called whenever @ref value data member changes. This virtual member
     * function defaults by sending a cmScrollBarChanged message to the scroll
     * bar's owner:
     *
     * <pre>
     * message(owner, @ref evBroadcast, cmScrollBarChanged, this);
     * </pre>
     * @see message
     */
    virtual void scrollDraw();
    /**
     * By default, scrollStep() returns a positive or negative step value,
     * depending on the scroll bar part given by `part', and the current
     * values of @ref arStep and @ref pgStep. Parameter `part' should be one
     * of the following constants:
     *
     * <pre>
     * Constant     Value Meaning
     *
* @ref sbLeftArrow  0     Left arrow of horizontal scroll bar
* @ref sbRightArrow 1     Right arrow of horizontal scroll bar
* @ref sbPageLeft   2     Left paging area of horizontal scroll bar
* @ref sbPageRight  3     Right paging area of horizontal scroll bar
* @ref sbUpArrow    4     Top arrow of vertical scroll bar
* @ref sbDownArrow  5     Bottom arrow of vertical scroll bar
* @ref sbPageUp     6     Upper paging area of vertical scroll bar
* @ref sbPageDown   7     Lower paging area of vertical scroll bar
* @ref sbIndicator  8     Position indicator on scroll bar
     * </pre>
     *
     * These constants define the different areas of a TScrollBar in which the
     * mouse can be clicked. The scrollStep() function converts these
     * constants into actual scroll step values.
     * Although defined, the sbIndicator constant is never passed to
     * scrollStep().
     */
    virtual int scrollStep( int part );
    /**
     * Sets the @ref value, @ref minVal, @ref maxVal, @ref pgStep and
     * @ref arStep with the given argument values. Some adjustments are made
     * if your arguments conflict.
     *
     * The scroll bar is redrawn by calling @ref drawView(). If value is
     * changed, @ref scrollDraw() is also called.
     */
    void setParams( int aValue, int aMin, int aMax,
                    int aPgStep, int aArStep );
    /**
     * Sets the legal range for value by setting @ref minVal and @ref maxVal
     * to the given arguments `aMin' and `aMax'.
     *
     * Calls @ref setParams(), so @ref drawView() and @ref scrollDraw() will
     * be called if the changes require the scroll bar to be redrawn.
     */
    void setRange( int aMin, int aMax );
    /**
     * Sets @ref pgStep and @ref arStep to the given arguments `aPgStep' and
     * `aArStep'.
     * Calls @ref setParams() with the other arguments set to their current
     * values.
     */
    void setStep( int aPgStep, int aArStep );
    /**
     * Sets @ref value to `aValue' by calling @ref setParams() with the other
     * arguments set to their current values.
     * Note: @ref drawView() and @ref scrollDraw() will be called if this
     * call changes value.
     */
    void setValue( int aValue );
    /**
     * Undocumented.
     */
    void drawPos( int pos );
    /**
     * Undocumented.
     */
    int getPos();
    /**
     * Undocumented.
     */
    int getSize();
    /**
     * This variable represents the current position of the scroll bar
     * indicator. This marker moves along the scroll bar strip to indicate the
     * relative position of the scrollable text being viewed relative to the
     * total text available for scrolling.
     *
     * The TScrollBar constructor sets value to zero by default.
     */
    int value;
    /**
     * TScrollChars is defined as:
     *
     * <pre>
     * typedef char TScrollChars[5];
     * </pre>
     *
     * Variable chars is set with the five basic character patterns used to
     * draw the scroll bar parts.
     */
    TScrollChars chars;
    /**
     * Variable minVal represents the minimum value for the @ref value data
     * member. The TScrollBar constructor sets minVal to zero by default.
     */
    int minVal;
    /**
     * Variable maxVal represents the maximum value for the @ref value data
     * member. The TScrollBar constructor sets maxVal to zero by default.
     */
    int maxVal;
    /**
     * Variable pgStep is the amount added or subtracted to the scroll bar's
     * @ref value data member when a mouse click event occurs in any of the
     * page areas (@ref sbPageLeft, @ref sbPageRight, @ref sbPageUp, or
     * @ref sbPageDown) or an equivalent keystroke is detected (Ctrl-Left,
     * Ctrl-Right, PgUp, or PgDn).
     *
     * The TScrollBar constructor sets pgStep to 1 by default. You can change
     * pgStep using @ref setParams(), @ref setStep() or
     * @ref TScroller::setLimit().
     */
    int pgStep;
    /**
     * Variable arStep is the amount added or subtracted to the scroll bar's
     * @ref value data member when an arrow area is clicked (@ref sbLeftArrow,
     * @ref sbRightArrow, @ref sbUpArrow, or @ref sbDownArrow) or the
     * equivalent keystroke made.
     *
     * The TScrollBar constructor sets arStep to 1 by default.
     */
    int arStep;
    /**
     * Undocumented.
     */
    static TScrollChars vChars;
    /**
     * Undocumented.
     */
    static TScrollChars hChars;
private:
    /**
     * Undocumented.
     */
    int getPartCode(void);
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TScrollBar( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TScrollBar& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TScrollBar*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TScrollBar& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TScrollBar* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TScrollBar

/* ---------------------------------------------------------------------- */
/*      class TScroller                                                   */
/*                                                                        */
/*      Palette layout                                                    */
/*      1 = Normal text                                                   */
/*      2 = Selected text                                                 */
/* ---------------------------------------------------------------------- */

#if defined( Uses_TScroller ) && !defined( __TScroller )
#define __TScroller

class TRect;
class TScrollBar;
class TEvent;

/**
 * TScroller provides a scrolling virtual window onto a larger view. That is,
 * a scrolling view lets the user scroll a large view within a clipped
 * boundary.
 *
 * The scroller provides an offset from which the @ref TView::draw() method
 * fills the visible region. All methods needed to provide both scroll bar
 * and keyboard scrolling are built into TScroller.
 *
 * The basic scrolling view provides a useful starting point for scrolling
 * views such as text views.
 * @short Provides a scrolling virtual window onto a larger view
 */
class TScroller : public TView
{
public:
    /**
     * Creates and initializes a TScroller object with the given size and
     * scroll bars. Calls @ref TView constructor to set the view's size.
     *
     * `aHScrollBar' should be 0 if you do not want a horizontal scroll bar;
     * `aVScrollBar' should be 0 if you do not want a vertical scroll bar.
     */
    TScroller( const TRect& bounds,
               TScrollBar *aHScrollBar,
               TScrollBar *aVScrollBar
             );
    /**
     * Changes the scroller's size by calling @ref TView::setbounds(). If
     * necessary, the scroller and scroll bars are then redrawn by calling
     * @ref setLimit() and @ref drawView().
     */
    virtual void changeBounds( const TRect& bounds );
    /**
     * Returns the default scroller palette string.
     */
    virtual TPalette& getPalette() const;
    /**
     * Handles most events by calling @ref TView::handleEvent().
     *
     * Broadcast events such as cmScrollBarChanged from either @ref hScrollBar
     * or @ref vScrollBar result in a call to @ref scrollDraw().
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Checks to see if @ref delta matches the current positions of the scroll
     * bars. If not, @ref delta is set to the correct value and
     * @ref drawView() is called to redraw the scroller.
     */
    virtual void scrollDraw();
    /**
     * Sets the scroll bars to (x,y) by calling hScrollBar->setValue(x) and
     * vScrollBar->setValue(y) and redraws the view by calling @ref drawView().
     * @see TScrollBar::hScrollBar
     * @see TScrollBar::vScrollBar
     * @see TScrollBar::setValue
     */
    void scrollTo( int x, int y );
    /**
     * Sets the @ref limit data member and redraws the scrollbars and
     * scroller if necessary.
     */
    void setLimit( int x, int y );
    /**
     * This member function is called whenever the scroller's state changes.
     * Calls @ref TView::setState() to set or clear the state flags in
     * `aState'.
     * If the new @ref state is @ref sfSelected and @ref sfActive, setState()
     * displays the scroll bars; otherwise, they are hidden.
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * If @ref drawLock is zero and @ref drawFlag is True, @ref drawFlag is set
     * False and @ref drawView() is called.
     * If @ref drawLock is non-zero or @ref drawFlag is False, checkDraw()
     * does nothing.
     *
     * Methods @ref scrollTo() and @ref setLimit() each call checkDraw() so
     * that @ref drawView() is only called if needed.
     */
    void checkDraw();
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when @ref destroy() is called.
     */
    virtual void shutDown();
    /**
     * Holds the x (horizontal) and y (vertical) components of the scroller's
     * position relative to the virtual view being scrolled.
     *
     * Automatic scrolling is achieved by changing either or both of these
     * components in response to scroll bar events that change the value data
     * member(s).
     *
     * Manual scrolling changes delta, triggers changes in the scroll bar
     * @ref TScrollBar::value data members, and leads to updating of the
     * scroll bar indicators.
     */
    TPoint delta;
protected:
    /**
     * A semaphore used to control the redrawing of scrollers.
     */
    uchar drawLock;
    /**
     * Set True if the scroller has to be redrawn.
     */
    Boolean drawFlag;
    /**
     * Points to the horizontal scroll bar object associated with the
     * scroller. If there is no such scroll bar, hScrollBar is 0.
     */
    TScrollBar *hScrollBar;
    /**
     * Points to the vertical scroll bar object associated with the
     * scroller. If there is no such scroll bar, vScrollBar is 0.
     */
    TScrollBar *vScrollBar;
    /**
     * Data members limit.x and limit.y are the maximum allowed values for
     * delta.x and delta.y data members.
     * @see TScroller::delta
     */
    TPoint limit;
private:
    /**
     * Undocumented.
     */
    void showSBar( TScrollBar *sBar );
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TScroller( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TScroller& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TScroller*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TScroller& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TScroller* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TScroller

#if defined( Uses_TListViewer ) && !defined( __TListViewer )
#define __TListViewer

class TRect;
class TScrollBar;
class TEvent;

/**
 * TListViewer is an abstract class from which you can derive list viewers of
 * various kinds, such as @ref TListBox. TListViewer's members offer the
 * following functionality:
 *
 * -# A view for displaying linked lists of items (but no list)
 * -# Control over one or two scroll bars
 * -# Basic scrolling of lists in two dimensions
 * -# Reading and writing the view and its scroll bars from and to a stream
 * -# Ability to use a mouse or the keyboard to select (highlight) items on
 *    list
 * -# Draw member function that copes with resizing and scrolling
 *
 * TListViewer has an abstract @ref getText() method, so you need to supply
 * the mechanism for creating and manipulating the text of the items to be
 * displayed.
 *
 * TListViewer has no list storage mechanism of its own. Use it to display
 * scrollable lists of arrays, linked lists, or similar data structures. You
 * can also use its descendants, such as @ref TListBox, which associates a
 * collection with a list viewer.
 * @short An abstract class from which you can derive list viewers of various
 * kinds, such as TListBox.
 */
class TListViewer : public TView
{
    static const char * emptyText;
public:
    /**
     * Creates and initializes a TListViewer object with the given size by
     * first calling @ref TView::TView(bounds).
     *
     * The @ref numCols data member is set to `aNumCols'. @ref TView::options
     * is set to (@ref ofFirstClick | @ref ofSelectable) so that mouse clicks
     * that select this view will be passed first to @ref handleEvent().
     *
     * The @ref TView::eventMask is set to @ref evBroadcast. The initial
     * values of @ref range and @ref focused are zero.
     *
     * You can supply pointers to vertical and/or horizontal scroll bars by
     * way of the `aVScrollBar' and `aHScrollBar' arguments. Setting either or
     * both to 0 suppresses one or both scroll bars. These two pointer
     * arguments are assigned to the @ref vScrollBar and @ref hScrollBar data
     * members.
     */
    TListViewer( const TRect& bounds,
                 ushort aNumCols,
                 TScrollBar *aHScrollBar,
                 TScrollBar *aVScrollBar
               );
    /**
     * Changes the size of the TListViewer object by calling
     * TView::changeBounds(bounds). If a horizontal scroll bar has been
     * assigned, @ref TScrollBar::pgStep is updated by way of
     * @ref TScrollBar::setStep().
     * @see TView::changeBounds
     */
    virtual void changeBounds( const TRect& bounds );
    /**
     * Draws the TListViewer object with the default palette by repeatedly
     * calling @ref getText() for each visible item. Takes into account the
     * @ref focused and selected items and whether the view is @ref sfActive.
     * @see TView::state
     */
    virtual void draw();
    /**
     * Makes the given item focused by setting the @ref focused data member to
     * `item'. Also sets the @ref TScrollBar::value data member of the
     * vertical scroll bar (if any) to `item' and adjusts @ref topItem.
     */
    virtual void focusItem( short item );
    /**
     * Returns the default TListViewer palette string.
     */
    virtual TPalette& getPalette() const;
    /**
     * Derived classes must override it with a function that writes a string
     * not exceeding `maxLen' at address `dest', given an item index
     * referenced by `item'.
     *
     * Note that @ref draw() needs to call getText().
     */
    virtual void getText( char *dest, short item, short maxLen );
    /**
     * Returns True if the given item is selected (focused), that is, if
     * `item' == @ref focused.
     */
    virtual Boolean isSelected( short item );
    /**
     * Handles events by first calling TView::handleEvent(event).
     * @see TView::handleEvent
     *
     * Mouse clicks and "auto" movements over the list will change the focused
     * item. Items can be selected with double mouse clicks.
     *
     * Keyboard events are handled as follows: Spacebar selects the currently
     * focused item; the arrow keys, PgUp, PgDn, Ctrl-PgDn, Ctrl-PgUp, Home,
     * and End keys are tracked to set the focused item.
     *
     * Broadcast events from the scroll bars are handled by changing the
     * @ref focused item and redrawing the view as required.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Selects the item'th element of the list, then broadcasts this fact to
     * the owning group by calling:
     *
     * <pre>
     * message(owner, @ref evBroadcast, cmListItemSelected, this);
     * </pre>
     * @see message
     */
    virtual void selectItem( short item );
    /**
     * Sets the @ref range data member to `aRange'.
     *
     * If a vertical scroll bar has been assigned, its parameters are adjusted
     * as necessary (and @ref TScrollBar::drawView() is invoked if redrawing is
     * needed).
     *
     * If the currently focused item falls outside the new range, the
     * @ref focused data member is set to zero.
     */
    void setRange( short aRange );
    /**
     * Calls TView::setState(aState, enable) to change the TListViewer
     * object's state. Depending on the `aState' argument, this can result in
     * displaying or hiding the view.
     * @see TView::setState
     * @see TView::state
     *
     * Additionally, if `aState' is @ref sfSelected and @ref sfActive, the
     * scroll bars are redrawn; if `aState' is @ref sfSelected but not
     * @ref sfActive, the scroll bars are hidden.
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Used internally by @ref focusItem(). Makes the given item focused by
     * setting the @ref focused data member to `item'.
     */
    virtual void focusItemNum( short item );
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when @ref destroy() is called.
     */
    virtual void shutDown();
    /**
     * Pointer to the horizontal scroll bar associated with this view. If 0,
     * the view does not have such a scroll bar.
     */
    TScrollBar *hScrollBar;
    /**
     * Pointer to the vertical scroll bar associated with this view. If 0,
     * the view does not have such a scroll bar.
     */
    TScrollBar *vScrollBar;
    /**
     * The number of columns in the list control.
     */
    short numCols;
    /**
     * The item number of the top item to be displayed. This value changes
     * during scrolling. Items are numbered from 0 to @ref range - 1. This
     * number depends on the number of columns, the size of the view, and the
     * value of variable @ref range.
     */
    short topItem;
    /**
     * The item number of the focused item. Items are numbered from 0 to
     * @ref range - 1. Initially set to 0, the first item, focused can be
     * changed by mouse click or Spacebar selection.
     */
    short focused;
    /**
     * The current total number of items in the list. Items are numbered from
     * 0 to range - 1.
     */
    short range;
    /**
     * Undocumented.
     */
    static char separatorChar;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TListViewer( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TListViewer& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TListViewer*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TListViewer& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TListViewer* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TListViewer

#if defined( Uses_TGroup ) && !defined( __TGroup )
#define __TGroup

class TView;

/**
 * TGroup objects and their derivatives (called groups for short) provide the
 * central driving power to TVision.
 *
 * A group is a special breed of view. In addition to all the members derived
 * from @ref TView and @ref TStreamable, a group has additional members and
 * many overrides that allow it to control a dynamically linked list of views
 * (including other groups) as though they were a single object.
 *
 * We often talk about the subviews of a group even when these subviews are
 * often groups in their own right.
 *
 * Although a group has a rectangular boundary from its @ref TView ancestry, a
 * group is only visible through the displays of its subviews. A group draws
 * itself via the @ref draw() methods of its subviews. A group owns its
 * subviews, and together they must be capable of drawing (filling) the
 * group's entire rectangular bounds.
 * @see TView::draw
 *
 * During the life of an application, subviews are created, inserted into
 * groups, and displayed as a result of user activity and events generated by
 * the application itself. The subviews can just as easily be hidden, deleted
 * from the group, or disposed of by user actions (such as closing a window or
 * quitting a dialog box).
 *
 * Three derived object types of TGroup, namely @ref TWindow, @ref TDeskTop,
 * and @ref TApplication (via @ref TProgram) illustrate the group and
 * subgroup concept. The application typically owns a desktop object, a
 * status line object, and a menu view object. @ref TDeskTop is a TGroup
 * derivative, so it, in turn, can own @ref TWindow objects, which in turn own
 * @ref TFrame objects, @ref TScrollBar objects, and so on.
 *
 * TGroup overrides many of the basic @ref TView methods in a natural way.
 * TGroup objects delegate both drawing and event handling to their subviews.
 * You'll rarely construct an instance of TGroup itself; rather you'll
 * usually use one or more of TGroup's derived object types:
 * @ref TApplication, @ref TDeskTop, and @ref TWindow.
 *
 * All TGroup objects are streamable, inheriting from @ref TStreamable by way
 * of @ref TView. This means that TGroup objects (including your entire
 * application group) can be written to and read from streams in a type-safe
 * manner using the familiar C++ iostream operators.
 * @short TGroup objects and their derivatives provide the central driving
 * power to TVision
 */
class TGroup : public TView
{
public:
    /**
     * Calls @ref TView::TView(bounds), sets @ref ofSelectable and
     * @ref ofBuffered in @ref options and sets @ref eventMask to 0xFFFF. The
     * members @ref last, @ref current, @ref buffer, @ref lockFlag and
     * @ref endState are all set to zero.
     */
    TGroup( const TRect& bounds );
    /**
     * Hides the group using @ref hide(), then disposes of each subview in
     * the group using delete.
     * Finally, the buffer is freed (if one).
     */
    ~TGroup();
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects.
     *
     * shutDown() is overridden in many classes to ensure the proper setting
     * of related data members when destroy is called.
     */
    virtual void shutDown();
    /**
     * execView() is the "modal" counterpart of the "modeless" @ref insert()
     * and @ref remove() member functions.
     *
     * Unlike @ref insert(), after inserting a view into the group, execView()
     * waits for the view to execute, then removes the view, and finally
     * returns the result of the execution.
     *
     * execView() is used in a number of places throughout TVision, most
     * notably to implement @ref TProgram::run() and to execute modal dialog
     * boxes.
     *
     * execView() saves the current context (the selected view, the modal
     * view, and the command set), makes `p' modal by calling
     * p->setState(sfModal, True), inserts `p' into the group (if it isn't
     * already inserted), and calls p->execute().
     * @see TView::execute
     * @see TView::setState
     *
     * When p->execute() returns, the group is restored to its previous state,
     * and the result of p->execute() is returned as the result of the
     * execView() call.
     *
     * If `p' is 0 upon a call to execView(), a value of cmCancel is returned.
     */
    ushort execView( TView *p );
    /**
     * Overrides @ref TView::execute(). execute() is a group's main event
     * loop: it repeatedly gets events using @ref getEvent() and handles
     * them using @ref handleEvent().
     *
     * The event loop is terminated by the group or some subview through a
     * call to @ref endModal(). Before returning, however, execute() calls
     * @ref valid() to verify that the modal state can indeed be terminated.
     */
    virtual ushort execute();
    /**
     * Calls the @ref TView::awaken() methods of each of the group's subviews
     * in Z-order.
     */
    virtual void awaken();
    /**
     * Undocumented.
     */
    void insertView( TView *p, TView *Target );
    /**
     * Removes the subview `p' from the group and redraws the other subviews
     * as required. p's owner and next members are set to 0.
     */
    void remove( TView *p );
    /**
     * Removes the subview `p' from this group. Used internally by
     * @ref remove().
     */
    void removeView( TView *p );
    /**
     * Selects (makes current) the first subview in the chain that is
     * @ref sfVisible and @ref ofSelectable. resetCurrent() works by calling
     * setCurrent(firstMatch(sfVisible, ofSelectable), normalSelect).
     * @see TGroup::firstMatch
     * @see TGroup::setCurrent
     * @see TView::options
     * @see TView::state
     *
     * The following enum type is useful for select mode arguments:
     *
     * <pre>
     * enum selectMode { normalSelect, enterSelect, leaveSelect };
     * </pre>
     */
    void resetCurrent();
    /**
     * Parameter `selectMode' is an enumeration defined in TGroup as follows:
     *
     * <pre>
     * enum selectMode {normalSelect, enterSelect, leaveSelect};
     * </pre>
     *
     * If `p' is the current subview, setCurrent() does nothing. Otherwise,
     * `p' is made current (that is, selected) by a call to @ref setState().
     * @see resetCurrent
     */
    void setCurrent( TView *p, selectMode mode );
    /**
     * If `forwards' is True, selectNext() selects (makes current) the next
     * selectable subview (one with its ofSelectable bit set) in the group's
     * Z-order.
     * If `forwards' is False, the member function selects the previous
     * selectable subview.
     */
    void selectNext( Boolean forwards );
    /**
     * firstThat() applies a user-supplied @ref Boolean function `func',
     * along with an argument list given by `args' (possibly empty), to each
     * subview in the group (in Z-order) until `func' returns True.
     *
     * The returned result is the subview pointer for which `func' returns
     * True, or 0 if `func' returns False for all items.
     *
     * The first pointer argument of `func' scans the subview. The second
     * argument of `func' is set from the `args' pointer of firstThat(), as
     * shown in the following implementation:
     *
     * <pre>
     * TView *TGroup::firstThat(Boolean (*func)(TView *, void *), void *args)
     * {
     *     TView *temp = last;
     *
     *     if (temp == 0) return 0;
     *     do {
     *         temp = temp->next;
     *         if (func(temp, args) == True)
     *         return temp;
     *     } while(temp != last);
     *     return 0;
     * }
     * </pre>
     */
    TView *firstThat( Boolean (*func)( TView *, void * ), void *args );
    /**
     * Undocumented.
     */
    Boolean focusNext(Boolean forwards);
    /**
     * forEach() applies an action, given by the function `func', to each
     * subview in the group in Z-order. The `args' argument lets you pass
     * arbitrary arguments to the action function:
     *
     * <pre>
     * void TGroup::forEach(void (*func)(TView*, void *), void *args)
     * {
     *     TView *term = last;
     *     TView *temp = last;
     *
     *     if (temp == 0) return;
     *
     *     TView *next = temp->next;
     *     do  {
     *         temp = next;
     *         next = temp->next;
     *         func(temp, args);
     *     } while(temp != term);
     * }
     * </pre>
     */
    void forEach( void (*func)( TView *, void * ), void *args );
    /**
     * Inserts the view given by `p' in the group's subview list. The new
     * subview is placed on top of all other subviews. If the subview has the
     * @ref ofCenterX and/or @ref ofCenterY flags set, it is centered
     * accordingly in the group.
     * @see TView::options
     *
     * If the view has the @ref sfVisible flag set, it will be shown in the
     * group. Otherwise it remains invisible until specifically shown. If the
     * view has the @ref ofSelectable flag set, it becomes the currently
     * selected subview.
     * @see TView::state
     */
    void insert( TView *p );
    /**
     * Inserts the view given by `p' in front of the view given by `Target'.
     * If `Target' is 0, the view is placed behind all other subviews in the
     * group.
     */
    void insertBefore( TView *p, TView *Target );
    /**
     * Points to the subview that is currently selected, or is 0 if no
     * subview is selected.
     */
    TView *current;
    /**
     * Returns a pointer to the subview at `index' position in Z-order.
     */
    TView *at( short index );
    /**
     * Returns a pointer to the first subview that matches its state with
     * `aState' and its options with `aOptions'.
     */
    TView *firstMatch( ushort aState, ushort aOptions );
    /**
     * Returns the Z-order position (index) of the subview `p'.
     */
    short indexOf( TView *p );
    /**
     * Returns True if the state and options settings of the view `p' are
     * identical to those of the calling view.
     */
    Boolean matches( TView *p );
    /**
     * Returns a pointer to the first subview (the one closest to the top in
     * Z-order), or 0 if the group has no subviews.
     */
    TView *first();
    /**
     * Overrides @ref TView::setState(). First calls the inherited
     * TView::setState(), then updates the subviews as follows (see
     * @ref state for more):
     *
     * -# If `aState' is @ref sfActive or @ref sfDragging, then each subview's
     *    setState() is called to update the subview correspondingly.
     * -# If `aState' is @ref sfFocused, then the currently selected subview is
     *    called to focus itself correspondingly.
     * -# If `aState' is @ref sfExposed, @ref TGroup::doExpose() is called for
     *    each subview. Finally, if `enable' is False, @ref freeBuffer() is
     *    called.
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Overrides @ref TView::handleEvent(). A group basically handles events by
     * passing them to the handleEvent() member functions of one or more of
     * its subviews. The actual routing, however, depends on the event class.
     *
     * For focused events (by default, @ref evKeyDown and @ref evCommand),
     * event handling is done in three phases (see @ref phase for more):
     *
     * -# The group's phase member is set to phPreProcess and the event
     *    is passed to the handleEvent() of all subviews that have the
     *    @ref ofPreProcess flag set in @ref options.
     * -# Phase is set to phFocused and the event is passed to the
     *    handleEvent() of the currently selected view.
     * -# Phase is set to phPostProcess and the event is passed to the
     *    handleEvent() of all subviews that have the @ref ofPostProcess flag
     *    set in @ref options.
     *
     * For positional events (by default, @ref evMouse), the event is passed
     * to the handleEvent() of the first subview whose bounding rectangle
     * contains the point given by `event.where'.
     *
     * For broadcast events (events that aren't focused or positional), the
     * event is passed to the handleEvent() of each subview in the group in
     * Z-order.
     *
     * If a subview's @ref eventMask member masks out an event class,
     * TGroup::handleEvent() will never send events of that class to the
     * subview. For example, the @ref TView::eventMask disables
     * @ref evMouseUp, @ref evMouseMove, and @ref evMouseAuto, so
     * TGroup::handleEvent() will never send such events to a standard TView.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Calls @ref TView::drawView() for each subview starting at `p', until
     * the subview `bottom' is reached.
     */
    void drawSubViews( TView *p, TView *bottom );
    /**
     * Overrides @ref TView::changeBounds(). Changes the group's bounds to
     * `bounds' and then calls @ref calcBounds() followed by
     * @ref TView::changeBounds() for each subview in the group.
     */
    virtual void changeBounds( const TRect& bounds );
    /**
     * Overrides @ref TView::dataSize(). Returns total size of group by
     * calling and accumulating dataSize() for each subview.
     * @see TGroup::getData
     * @see TGroup::setData
     */
    virtual ushort dataSize();
    /**
     * Overrides @ref TView::getData(). Calls getData() for each subview in
     * reverse order, incrementing the location given by `rec' by the
     * dataSize() of each subview.
     * @see TGroup::dataSize
     * @see TGroup::setData
     */
    virtual void getData( void *rec );
    /**
     * Overrides @ref TView::setData(). Calls setData() for each subview in
     * reverse Z-order, incrementing the location given by `rec' by the
     * dataSize() of each subview.
     * @see TGroup::dataSize
     * @see TGroup::getData
     */
    virtual void setData( void *rec );
    /**
     * Overrides @ref TView::draw(). If a cache buffer exists (see
     * @ref TGroup::buffer data member), then the buffer is written to the
     * screen using @ref TView::writeBuf().
     *
     * Otherwise, each subview is told to draw itself using a call to
     * @ref TGroup::redraw().
     */ 
    virtual void draw();
    /**
     * Redraws the group's subviews in Z-order. TGroup::redraw() differs from
     * @ref TGroup::draw() in that redraw() will never draw from the cache
     * buffer.
     */
    void redraw();
    /**
     * Locks the group, delaying any screen writes by subviews until the group
     * is unlocked.
     *
     * lock() has no effect unless the group has a cache buffer (see
     * @ref ofBuffered flag and @ref buffer data member). lock() works by
     * incrementing the data member @ref lockFlag. This semaphore is likewise
     * decremented by @ref unlock().
     * @see TView::options
     *
     * When a call to unlock() decrements the count to zero, the entire group
     * is written to the screen using the image constructed in the cache
     * buffer.
     *
     * By "sandwiching" draw-intensive operations between calls to lock() and
     * @ref unlock(), unpleasant "screen flicker" can be reduced, if not
     * eliminated. lock() and @ref unlock() calls must be balanced;
     * otherwise, a group may end up in a permanently locked state, causing
     * it to not redraw itself properly when so requested.
     */
    void lock();
    /**
     * Unlocks the group by decrementing @ref lockFlag. If @ref lockFlag
     * becomes zero, then the entire group is written to the screen using the
     * image constructed in the cache @ref buffer.
     * @see TGroup::lock
     */
    void unlock();
    /**
     * Undocumented.
     */
    virtual void resetCursor();
    /**
     * If this group is the current modal view, endModal() terminates its
     * modal state.
     *
     * Parameter `command' is passed to @ref TGroup::execView() (which made
     * this view modal in the first place), which returns `command' as its
     * result. If this group is not the current modal view, it calls
     * @ref TView::endModal().
     */
    virtual void endModal( ushort command );
    /**
     * Is called whenever the modal @ref TGroup::execute() event-handling loop
     * encounters an event that cannot be handled.
     *
     * The default action is: if the group's @ref owner is nonzero,
     * eventError() calls its owner's eventError(). Normally this chains back
     * to @ref TApplication's eventError(). You can override eventError() to
     * trigger appropriate action.
     * @see TProgram::eventError
     */
    virtual void eventError( TEvent& event );
    /**
     * Returns the help context of the current focused view by calling the
     * selected subviews' @ref TView::getHelpCtx() member function.
     *
     * If no help context is specified by any subview, getHelpCtx() returns
     * the value of its own @ref helpCtx member, by calling
     * TView::getHelpCtx().
     */
    virtual ushort getHelpCtx();
    /**
     * Overrides @ref TView::valid(). Returns True if all the subview's
     * valid() calls return True. TGroup::valid() is used at the end of the
     * event-handling loop in @ref execute() to confirm that termination is
     * allowed.
     *
     * A modal state cannot terminate until all valid() calls return True. A
     * subview can return False if it wants to retain control.
     */
    virtual Boolean valid( ushort command );
    /**
     * Frees the group's draw buffer (if one exists) by calling delete buffer
     * and setting @ref buffer to 0.
     */
    void freeBuffer();
    /**
     * If the group is @ref sfExposed and @ref ofBuffered, a draw buffer is
     * created. The buffer size will be (size.x * size.y) and the
     * @ref buffer data member is set to point at the new buffer.
     * @see TView::options
     * @see TView::state
     */
    void getBuffer();
    /**
     * Points to the last subview in the group (the one furthest from the top
     * in Z-order).
     */
    TView *last;
    /**
     * Holds the clip extent of the group, as returned by
     * @ref TView::getExtent() or @ref TView::getClipRect(). The clip extent
     * is defined as the minimum area that needs redrawing when @ref draw() is
     * called.
     */
    TRect clip;
    /**
     * The current phase of processing for a focused event. Subviews that have
     * the @ref ofPreProcess or @ref ofPostProcess flags set can examine
     * owner->phase to determine whether a call to their
     * @ref TView::handleEvent() is happening in the phPreProcess,
     * phFocused, or phPostProcess phase.
     * @see TView::options
     *
     * phaseType is an enumeration defined as follows in TView:
     *
     * <pre>
     * enum phaseType {phFocussed, phPreProcess, phPostProcess};
     * </pre>
     */
    phaseType phase;
    /**
     * Points to a buffer used to cache redraw operations, or is 0 if the
     * group has no cache buffer. Cache buffers are created and destroyed
     * automatically, unless the ofBuffered flag is cleared in the group's
     * @ref options member.
     */
    ushort *buffer;
    /**
     * Acts as a semaphore to control buffered group draw operations. lockFlag
     * keeps a count of the number of locks set during a set of nested draw
     * calls.
     *
     * @ref lock() and @ref unlock() increment and decrement this value. When
     * it reaches zero, the whole group will draw itself from its buffer.
     *
     * Intensive @ref TGroup::draw() operations should be sandwiched between
     * calls to @ref lock() and @ref unlock() to prevent excessive CPU load.
     */
    uchar lockFlag;
    /**
     * Holds the state of the group after a call to @ref endModal().
     */
    ushort endState;
private:
    /**
     * Undocumented.
     */
    Boolean invalid( TView *p, ushort command );
    /**
     * Undocumented.
     */
    void focusView( TView *p, Boolean enable );
    /**
     * Undocumented.
     */
    void selectView( TView *p, Boolean enable );
    /**
     * Undocumented.
     */
    TView* findNext(Boolean forwards);
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TGroup( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TGroup& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TGroup*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TGroup& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TGroup* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TGroup

#if defined( Uses_TWindow ) && !defined( __TWindow )
#define __TWindow

#define cpBlueWindow "\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
#define cpCyanWindow "\x10\x11\x12\x13\x14\x15\x16\x17"
#define cpGrayWindow "\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"

class TFrame;
class TRect;
class TPoint;
class TEvent;
class TFrame;
class TScrollBar;

/**
 * @ref TWindow inherits multiply from @ref TGroup and the virtual base class
 * TWindowInit.
 *
 * TWindowInit provides a constructor and @ref TWindowInit::createFrame()
 * member function used in creating and inserting a frame object. A similar
 * technique is used for @ref TProgram, @ref THistoryWindow and @ref TDeskTop.
 * @short Virtual base class for TWindow
 */
class TWindowInit
{
public:
    /**
     * This constructor takes a function address argument, usually
     * &TWindow::initFrame.
     * @see TWindow::initFrame
     *
     * Note: the @ref TWindow constructor invokes @ref TGroup constructor and
     * TWindowInit(&initFrame) to create a window object of size `bounds'
     * and associated frame. The latter is inserted in the window group
     * object.
     * @see TGroup::TGroup
     * @see TWindow::TWindow
     */
    TWindowInit( TFrame *(*cFrame)( TRect bounds ) );
protected:
    /**
     * Called by the TWindowInit constructor to create a @ref TFrame object
     * with the given bounds and return a pointer to it. A 0 pointer
     * indicates lack of success in this endeavor.
     */
    TFrame *(*createFrame)( TRect bounds );
};

/* ---------------------------------------------------------------------- */
/*      class TWindow                                                     */
/*                                                                        */
/*      Palette layout                                                    */
/*        1 = Frame passive                                               */
/*        2 = Frame active                                                */
/*        3 = Frame icon                                                  */
/*        4 = ScrollBar page area                                         */
/*        5 = ScrollBar controls                                          */
/*        6 = Scroller normal text                                        */
/*        7 = Scroller selected text                                      */
/*        8 = Reserved                                                    */
/* ---------------------------------------------------------------------- */

/**
 * A TWindow object is a specialized group that typically owns a @ref TFrame
 * object, an interior @ref TScroller object, and one or two @ref TScrollBar
 * objects.
 * These attached subviews provide the "visibility" to the TWindow object.
 *
 * TWindow inherits multiply from @ref TGroup and the virtual base class
 * @ref TWindowInit.
 * @short Implements a window
 */
class TWindow: public TGroup, public virtual TWindowInit
{
public:
    /**
     * Calls the @ref TGroup constructor to set window bounds to `bounds'.
     * Sets default @ref state to @ref sfShadow. Sets default @ref options to
     * (@ref ofSelectable | @ref ofTopSelect). Sets default @ref growMode to
     * @ref gfGrowAll | @ref gfGrowRel. Sets default @ref flags to
     * (@ref wfMove | @ref wfGrow | @ref wfClose | @ref wfZoom). Sets the
     * @ref title data member to `aTitle' and the @ref number data member to
     * `aNumber'.
     *
     * Calls @ref initFrame() by default, and if the resulting frame pointer
     * is nonzero, inserts it in this window's group. Finally, the default
     * @ref zoomRect is set to the given bounds.
     *
     * `aNumber' is the number assigned to this window. If `aNumber' is
     * between 1 and 9, the number will appear in the frame title, and the
     * window can be selected with the Alt-n keys (n = 1 to 9).
     *
     * Use the constant @ref wnNoNumber to indicate that the window is not
     * to be numbered and cannot be selected via the Alt+number key.
     * @ref wnNoNumber is defined in `views.h' as:
     *
     * <pre>
     * const ushort @ref wnNoNumber = 0;
     * </pre>
     */
    TWindow( const TRect& bounds,
             const char *aTitle,
             short aNumber
           );
    /**
     * Deletes title, then disposes of the window and any subviews by calling
     * the parent destructor(s).
     */
    ~TWindow();
    /**
     * Calls valid(cmClose); if True is returned, the calling window is
     * deleted.
     * @see TGroup::Valid
     */
    virtual void close();
    /**
     * Returns the palette string given by the palette index in the
     * @ref palette data member.
     */
    virtual TPalette& getPalette() const;
    /**
     * Returns @ref title data member, the window's title string.
     */
    virtual const char *getTitle( short maxSize );
    /**
     * First calls @ref TGroup::handleEvent(), and then handles events
     * specific to a TWindow as follows:
     *
     * -# The following @ref evCommand events are handled if the @ref flags
     *    data member permits that operation:
     *    - cmResize (move or resize the window using the @ref dragView()
     *      member function);
     *    - cmClose (close the window by creating a cmCancel event);
     *    - cmZoom (zoom the window using the @ref zoom() member function).
     * -# @ref evKeyDown events with a keyCode value of kbTab or kbShiftTab
     *    are handled by selecting the next or previous selectable subview (if
     *    any).
     * -# An @ref evBroadcast event with a command value of cmSelectWindowNum
     *    is handled by selecting the window if the `event.infoInt' data
     *    member is equal to @ref number data member.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Creates a @ref TFrame object for the window and stores a pointer to the
     * frame in the @ref frame data member. TWindow constructor calls
     * initFrame(); it should never be called directly. You can override
     * initFrame() to instantiate a user-defined class derived from
     * @ref TFrame instead of the standard @ref TFrame.
     */
    static TFrame *initFrame( TRect );
    /**
     * First calls TGroup::setState(aState, enable). Then, if `aState' is
     * equal to @ref sfSelected, activates or deactivates the window and all
     * its subviews using a call to setState(sfActive, enable), and calls
     * @ref enableCommands() or @ref disableCommands() for cmNext, cmPrev,
     * cmResize, cmClose and cmZoom.
     * @see TGroup::setState
     * @see TView::state
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Overrides TView::sizeLimits(). First calls TView::sizeLimits(min, max)
     * and then changes `min' to the minimum window size, minWinSize, a
     * @ref TPoint constant defined at the head of file `TWindow.cc'.
     * minWinSize is currently set to (16, 6).
     *
     * minWinSize defines the minimum size of a TWindow or of any class
     * derived from TWindow. Any change to minWinSize affects all windows,
     * unless a window's sizeLimits() member function is overridden.
     */
    virtual void sizeLimits( TPoint& min, TPoint& max );
    /**
     * Creates, inserts, and returns a pointer to a "standard" scroll bar for
     * the window. "Standard" means the scroll bar fits onto the frame of the
     * window without covering the corners or the resize icon.
     *
     * The `aOptions' parameter can be either @ref sbHorizontal to produce a
     * horizontal scroll bar along the bottom of the window or
     * @ref sbVertical to produce a vertical scroll bar along the right side
     * of the window.
     *
     * Either may be combined with @ref sbHandleKeyboard to allow the scroll
     * bar to respond to arrows and page keys from the keyboard in addition
     * to mouse clicks.
     *
     * The following values can be passed to standardScrollBar():
     *
     * <pre>
     * Constant         Value  Meaning
     *
* @ref sbHorizontal     0x0000 Scroll bar is horizontal
* @ref sbVertical       0x0001 Scroll bar is vertical
* @ref sbHandleKeyboard 0x0002 Scroll bar responds to keyboard commands
     * </pre>
     */
    TScrollBar *standardScrollBar( ushort aOptions );
    /**
     * Zooms the calling window. This member function is usually called in
     * response to a cmZoom command (triggered by a click on the zoom icon).
     * zoom() takes into account the relative sizes of the calling window and
     * its owner, and the value of @ref zoomRect.
     */
    virtual void zoom();
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when @ref destroy() is called.
     */
    virtual void shutDown();
    /**
     * The flags data member contains a combination of mnemonics constants
     * that define bits. If the bits are set, the window will have the
     * corresponding attribute: the window can move, grow, close or zoom.
     *
     * The window flags are defined as follows:
     *
     * <pre>
     * Constant Value Meaning
     *
* @ref wfMove   0x01  Window can be moved
     *
* @ref wfGrow   0x02  Window can be resized and has a grow icon in the
     *                lower-right corner
     *
* @ref wfClose  0x04  Window frame has a close icon that can be mouse-clicked
     *                to close the window
     *
* @ref wfZoom   0x08  Window frame has a zoom icon that can be mouse-clicked
     *                to zoom the window
     * </pre>
     *
     * If a particular bit is set, the corresponding property is enabled;
     * otherwise that property is disabled.
     */
    uchar flags;
    /**
     * The normal, unzoomed boundary of the window.
     */
    TRect zoomRect;
    /**
     * The number assigned to this window. If number is between 1 and 9,
     * the number will appear in the frame title, and the window can be
     * selected with the Alt-n keys (n = 1 to 9).
     */
    short number;
    /**
     * Specifies which palette the window is to use: @ref wpBlueWindow,
     * @ref wpCyanWindow or @ref wpGrayWindow. These constants define the
     * three standard color mapping assignments for windows:
     *
     * <pre>
     * Constant     Value  Meaning
     *
* @ref wpBlueWindow 0      Window text is yellow on blue
* @ref wpCyanWindow 1      Window text is blue on cyan
* @ref wpGrayWindow 2      Window text is black on gray
     * </pre>
     *
     * By default, the TWindow palette is @ref wpBlueWindow. The default for
     * @ref TDialog objects is @ref wpGrayWindow.
     */
    short palette;
    /**
     * Pointer to this window's associated TFrame object.
     */
    TFrame *frame;
    /**
     * A character string giving the (optional) title that appears on the
     * frame.
     */
    const char *title;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TWindow( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TWindow& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TWindow*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TWindow& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TWindow* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TWindow
