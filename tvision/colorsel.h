/*
 * colorsel.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_COLOR_COMMAND_CODES )
#define TV_COLOR_COMMAND_CODES

/** \file colorsel.h
 * colorsel.h
 */

const
  int cmColorForegroundChanged = 71,
  cmColorBackgroundChanged = 72,
  cmColorSet               = 73,
  cmNewColorItem           = 74,
  cmNewColorIndex          = 75,
  cmSaveColorIndex         = 76;

#endif  // TV_COLOR_COMMAND_CODES

class TColorItem;
class TColorGroup;

#if defined( Uses_TColorItem ) && !defined( TV_TColorItem )
#define TV_TColorItem

class TColorGroup;

/**
 * Stores information about a color item. TColorItem defines a linked list of
 * color names and indexes.
 *
 * The interrelated classes TColorItem, @ref TColorGroup, @ref TColorSelector,
 * @ref TMonoSelector, @ref TColorDisplay, @ref TColorGroupList,
 * @ref TColorItemList and @ref TColorDialog provide viewers and dialog boxes
 * from which the user can select and change the color assignments from
 * available palettes with immediate effect on the screen.
 * @short Information about a color item
 */
class TColorItem
{
public:
    /**
     * Constructor.
     *
     * Creates a color item object with @ref name set to `nm', @ref index set
     * to `idx' and, by default, @ref next set to 0.
     *
     * `nm' is a pointer to the name of the color item.  A local copy of the
     * string is created. `idx' is the color index. `nxt' is a pointer to the
     * next color item (its default value is 0).
     *
     * See file `demo/tvdemo2.cc' for an example.
     */
    TColorItem( const char *nm, uchar idx, TColorItem *nxt = nullptr );
    /**
     * Destructor.
     */
    virtual ~TColorItem();
    /**
     * The name of the color item.
     */
    const char *name;
    /**
     * The color index of the item.
     */
    uchar index;
    /**
     * Link to the next color item, or 0 if there is no next item.
     */
    TColorItem *next;
    /**
     * Inserts color item `i' in color group `g'.
     * @see TColorGroup
     */
    friend TColorGroup& operator + ( TColorGroup& g, TColorItem& i );
    /**
     * Inserts another color item after this one by changing the
     * @ref TColorItem::next pointer.
     */
    friend TColorItem& operator + ( TColorItem& i1, TColorItem& i2 );
};

#endif  // Uses_TColorItem

#if defined( Uses_TColorGroup ) && !defined( TV_TColorGroup )
#define TV_TColorGroup

class TColorItem;

/**
 * The interrelated classes @ref TColorItem, TColorGroup, @ref TColorSelector,
 * @ref TMonoSelector, @ref TColorDisplay, @ref TColorGroupList,
 * @ref TColorItemList and @ref TColorDialog provide viewers and dialog boxes
 * from which the user can select and change the color assignments from
 * available palettes with immediate effect on the screen.
 *
 * The TColorGroup class defines a group of linked lists of @ref TColorItem
 * objects. Each member of a color group consists of a set of color names and
 * their associated color codes.
 * @short Stores a set of color items
 */
class TColorGroup
{
public:
    /**
     * Constructor.
     *
     * Creates a color group with the given argument values.
     *
     * `nm' is a pointer to the name of the color group.  A local copy of the
     * string is created. `itm' is a pointer to the first color item of the
     * color group (its default value is 0). `nxt' is a pointer to the next
     * color group (its default value is 0).
     *
     * See file `demo/tvdemo2.cc' for an example.
     */
    TColorGroup( const char *nm, TColorItem *itm = nullptr, TColorGroup *nxt = nullptr );
    /**
     * Destructor.
     */
    virtual ~TColorGroup();
    /**
     * The name of the color group.
     */
    const char *name;
    /**
     * The start index of the color group.
     */
    uchar index;
    /**
     * Pointer to the associated list of color items associated with this
     * color group.
     */
    TColorItem *items;
    /**
     * Pointer to next color group, or 0 if no next.
     */
    TColorGroup *next;
    /**
     * Inserts color item `i' in color group `g'.
     * @see TColorItem
     */
    friend TColorGroup& operator + ( TColorGroup& g, TColorItem& i);
    /**
     * Inserts another color group after this one by changing the
     * @ref TColorGroup::next pointer.
     * @see TColorItem
     */
    friend TColorGroup& operator + ( TColorGroup& g1, TColorGroup& g2 );
};

