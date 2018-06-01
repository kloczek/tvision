/*
 * app.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TBackground ) && !defined( __TBackground )
#define __TBackground

/** \file app.h
 * app.h
 */

class TRect;

/**
 * The default desktop background.
 *
 * TBackground is a very simple view which by default is the background of the
 * desktop. It is a rectangle painted with an uniform pattern.
 * @see TDeskTop
 * @short The default desktop background
 */
class TBackground : public TView
{
public:
    /**
     * Constructor.
     *
     * `bounds' is the bounding rectangle of the background.
     *
     * @ref growMode is set to @ref gfGrowHiX | @ref gfGrowHiY, and the
     * @ref pattern data member is set to `aPattern'.
     */
    TBackground( const TRect& bounds, char aPattern );
    /**
     * Fills the background view rectangle with the current pattern in the
     * default color.
     */
    virtual void draw();
    /**
     * Returns a reference to the standard TBackground palette.
     */
    virtual TPalette& getPalette() const;
protected:
    /**
     * Is the pattern used to fill the view.
     */
    char pattern;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the view from a stream.
     *
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TBackground( StreamableInit );
    /**
     * Used to store the view in a stream.
     *
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Used to recover the view from a stream.
     *
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TBackground.
     *
     * Used to recover the view from a stream.
     *
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TBackground& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TBackground*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TBackground& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TBackground* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TBackground


#if defined( Uses_TDeskTop )  && !defined( __TDeskTop )
#define __TDeskTop

class TBackground;
class TRect;
class TEvent;

/**
 * TDeskInit is used as a virtual base class for a number of classes,
 * providing a constructor and a create background member function used in
 * creating and inserting a background object.
 * @see TDeskTop
 * @short Virtual base class for TDeskTop
 */
class TDeskInit
{
public:
    /**
     * This constructor takes a function address argument, usually
     * &TDeskTop::initBackground.
     * @see TDeskTop::initBackground
     *
     * Note: the @ref TDeskTop constructor invokes @ref TGroup constructor and
     * TDeskInit(&initBackground) to create a desk top object of size `bounds'
     * and associated background. The latter is inserted in the desk top group
     * object.
     * @see TDeskTop::TDeskTop
     */
    TDeskInit( TBackground *(*cBackground)( TRect bounds ) );
protected:
    /**
     * Called by the TDeskInit constructor to create a TBackground object
     * with the given bounds and return a pointer to it. A 0 pointer
     * indicates lack of success in this endeavor.
     */
    TBackground *(*createBackground)( TRect bounds );
};

/**
 * The desktop of the application.
 * @see TApplication
 *
 * TDeskTop inherits multiply from @ref TGroup and the virtual base class
 * @ref TDeskInit. @ref TDeskInit provides a constructor and a create
 * background member function used in creating and inserting a background
 * object. TDeskTop is a simple group that owns the @ref TBackground view
 * upon which the application's windows and other views appear.
 *
 * TDeskTop represents the desk top area of the screen between the top menu
 * bar and bottom status line (but only when the bar and line exist). By
 * default, TDeskTop has a @ref TBackground object inside which paints its
 * background.
 *
 * TDeskTop objects can be written to and read from streams using the
 * overloaded >> and << operators.
 * @short The desktop of the application
 */
class TDeskTop : public TGroup, public virtual TDeskInit
{
public:
    /**
     * Constructor.
     *
     * Creates a TDeskTop group with size `bounds' by calling its base
     * constructors TGroup::TGroup and TDeskInit::TDeskInit(&initBackground).
     * The resulting @ref TBackground object created by @ref initBackground is
     * then inserted into the desk top.
     * @see TDeskInit:TDeskInit
     * @see TGroup::TGroup
     *      
     * @ref growMode is set to @ref gfGrowHiX | @ref gfGrowHiY.
     */
    TDeskTop( const TRect& bounds );
    /**
     * Moves all the windows in a cascade-like fashion.
     *
     * Redisplays all tileable windows owned by the desk top in cascaded
     * format. The first tileable window in Z-order (the window "in back") is
     * zoomed to fill the desk top, and each succeeding window fills a region
     * beginning one line lower and one space further to the right than the
     * one before. The active window appears "on top" as the smallest window.
     */
    void cascade( const TRect& );
    /**
     * Standard TDeskTop event handler.
     *
     * Calls @ref TGroup::handleEvent() and takes care of the commands cmNext
     * (usually the hot key F6) and cmPrev by cycling through the windows
     * owned by the desk top, starting with the currently selected view.
     */
    virtual void handleEvent( TEvent& );
    /**
     * Creates a new background.
     *
     * Returns a pointer to a newly-allocated @ref TBackground object. The
     * address of this member function is passed as an argument to the
     * @ref TDeskInit constructor. The latter invokes @ref initBackground()
     * to create a new @ref TBackground object with the same bounds as the
     * calling TDeskTop object.
     * The @ref background data member is set to point at the new
     * @ref TBackground object.
     *
     * Redefine this method if you want a custom background.
     */
    static TBackground *initBackground( TRect );
    /**
     * Moves all the windows in a tile-like fashion.
     */
    void tile( const TRect& );
    /**
     * Called on tiling error.
     *
     * This method is called whenever @ref cascade() or @ref tile() run into
     * troubles in moving the windows. You can redefine it if you want to
     * give an error message to the user. By default, it does nothing.
     */
    virtual void tileError();
    /**
     * Releases TDeskTop resources.
     *
     * This function is derived from @ref TObject. Used internally by
     * @ref TObject::destroy() to ensure correct destruction of derived and
     * related objects. shutDown() is overridden in many classes to ensure
     * the proper setting of related data members when destroy() is called.
     *
     * This method releases all the resources allocated by the TDeskTop. It
     * sets pointer @ref background to 0 and then calls
     * @ref TGroup::shutDown().
     */
    virtual void shutDown();
    /**
     * The default pattern which will be used for painting the background.
     */
    static char defaultBkgrnd;
protected:
    /**
     * This variable stores a pointer to the background object associated with
     * this desk top.
     */
    TBackground *background;
    /**
     * True if method @ref tile() should favour columns first. Set to False in
     * TDeskTop constructor.
     * @see TDeskTop::TDeskTop
     */
    Boolean tileColumnsFirst;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor used to recover the view from a stream.
     *
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TDeskTop( StreamableInit );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TDeskTop.
     *
     * Used to recover the view from a stream. Called to create an object in
     * certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TDeskTop& cl )
    { return is >> (TStreamable&)(TGroup&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TDeskTop*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDeskTop& cl )
    { return os << (TStreamable&)(TGroup&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDeskTop* cl )
    { return os << (TStreamable *)(TGroup *)cl; }

#endif

// Turbo Vision 2.0 Color Palettes

#ifndef __UNPATCHED
#define cpAppColor \
       "\x71\x70\x78\x74\x20\x28\x24\x17\x1F\x1A\x31\x31\x1E\x71\x1F" \
    "\x37\x3F\x3A\x13\x13\x3E\x21\x3F\x70\x7F\x7A\x13\x13\x70\x7F\x7E" \
    "\x70\x7F\x7A\x13\x13\x70\x70\x7F\x7E\x20\x2B\x2F\x78\x2E\x70\x30" \
    "\x3F\x3E\x1F\x2F\x1A\x20\x72\x31\x31\x30\x2F\x3E\x31\x13\x38\x00" \
    "\x17\x1F\x1A\x71\x71\x1E\x17\x1F\x1E\x20\x2B\x2F\x78\x2E\x10\x30" \
    "\x3F\x3E\x70\x2F\x7A\x20\x12\x31\x31\x30\x2F\x3E\x31\x13\x38\x00" \
    "\x37\x3F\x3A\x13\x13\x3E\x30\x3F\x3E\x20\x2B\x2F\x78\x2E\x30\x70" \
    "\x7F\x7E\x1F\x2F\x1A\x20\x32\x31\x71\x70\x2F\x7E\x71\x13\x78\x00" \
    "\x37\x3F\x3A\x13\x13\x30\x3E\x1E"    // help colors
#else
#define cpAppColor \
       "\x71\x70\x78\x74\x20\x28\x24\x17\x1F\x1A\x31\x31\x1E\x71\x1F" \
    "\x37\x3F\x3A\x13\x13\x3E\x21\x3F\x70\x7F\x7A\x13\x13\x70\x7F\x7E" \
    "\x70\x7F\x7A\x13\x13\x70\x70\x7F\x7E\x20\x2B\x2F\x78\x2E\x70\x30" \
    "\x3F\x3E\x1F\x2F\x1A\x20\x72\x31\x31\x30\x2F\x3E\x31\x13\x38\x00" \
    "\x17\x1F\x1A\x71\x71\x1E\x17\x1F\x1E\x20\x2B\x2F\x78\x2E\x10\x30" \
    "\x3F\x3E\x70\x2F\x7A\x20\x12\x31\x31\x30\x2F\x3E\x31\x13\x38\x00" \
    "\x37\x3F\x3A\x13\x13\x3E\x30\x3F\x3E\x20\x2B\x2F\x78\x2E\x30\x70" \
    "\x7F\x7E\x1F\x2F\x1A\x20\x32\x31\x71\x70\x2F\x7E\x71\x13\x38\x00" \
    "\x37\x3F\x3A\x13\x13\x30\x3E\x1E"    // help colors
#endif

#ifndef __UNPATCHED
#define cpAppBlackWhite \
       "\x70\x70\x78\x7F\x07\x07\x0F\x07\x0F\x07\x70\x70\x07\x70\x0F" \
    "\x07\x0F\x07\x70\x70\x07\x70\x0F\x70\x7F\x7F\x70\x07\x70\x07\x0F" \
    "\x70\x7F\x7F\x70\x07\x70\x70\x7F\x7F\x07\x0F\x0F\x78\x0F\x78\x07" \
    "\x0F\x0F\x0F\x70\x0F\x07\x70\x70\x70\x07\x70\x0F\x07\x07\x08\x00" \
    "\x07\x0F\x0F\x07\x70\x07\x07\x0F\x0F\x70\x78\x7F\x08\x7F\x08\x70" \
    "\x7F\x7F\x7F\x0F\x70\x70\x07\x70\x70\x70\x07\x7F\x70\x07\x78\x00" \
    "\x70\x7F\x7F\x70\x07\x70\x70\x7F\x7F\x07\x0F\x0F\x78\x0F\x78\x07" \
    "\x0F\x0F\x0F\x70\x0F\x07\x70\x70\x70\x07\x70\x0F\x07\x07\x08\x00" \
    "\x07\x0F\x07\x70\x70\x07\x0F\x70"    // help colors
#else
#define cpAppBlackWhite \
       "\x70\x70\x78\x7F\x07\x07\x0F\x07\x0F\x07\x70\x70\x07\x70\x0F" \
    "\x07\x0F\x07\x70\x70\x07\x70\x0F\x70\x7F\x7F\x70\x07\x70\x07\x0F" \
    "\x70\x7F\x7F\x70\x07\x70\x70\x7F\x7F\x07\x0F\x0F\x78\x0F\x78\x07" \
    "\x0F\x0F\x0F\x70\x0F\x07\x70\x70\x70\x07\x70\x0F\x07\x07\x78\x00" \
    "\x07\x0F\x0F\x07\x70\x07\x07\x0F\x0F\x70\x78\x7F\x08\x7F\x08\x70" \
    "\x7F\x7F\x7F\x0F\x70\x70\x07\x70\x70\x70\x07\x7F\x70\x07\x78\x00" \
    "\x70\x7F\x7F\x70\x07\x70\x70\x7F\x7F\x07\x0F\x0F\x78\x0F\x78\x07" \
    "\x0F\x0F\x0F\x70\x0F\x07\x70\x70\x70\x07\x70\x0F\x07\x07\x78\x00" \
    "\x07\x0F\x07\x70\x70\x07\x0F\x70"    // help colors
#endif

#ifndef __UNPATCHED
#define cpAppMonochrome \
       "\x70\x07\x07\x0F\x70\x70\x70\x07\x0F\x07\x70\x70\x07\x70\x00" \
    "\x07\x0F\x07\x70\x70\x07\x70\x00\x70\x70\x70\x07\x07\x70\x07\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x70\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x01\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x01\x00" \
    "\x07\x0F\x07\x70\x70\x07\x0F\x70"    // help colors
#else
#define cpAppMonochrome \
       "\x70\x07\x07\x0F\x70\x70\x70\x07\x0F\x07\x70\x70\x07\x70\x00" \
    "\x07\x0F\x07\x70\x70\x07\x70\x00\x70\x70\x70\x07\x07\x70\x07\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x70\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x70\x00" \
    "\x70\x70\x70\x07\x07\x70\x70\x70\x0F\x07\x07\x0F\x70\x0F\x70\x07" \
    "\x0F\x0F\x07\x70\x07\x07\x70\x07\x07\x07\x70\x0F\x07\x07\x70\x00" \
    "\x07\x0F\x07\x70\x70\x07\x0F\x70"    // help colors
#endif

#if defined( Uses_TProgram ) && !defined( __TProgram )
#define __TProgram

// Standard application help contexts

// Note: range $FF00 - $FFFF of help contexts are reserved by Borland

const unsigned short hcNew          = 0xFF01;
const unsigned short hcOpen         = 0xFF02;
const unsigned short hcSave         = 0xFF03;
const unsigned short hcSaveAs       = 0xFF04;
const unsigned short hcSaveAll      = 0xFF05;
const unsigned short hcChangeDir    = 0xFF06;
const unsigned short hcDosShell     = 0xFF07;
const unsigned short hcExit         = 0xFF08;

const unsigned short hcUndo         = 0xFF10;
const unsigned short hcCut          = 0xFF11;
const unsigned short hcCopy         = 0xFF12;
const unsigned short hcPaste        = 0xFF13;
const unsigned short hcClear        = 0xFF14;

const unsigned short hcTile         = 0xFF20;
const unsigned short hcCascade      = 0xFF21;
const unsigned short hcCloseAll     = 0xFF22;
const unsigned short hcResize       = 0xFF23;
const unsigned short hcZoom         = 0xFF24;
const unsigned short hcNext         = 0xFF25;
const unsigned short hcPrev         = 0xFF26;
const unsigned short hcClose        = 0xFF27;


class TStatusLine;
class TMenuBar;
class TDeskTop;
class TEvent;
class TView;

/**
 * TProgInit is a virtual base class for TProgram.
 *
 * The @ref TProgram constructor calls the TProgInit base constructor,
 * passing to it the addresses of three initialization functions that
 * create the status line, menu bar, and desk top.
 * @short Virtual base class for TProgram
 */
class TProgInit
{
public:
    /**
     * The @ref TProgram constructor calls the TProgInit constructor, passing
     * to it the addresses of three init functions. The TProgInit constructor
     * creates a status line, menu bar, and desk top. If these calls are
     * successful, the three objects are inserted into the TProgram group.
     * Variables @ref TProgram::statusLine, @ref TProgram::menuBar and
     * @ref TProgram::deskTop are set to point at these new objects.
     *
     * The @ref TGroup constructor is also invoked to create a full screen
     * view: the video @ref TGroup::buffer and default palettes are
     * initialized and the following @ref TView::state flags are set:
     *
     * <pre>
     * state = @ref sfVisible | @ref sfSelected | @ref sfFocused |
     *         @ref sfModal | @ref sfExposed;
     * </pre>
     */
    TProgInit( TStatusLine *(*cStatusLine)( TRect ),
               TMenuBar *(*cMenuBar)( TRect ),
               TDeskTop *(*cDeskTop )( TRect )
             );
protected:
    /**
     * Creates the status line with the given size.
     */
    TStatusLine *(*createStatusLine)( TRect );
    /**
     * Creates the menu bar with the given size.
     */
    TMenuBar *(*createMenuBar)( TRect );
    /**
     * Creates the desk top with the given size.
     */
    TDeskTop *(*createDeskTop)( TRect );
};

/* ---------------------------------------------------------------------- */
/*      class TProgram                                                    */
/*                                                                        */
/*      Palette layout                                                    */
/*          1 = TBackground                                               */
/*       2- 7 = TMenuView and TStatusLine                                 */
/*       8-15 = TWindow(Blue)                                             */
/*      16-23 = TWindow(Cyan)                                             */
/*      24-31 = TWindow(Gray)                                             */
/*      32-63 = TDialog                                                   */
/* ---------------------------------------------------------------------- */

//  TApplication palette entries

    /** \var apColor
     * Use palette for color screen.
     * @see TProgram::appPalette
     */
const int
    apColor      = 0,

    /** \var apBlackWhite
     * Use palette for LCD screen.
     * @see TProgram::appPalette
     */
    apBlackWhite = 1,

    /** \var apMonochrome
     * Use palette for monochrome screen.
     * @see TProgram::appPalette
     */
    apMonochrome = 2;

class TDialog;
class TWindow;

/**
  * The mother of @ref TApplication.
  *
  * TProgram provides the basic template for all standard TVision
  * applications. All programs must be derived from TProgram or its immediate
  * derived class, @ref TApplication. @ref TApplication differs from TProgram
  * only for its constructor and destructor. However most applications will
  * be derived from @ref TApplication.
  * @short The mother of TApplication
  */
class TProgram : public TGroup, public virtual TProgInit
{
public:
    /**
     * Constructor.
     *
     * The TProgram constructor calls the @ref TProgInit constructor, passing
     * to it the addresses of three init functions. The @ref TProgInit
     * constructor creates a status line, menu bar, and desk top.
     *
     * If these calls are successful, the three objects are inserted into the
     * TProgram group. Variables @ref statusLine, @ref menuBar and
     * @ref deskTop are set to point at these new objects.
     *
     * The @ref TGroup constructor is also invoked to create a full screen
     * view; the video buffer and default palettes are initialized; and the
     * following state flags are set:
     *
     * <pre>
     * state = @ref sfVisible | @ref sfSelected | @ref sfFocused |
     *         @ref sfModal | @ref sfExposed;
     * </pre>
     */
    TProgram();
    /**
     * Destructor.
     *
     * Deletes the associated @ref deskTop, @ref menuBar, @ref statusLine
     * objects, and sets @ref application to 0.
     */
    virtual ~TProgram();
    /**
     * Returns True if the focus can be moved from one desktop view to another
     * one.
     *
     * It just returns `deskTop->valid(cmReleasedFocus)'.
     * @see TGroup::valid
     */
    virtual Boolean canMoveFocus();
    /**
     * Executes a dialog.
     *
     * `pD' points to the dialog. The dialog is executed only if it is valid.
     * @see TDialog::valid
     *
     * `data' is a pointer to the memory area where the dialog data will be
     * read before executing the dialog and where the dialog data will be
     * written after executing the dialog. If `data' is 0 no data area is
     * used.
     * @see TGroup::getData
     * @see TGroup::setData
     *
     * This method calls @ref TGroup::execView() to execute the dialog. The
     * dialog is destroyed before returning from the function, so a call to
     * delete is not necessary. executeDialog() returns cmCancel if the view
     * is not valid, otherwise it returns the return value of
     * @ref TGroup::execView().
     */
    virtual ushort executeDialog(TDialog* pD, void*data = 0);
    /**
     * Gets an event.
     *
     * This method collects events from the system like key events, mouse
     * events and timer events and returns them in the `event' structure.
     *
     * getEvent() first checks if @ref TProgram::putEvent() has generated a
     * pending event. If so, getEvent() returns that event. If there is no
     * pending event, getEvent() calls @ref TScreen::getEvent(). 
     *
     * If both calls return @ref evNothing, indicating that no user input is
     * available, getEvent() calls @ref TProgram::idle() to allow "background"
     * tasks to be performed while the application is waiting for user input.
     * @see @ref TProgram::idle()
     *
     * Before returning, getEvent() passes any @ref evKeyDown and
     * @ref evMouseDown events to the @ref statusLine for it to map into
     * associated @ref evCommand hot key events.
     */
    virtual void getEvent(TEvent& event);
    /**
     * Returns a reference to the standard TProgram palette.
     *
     * Returns the palette string given by the palette index in
     * @ref appPalette. TProgram supports three palettes. @ref appPalette is
     * initialized by @ref TProgram::initScreen().
     */
    virtual TPalette& getPalette() const;
    /**
     * Standard TProgram event handler.
     *
     * This method first checks for keyboard events. When it catches keys from
     * Alt-1 to Alt-9 it generates an @ref evBroadcast event with the `command'
     * field equal to cmSelectWindowNum and the `infoPtr' field in the range 1
     * to 9.
     *
     * Then it calls @ref TGroup::handleEvent().
     *
     * Last it checks for a cmQuit command in a @ref evCommand event. On
     * success it calls TGroup::endModal(cmQuit) to end the modal state. This
     * causes @ref TProgram::run() method to return. In most applications
     * this will result in program termination.
     * @see TGroup::endModal
     *
     * Method handleEvent() is almost always overridden to introduce handling
     * of commands that are specific to your own application.
     */
    virtual void handleEvent(TEvent& event);
    /**
     * Called when in idle state.
     *
     * This method is called whenever the library is in idle state, i.e. there
     * is not any event to serve. It allows the application to perform
     * background tasks while waiting for user input.
     *
     * The default idle() calls statusLine->update() to allow the status line
     * to update itself according to the current help context. Then, if the
     * command set has changed since the last call to idle(), an
     * @ref evBroadcast with a command value of cmCommandSetChanged is
     * generated to allow views that depend on the command set to enable or
     * disable themselves.
     * @see TStatusLine::update
     *
     * Note: in the original DOS version this method was called continously.
     * In my port this method is called about 10 times in a second. This
     * result in less CPU load.
     *
     * The user may redefine this method, for example, to update a clock in
     * the upper right corner of the screen, like the `demo' program does.
     */
    virtual void idle();
    /**
     * Initializes the screen.
     *
     * This method is called by the TProgram constructor and
     * @ref setScreenMode() every time the screen mode is initialized or
     * changed.
     * @see TProgram::TProgram
     *
     * Performs the updating and adjustment of screen mode-dependent variables
     * for shadow size, markers and application palette (color, monochrome or
     * black & white). The shadows are usually painted in the right and bottom
     * sides of menus and windows.
     */
    virtual void initScreen();
    /**
     * Called on out of memory condition.
     *
     * It is called from @ref validView() whenever @ref lowMemory() returns
     * True.
     *
     * This happens when there is few free memory. Of course this should
     * rarely happen. This method may be redefined to tell the user (by
     * calling @ref messageBox() for example) that there is not free memory
     * to end the current task.
     */
    virtual void outOfMemory();
    /**
     * Sets a pending event.
     *
     * Puts an event in the pending state, by storing a copy of the `event'
     * structure in the @ref pending variable, a static member of TProgram.
     *
     * Only one event is allowed to be pending. The next call to
     * @ref getEvent() will return this pending event even if there are
     * other events in the system queue to be handled.
     */
    virtual void putEvent( TEvent& event );
   /**
     * Runs TProgram.
     *
     * Executes TProgram by calling its method @ref execute(), which TProgram
     * inherits from TGroup.
     */
    virtual void run();
    /**
     * Inserts a window in the TProgram.
     */
    virtual TWindow* insertWindow(TWindow*);
    /**
     * Sets a new screen mode.
     *
     * The `mode' parameter can by one of the constants smCO80, smBW80 or
     * smMono, defined in `system.h' as follows. Optionally the value may be
     * or-ed with smFont8x8.
     *
     * <pre>
     * Constant  Value  Meaning
     *
     * smBW80    0x0002 Requires black & white screen, 80 columns
     * smCO80    0x0003 Requires color screen, 80 columns
     * smMono    0x0007 Requires monochrome screen
     * smFont8x8 0x0100 Requires small size font
     * </pre>
     *
     * Note: in my port this method only redraws the screen.
     */
    void setScreenMode( ushort mode );
    /**
     * Checks if a view is valid.
     *
     * Returns `p' if the view pointed by `p' is valid. Otherwise returns a
     * null pointer.
     *
     * First, if `p' is 0 the call returns 0.
     *
     * Next, if @ref lowMemory() returns True the view pointed by `p' is
     * released by calling @ref TObject::destroy() followed by
     * @ref outOfMemory() and the function returns 0.
     *
     * Last if a call to `p->valid(cmValid)' returns False the view pointed by
     * `p' is released and the function returns 0.
     * @see TView::valid
     *
     * Otherwise, the view is considered valid, and pointer `p' is returned.
     */
    TView *validView( TView *p );
    /**
     * Releases TProgram resources.
     *
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects.
     * @see TObject::shutDown
     *
     * This method releases all the resources allocated by TProgram. It sets
     * pointers @ref statusLine, @ref menuBar and @ref deskTop to 0 and then
     * calls @ref TGroup::shutDown() and @ref TVMemMgr::clearSafetyPool().
     */
    virtual void shutDown();
    /**
     * Stops the execution of the application.
     *
     * This method is empty. Will be redefined in TApplication which is a
     * child of TProgram.
     * @see TApplication::suspend
     */
    virtual void suspend() {}
    /**
     * Restores the execution of the application.
     *
     * This method is empty. Will be redefined in TApplication which is a
     * child of TProgram.
     * @see TApplication::resume
     */
    virtual void resume() {}
    /**
     * Creates a new status line.
     *
     * This method creates a standard @ref TStatusLine view and returns its
     * address.
     *
     * The address of this function is passed to the @ref TProgInit
     * constructor, which creates a @ref TStatusLine object for the
     * application and stores a pointer to it in the @ref statusLine static
     * member.
     *
     * initStatusLine() should never be called directly. initStatusLine() is
     * almost always overridden to instantiate a user defined @ref TStatusLine
     * instead of the default empty @ref TStatusLine.
     */
    static TStatusLine *initStatusLine( TRect );
    /**
     * Creates a new menu bar.
     *
     * This method creates a standard @ref TMenuBar view and returns its
     * address.
     *
     * The address of this function is passed to the @ref TProgInit
     * constructor, which creates a @ref TMenuBar object for the
     * application and stores a pointer to it in the @ref menuBar static
     * member.
     *
     * initMenuBar() should never be called directly. initMenuBar() is almost
     * always overridden to instantiate a user defined @ref TMenuBar
     * instead of the default empty @ref TMenuBar.
     */
    static TMenuBar *initMenuBar( TRect );
    /**
     * Creates a new desktop.
     *
     * This method creates a standard @ref TDeskTop view and returns its
     * address.
     *
     * The address of this function is passed to the @ref TProgInit
     * constructor, which creates a @ref TDeskTop object for the
     * application and stores a pointer to it in the @ref deskTop static
     * member.
     *
     * initDeskTop() should never be called directly. Few applications need to
     * redefine it to have a custom desktop, instead of the default empty
     * @ref TDeskTop.
     */
    static TDeskTop *initDeskTop( TRect );
    /**
     * A pointer to the current application, direct istance of TProgram or
     * istance of another class derived from TProgram, usually
     * @ref TApplication.
     * Set to this by the @ref TProgInit constructor.
     *
     * Only one TProgram object can exist at any time. In this way every
     * object can call TProgram methods even if it does't know its name.
     */
    static TProgram * application;
    /**
     * A pointer to the current status line object, set by a call to
     * @ref TProgInit::createStatusLine() in the TProgram constructor. The
     * resulting status line is inserted into the TProgram group.
     *
     * May be 0 if no status line exist.
     */
    static TStatusLine * statusLine;
    /**
     * A pointer to the current menu bar object, set by a call to
     * @ref TProgInit::createMenuBar() in the TProgram constructor. The
     * resulting menu bar is inserted into the TProgram group.
     *
     * May be 0 if no menu bar exist.
     */
    static TMenuBar * menuBar;
    /**
     * A pointer to the current desk top object, set by a call to
     * @ref TProgInit::createDeskTop() in the TProgram constructor. The
     * resulting desk top is inserted into the TProgram group.
     *
     * May be 0 if no desk top exist.
     */
    static TDeskTop * deskTop;
    /**
     * The current application palette. Indexes the default palette for this
     * application. The @ref TPalette object corresponding to appPalette is
     * returned by @ref getPalette().
     * This value is set automatically at startup by @ref initScreen().
     *
     * The following application palette constants are defined:
     *
     * <pre>
     * Constant     Value Meaning
     *
* @ref apColor      0     Use palette for color screen
* @ref apBlackWhite 1     Use palette for LCD screen
* @ref apMonochrome 2     Use palette for monochrome screen
     * </pre>
     */
     static int appPalette;
protected:
    /**
     * The current pending event.
     *
     * This structure contains the current pending event, if any exists. A
     * maximum of one pending event may be set by calling @ref putEvent().
     */
    static TEvent pending;
private:
    static const char * exitText;
};

#endif

#if defined( Uses_TApplication ) && !defined( __TApplication )
#define __TApplication

/**
 * The mother of all applications.
 *
 * TApplication is a shell around @ref TProgram and differs from it mainly in
 * constructor and destructor. TApplication provides the application with a
 * standard menu bar, a standard desktop and a standard status line.
 *
 * In any real application, you usually need to inherit TApplication and
 * redefine some of its methods. For example to add custom menus you must
 * redefine @ref TProgram::initMenuBar(). To add a custom status line, you
 * need to redefine @ref TProgram::initStatusLine(). In the same way, to add
 * a custom desktop you need to redefine @ref TProgram::initDeskTop().
 *
 * TVision's subsystems (the memory, video, event, system error, and history
 * list managers) are all static objects, so they are constructed before
 * entering into main, and are all destroyed on exit from main.
 *
 * Should you require a different sequence of subsystem initialization and
 * shut down, however, you can derive your application from TProgram, and
 * manually initialize and shut down the TVision subsystems along with your
 * own.
 * @short The mother of all applications
 */
class TApplication : public TScreen, public TProgram
{
protected:
    /**
     * Constructor.
     *
     * Initializes the basics of the library.
     *
     * This creates a default TApplication object by passing the three init
     * function pointers to the @ref TProgInit constructor.
     *
     * TApplication objects get a full-screen view,
     * @ref TProgram::initScreen() is called to set up various
     * screen-mode-dependent variables, and a screen buffer is allocated.
     *
     * @ref initDeskTop(), @ref initStatusLine(), and @ref initMenuBar() are
     * then called to create the three basic TVision views for your
     * application. Then the desk top, status line, and menu bar objects are
     * inserted in the application group.
     *
     * The @ref state data member is set to (@ref sfVisible | @ref sfSelected |
     * @ref sfFocused | @ref sfModal | @ref sfExposed).
     *
     * The @ref options data member is set to zero.
     *
     * Finally, the @ref application pointer is set (to this object) and
     * @ref initHistory() is called to initialize an associated
     * @ref THistory object.
     */
    TApplication();
    /**
     * Destructor.
     *
     * Destroys the application object and, via the base destructors, destroys
     * all its associated objects and frees all memory allocations.
     */
    virtual ~TApplication();
public:
    /**
     * Stops the execution of the application.
     *
     * Suspends the program, used usually before temporary exit.
     * In my port, by default, this function is called just after the user
     * presses Ctrl-Z to suspend the program.
     * @see TScreen::suspend
     */
    virtual void suspend();
    /**
     * Restores the execution of the application.
     *
     * Resumes the normal program execution.
     * In my port, by default, it is called after the user recovers the
     * execution of the program with `fg'.
     * @see TScreen::resume
     */
    virtual void resume();
    /**
     * Gets the next event from the event queue.
     *
     * Simply calls @ref TProgram::getEvent().
     */
    void getEvent(TEvent& event) { TProgram::getEvent(event); };
};

#endif