/**
 * Data structure used by @ref TColorDialog::getIndexes() and
 * @ref TColorDialog::setIndexes().
 */
class TColorIndex
{
public:
    /**
     * Undocumented.
     */
    uchar groupIndex;
    /**
     * Undocumented.
     */
    uchar colorSize;
    /**
     * Undocumented.
     */
    uchar colorIndex[256];
};


#endif  // Uses_TColorGroup

#if defined( Uses_TColorSelector ) && !defined( TV_TColorSelector )
#define TV_TColorSelector

class TRect;
class TEvent;

/**
 * The interrelated classes @ref TColorItem, @ref TColorGroup,
 * TColorSelector, @ref TMonoSelector, @ref TColorDisplay,
 * @ref TColorGroupList, @ref TColorItemList and @ref TColorDialog provide
 * viewers and dialog boxes from which the user can select and change the
 * color assignments from available palettes with immediate effect on the
 * screen.
 *
 * TColorSelector is a view for displaying the color selections available.
 * @short Color viewer used to display available color selections
 */
class TColorSelector : public TView
{
public:
    /**
     * This view can handle two sets of colors: the 8 background colors or the
     * 16 foreground colors.
     */
    enum ColorSel { csBackground = 0, csForeground };
    /**
     * Constructor.
     *
     * Calls TView constructor TView(bounds) to create a view with the given
     * `bounds'. Sets @ref options data member with @ref ofSelectable,
     * @ref ofFirstClick, and @ref ofFramed. Sets @ref eventMask to
     * @ref evBroadcast, @ref selType to `aSelType', and @ref color to 0.
     *
     * `aSelType' may be one of the following values:
     *
     * <pre>
     * Constant     Value Meaning
     *
     * csBackground 0     show the 8 background colors
     * csForeground 1     show the 16 foreground colors
     * </pre>
     */
    TColorSelector( const TRect& bounds, ColorSel aSelType );
    /**
     * Draws the color selector.
     */
    virtual void draw();
    /**
     * Handles mouse and key events: you can click on a given color indicator
     * to select that color, or you can select colors by positioning the
     * cursor with the arrow keys.
     *
     * Changes invoke @ref drawView() when appropriate.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * This character is used to mark the current color.
     */
    static char icon;
protected:
    /**
     * Holds the currently selected color.
     */
    uchar color;
    /**
     * Specifies if the view shows the 8 background colors or the 16
     * foreground colors.
     *
     * Gives attribute (foreground or background) of the currently selected
     * color. @ref ColorSel is an enum defined as follows:
     *
     * <pre>
     * enum ColorSel { csBackground = 0, csForeground }
     * </pre>
     */
    ColorSel selType;
private:
    void colorChanged();
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the view from a stream.
     */
    TColorSelector( StreamableInit );
    /**
     * Used to store the view in a stream. Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Used to recover the view from a stream. Reads from the input stream
     * `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TColorSelector.
     *
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorSelector& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorSelector*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorSelector& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorSelector* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TColorSelector


#if defined( Uses_TMonoSelector ) && !defined( TV_TMonoSelector )
#define TV_TMonoSelector

class TRect;
class TEvent;

/**
 * The interrelated classes @ref TColorItem, @ref TColorGroup,
 * @ref TColorSelector, TMonoSelector, @ref TColorDisplay,
 * @ref TColorGroupList, @ref TColorItemList and @ref TColorDialog are used
 * to provide viewers and dialog boxes from which the user can select and
 * change the color assignments from available palettes with immediate effect
 * on the screen.
 *
 * TMonoSelector implements a cluster from which you can select normal,
 * highlight, underline, or inverse video attributes on monochrome screens.
 * @short Monochrome color selector
 */
class TMonoSelector : public TCluster
{
public:
    /**
     * Constructor.
     *
     * Creates a cluster by calling the TCluster constructor with four
     * buttons labeled: "Normal", "Highlight", "Underline", and "Inverse".
     * The @ref evBroadcast flag is set in @ref eventMask. `bounds' is the
     * bounding rectangle of the view.
     * @see TCluster::TCluster
     */
    TMonoSelector( const TRect& bounds );
    /**
     * Draws the selector cluster.
     */
    virtual void draw();
    /**
     * Calls @ref TCluster::handleEvent() and responds to cmColorSet events by
     * changing the data member value accordingly. The view is redrawn if
     * necessary. @ref value holds a video attribute corresponding to the
     * selected attribute.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Returns True if the item'th button has been selected; otherwise returns
     * False.
     */
    virtual Boolean mark( int item );
    /**
     * Informs the owning group if the attribute has changed.
     */
    void newColor();
    /**
     * "Presses" the item'th button and calls @ref newColor().
     */
    virtual void press( int item );
    /**
     * Sets value to the item'th attribute (same effect as @ref press()).
     */
    void movedTo( int item );
private:
    static const char * button;
    static const char * normal;
    static const char * highlight;
    static const char * underline;
    static const char * inverse;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the view from a stream.
     */
    TMonoSelector( StreamableInit );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TMonoSelector.
     *
     * Used to recover the view from a stream. Called to create an object in
     * certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TMonoSelector& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TMonoSelector*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TMonoSelector& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TMonoSelector* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TMonoSelector

#if defined( Uses_TColorDisplay ) && !defined( TV_TColorDisplay )
#define TV_TColorDisplay

class TRect;
class TEvent;

/**
 * TColorDisplay and these interrelated classes provide viewers and dialog
 * boxes from which the user can select and change the screen attributes and
 * color assignments from available palettes with immediate effect on the
 * screen:
 *
 * @ref TColorItem, @ref TColorGroup, @ref TColorSelector, @ref TMonoSelector,
 * @ref TColorGroupList, @ref TColorItemList and @ref TColorDialog.
 *
 * TColorDisplay is a view for displaying text so that the user can select a
 * suitable palette.
 * @short Viewer used to display and select colors
 */
class TColorDisplay : public TView
{
public:
    /**
     * Constructor.
     *
     * Creates a view of the given size with TView constructors TView(bounds),
     * then sets text to the `aText' argument.
     *
     * `bounds' is the bounding rectangle of the view. `aText' is the string
     * printed in the view.
     */
    TColorDisplay( const TRect& bounds, const char *aText );
    /**
     * Destructor.
     *
     * Destroys both the view and the text string.
     */
    virtual ~TColorDisplay();
    /**
     * Draws the view with given text and current color.
     */
    virtual void draw();
    /**
     * Calls @ref TView::handleEvent() and redraws the view as appropriate in
     * response to the cmColorBackgroundChanged and cmColorForegroundChanged
     * broadcast events.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Change the currently displayed color. Sets color to `aColor',
     * broadcasts the change to the owning group, then calls
     * @ref drawView().
     */
    virtual void setColor( uchar *aColor );
protected:
    /**
     * Stores the current color for this display.
     */
    uchar *color;
    /**
     * Stores a pointer to the text string to be displayed.
     */
    const char *text;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the view from a stream.
     */
    TColorDisplay( StreamableInit );
    /**
     * Used to store the view in a stream. Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Used to recover the view from a stream. Reads from the input stream
     * `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TColorDisplay.
     *
     * Used to recover the view from a stream. Called to create an object in
     * certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorDisplay& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorDisplay*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorDisplay& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorDisplay* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TColorDisplay


#if defined( Uses_TColorGroupList ) && !defined( TV_TColorGroupList )
#define TV_TColorGroupList

class TRect;
class TScrollBar;
class TColorGroup;
class TColorItem;

/**
 * The interrelated classes @ref TColorItem, @ref TColorGroup,
 * @ref TColorSelector, @ref TMonoSelector, @ref TColorDisplay,
 * TColorGroupList, @ref TColorItemList and @ref TColorDialog provide
 * viewers and dialog boxes from which the user can select and change the
 * color assignments from available palettes with immediate effect on the
 * screen.
 *
 * TColorGroupList is a specialized derivative of @ref TListViewer providing a
 * scrollable list of named color groups. Groups can be selected in any of the
 * usual ways (by mouse or keyboard).
 *
 * TColorGroupList uses the @ref TListViewer event handler without
 * modification.
 * @short Implements a scrollable list of named color groups
 */
class TColorGroupList : public TListViewer
{
public:
    /**
     * Constructor.
     *
     * Calls TListViewer(bounds, 1, 0, aScrollBar) to create a single-column
     * list viewer a single vertical scroll bar. Then, sets @ref groups data
     * member to `aGroups'.
     * @see TListViewer::TListViewer
     */
    TColorGroupList( const TRect& bounds,
                     TScrollBar *aScrollBar,
                     TColorGroup *aGroups
                      );
    /**
     * Destroys the list viewer and all associated groups and their items.
     */
    virtual ~TColorGroupList();
    /**
     * Selects the given item by calling TListViewer::focusItem(item) and then
     * broadcasts a cmNewColorItem event.
     * @see TListViewer::focusItem
     */
    virtual void focusItem( short item );
    /**
     * Copies the group name corresponding to `item' to the `dest' string.
     */
    virtual void getText( char *dest, short item, short maxLen );
    /**
     * Undocumented.
     */
    virtual void handleEvent(TEvent&);
protected:
    /**
     * The color group for this list viewer.
     */
    TColorGroup *groups;
private:
    virtual const char *streamableName() const
        { return name; }
    static void writeItems( opstream&, TColorItem * );
    static void writeGroups( opstream&, TColorGroup * );
    static TColorItem *readItems( ipstream& );
    static TColorGroup *readGroups( ipstream& );
protected:
    /**
     * Undocumented.
     */
    TColorGroupList( StreamableInit );
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
    void setGroupIndex(uchar groupNum, uchar itemNum);
    /**
     * Undocumented.
     */
    TColorGroup* getGroup(uchar groupNum);
    /**
     * Undocumented.
     */
    uchar getGroupIndex(uchar groupNum);
    /**
     * Undocumented.
     */
    uchar getNumGroups();
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
inline ipstream& operator >> ( ipstream& is, TColorGroupList& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorGroupList*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorGroupList& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorGroupList* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TColorGroupList


#if defined( Uses_TColorItemList ) && !defined( TV_TColorItemList )
#define TV_TColorItemList

class TRect;
class TScrollBar;
class TColorItem;
class TEvent;

/**
 * The interrelated classes @ref TColorItem, @ref TColorGroup,
 * @ref TColorSelector, @ref TMonoSelector, @ref TColorDisplay,
 * @ref TColorGroupList, TColorItemList, and @ref TColorDialog provide
 * viewers and dialog boxes from which the user can select and change the
 * color assignments from available palettes with immediate effect on the
 * screen.
 *
 * TColorItemList is a simpler variant of @ref TColorGroupList for viewing and
 * selecting single color items rather than groups of colors.
 *
 * Like @ref TColorGroupList, TColorItemList is specialized derivative of
 * @ref TListViewer. Color items can be selected in any of the usual ways (by
 * mouse or keyboard).
 *
 * Unlike @ref TColorGroupList, TColorItemList overrides the @ref TListViewer
 * event handler.
 * @short Used to view and select single color items
 */
class TColorItemList : public TListViewer
{
public:
    /**
     * Calls TListViewer constructor TListViewer(bounds, 1, 0, aScrollBar) to
     * create a single-column list viewer with a single vertical scroll bar.
     *
     * Then, the constructor sets @ref items data member to `aItems' and
     * @ref range to the number of items.
     */
    TColorItemList( const TRect& bounds,
                    TScrollBar *aScrollBar,
                    TColorItem *aItems
                  );
    /**
     * Selects the given item by calling TListViewer::focusItem(item), then
     * broadcasts a cmNewColorIndex event.
     * @see TListViewer::focusItem
     */
    virtual void focusItem( short item );
    /**
     * Copies the item name corresponding to `item' to the `dest' string.
     */
    virtual void getText( char *dest, short item, short maxLen );
    /**
     * Calls @ref TListViewer::handleEvent(). Then, if the event is
     * cmNewColorItem, the appropriate item is focused and the viewer is
     * redrawn.
     */
    virtual void handleEvent( TEvent& event );
protected:
    /**
     * The color item list for this viewer.
     */
    TColorItem *items;
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
    TColorItemList( StreamableInit );
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
inline ipstream& operator >> ( ipstream& is, TColorItemList& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorItemList*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorItemList& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorItemList* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TColorItemList


#if defined( Uses_TColorDialog ) && !defined( TV_TColorDialog )
#define TV_TColorDialog

class TColorGroup;
class TEvent;
class TColorDisplay;
class TColorGroupList;
class TLabel;
class TColorSelector;
class TMonoSelector;
class TPalette;

/**
 * These interrelated classes provide viewers and dialog boxes from which the
 * user can select and change the color assignments from available palettes
 * with immediate effect on the screen:
 *
 * @ref TColorDisplay, @ref TColorGroup, @ref TColorGroupList,
 * @ref TColorItem, @ref TColorItemList, @ref TColorSelector,
 * @ref TMonoSelector and TColorDialog.
 *
 * TColorDialog is a specialized scrollable dialog box called "Colors" from
 * which the user can examine various palette selections before making a
 * selection.
 * @short Viewer used to examine and change the standard palette
 */
class TColorDialog : public TDialog
{
public:
    /**
     * Constructor.
     *
     * Calls the @ref TDialog and @ref TScrollBar constructors to create a
     * fixed size, framed window titled "Colors" with two scroll bars. The
     * @ref pal data member is set to `aPalette'. The given `aGroups'
     * argument creates and inserts a @ref TColorGroup object with an
     * associated label: "~G~roup". The items in `aGroups' initialize a
     * @ref TColorItemList object, which is also inserted in the dialog,
     * labeled as "~I~tem".
     *
     * `aPalette' is a pointer to the initial palette to be modified. This
     * class creates a local copy of the initial palette, so the initial
     * palette is never modified. You may safely leave this field to 0 and set
     * the palette with a subsequent call to @ref setData(). `aGroups' is a
     * pointer to a cluster of objects which specifies all the items in the
     * palette.
     *
     * See file `demo/tvdemo2.cc' for an example.
     */
    TColorDialog( TPalette *aPalette, TColorGroup *aGroups );
    /**
     * Destructor.
     *
     * Deletes the local copy of the palette.
     */
    ~TColorDialog();
    /**
     * By default, dataSize() returns the size of the current palette,
     * expressed in bytes.
     */
    virtual ushort dataSize();
    /**
     * Reads the data record of this dialog.
     *
     * Copies @ref dataSize() bytes from @ref pal to `rec'. `rec' should
     * point to a @ref TPalette object. Its contents will be overwritten by
     * the contents of this object.
     * @see setData
     */
    virtual void getData( void *rec );
    /**
     * Calls @ref TDialog::handleEvent() and redisplays if the broadcast event
     * generated is cmNewColorIndex.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Writes the data record of this view.
     *
     * The reverse of @ref getData(): copies from `rec' to @ref pal,
     * restoring the saved color selections. `rec' should point to a
     * @ref TPalette object.
     */
    virtual void setData( void *rec);
    /**
     * Is a pointer to the current palette selection.
     */
    TPalette *pal;
protected:
    /**
     * The color display object for this dialog box.
     */
    TColorDisplay *display;
    /**
     * The color group for this dialog box.
     */
    TColorGroupList *groups;
    /**
     * The foreground color label.
     */
    TLabel *forLabel;
    /**
     * The foreground color selector.
     */
    TColorSelector *forSel;
    /**
     * The background color label.
     */
    TLabel *bakLabel;
    /**
     * The background color selector.
     */
    TColorSelector *bakSel;
    /**
     * The monochrome label.
     */
    TLabel *monoLabel;
    /**
     * The selector for monochrome attributes.
     */
    TMonoSelector *monoSel;
    /**
     * Undocumented.
     */
    uchar groupIndex;
private:
    static const char * colors;
    static const char * groupText;
    static const char * itemText;
    static const char * forText;
    static const char * bakText;
    static const char * textText;
    static const char * colorText;
    static const char * okText;
    static const char * cancelText;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the dialog from a stream. Foreground and background
     * color selectors are created and inserted, labeled as "~F~oreground" and
     * "~B~ackground". The string "Text " is displayed in the current text
     * color.
     *
     * A @ref TMonoSelector object is created, inserted, and labeled
     * "~C~olor". All the items are displayed in their correct colors and
     * attributes. Finally, "O~K~" and "Cancel" buttons are inserted.
     */
    TColorDialog( StreamableInit );
    /**
     * Used to store the dialog in a stream. Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Used to recover the dialog from a stream. Reads from the input stream
     * `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    void getIndexes(TColorIndex*&);
    /**
     * Undocumented.
     */
    void setIndexes(TColorIndex*&);
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
inline ipstream& operator >> ( ipstream& is, TColorDialog& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TColorDialog*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorDialog& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TColorDialog* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // TColorDialog
