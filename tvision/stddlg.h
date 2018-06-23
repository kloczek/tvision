/*
 * stddlg.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( __FILE_CMDS )
#define __FILE_CMDS

/** \file stddlg.h
 * stddlg.h
 */

const

//  Commands

int cmFileOpen    = 1001,   // Returned from TFileDialog when Open pressed
    cmFileReplace = 1002,   // Returned from TFileDialog when Replace pressed
    cmFileClear   = 1003,   // Returned from TFileDialog when Clear pressed
    cmFileInit    = 1004,   // Used by TFileDialog internally
    cmChangeDir   = 1005,   //
    cmRevert      = 1006,   // Used by TChDirDialog internally
#ifndef __UNPATCHED
    cmDirSelection= 1007,   //!! New event - Used by TChDirDialog internally
#endif

//  Messages

    cmFileFocused = 102,    // A new file was focused in the TFileList
    cmFileDoubleClicked     // A file was selected in the TFileList
            = 103;

#endif  // __FILE_CMDS

#if defined( Uses_TSearchRec ) && !defined( __TSearchRec )
#define __TSearchRec

#include <limits.h>

/* SS: some dos stuff */

#define FA_ARCH		0x01
#define FA_DIREC	0x02
#define FA_RDONLY	0x04

/** \struct ftime
 * Stores date and time information about a specified file.
 * @short Stores date and time information about a specified file
 */
struct  ftime   {
    /**
     * Undocumented.
     */
    unsigned    ft_tsec  : 5;   /* Two second interval */
    /**
     * Undocumented.
     */
    unsigned    ft_min   : 6;   /* Minutes */
    /**
     * Undocumented.
     */
    unsigned    ft_hour  : 5;   /* Hours */
    /**
     * Undocumented.
     */
    unsigned    ft_day   : 5;   /* Days */
    /**
     * Undocumented.
     */
    unsigned    ft_month : 4;   /* Months */
    /**
     * Undocumented.
     */
    unsigned    ft_year  : 7;   /* Year */
};

/** \struct TSearchRec
 * Internal structure used by @ref TFileCollection.
 * @see TFileInfoPane::file_block
 * @short Internal structure used by TFileCollection
 */
struct TSearchRec
{
    /**
     * Undocumented.
     */
    uchar attr;
    /**
     * Undocumented.
     */
    long time;
    /**
     * Undocumented.
     */
    long size;
    /**
     * Undocumented.
     */
    char name[PATH_MAX];
};

#endif  // Uses_TSearchRec

#if defined( Uses_TFileInputLine ) && !defined( __TFileInputLine )
#define __TFileInputLine

class TRect;
class TEvent;

/**
 * TFileInputLine implements a specialized @ref TInputLine allowing the input
 * and editing of file names, including optional paths and wild cards.
 *
 * A TFileInputLine object is associated with a file dialog box.
 * @short Allows the input and editing of file names, including optional paths
 * and wild cards
 */
class TFileInputLine : public TInputLine
{
public:
    /**
     * Calls TInputLine constructor TInputLine(bounds, aMaxLen) to create a
     * file input line with the given bounds and maximum length `aMaxLen'.
     *
     * @ref evBroadcast flag is set in the @ref eventMask.
     */
    TFileInputLine( const TRect& bounds, short aMaxLen );
    /**
     * Calls @ref TInputLine::handleEvent(), then handles broadcast
     * cmFileFocused events by copying the entered file name into the input
     * line's @ref TInputLine::data member and redrawing the view.
     *
     * If the edited name is a directory, the current file name in the owning
     * @ref TFileDialog object is appended first.
     */
    virtual void handleEvent( TEvent& event );
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TFileInputLine( StreamableInit );
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
inline ipstream& operator >> ( ipstream& is, TFileInputLine& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileInputLine*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileInputLine& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileInputLine* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileInputLine

#if defined( Uses_TFileCollection ) && !defined( __TFileCollection )
#define __TFileCollection

class TSearchRec;

/**
 * TFileCollection is a simple derivative of TSortedCollection offering a
 * sorted collection of file names.
 * @short Implements a sorted collection of file names
 */
class TFileCollection: public TSortedCollection
{
public:
    /**
     * Calls the base @ref TSortedCollection constructor to create a
     * collection with the given limit `aLimit' and delta `aDelta'.
     */
    TFileCollection( ccIndex aLimit, ccIndex aDelta) :
        TSortedCollection( aLimit, aDelta ) {}
    /**
     * Returns a pointer to the @ref TSearchRec object indexed by `index'
     * in this file collection.
     */
    TSearchRec *at( ccIndex index )
        { return (TSearchRec *)TSortedCollection::at( index ); }
    /**
     * Returns the index of the given @ref TSearchRec file `item' in this
     * file collection.
     */
    virtual ccIndex indexOf( TSearchRec *item )
        { return TSortedCollection::indexOf( item ); }
    /**
     * Removes (deletes) the given @ref TSearchRec file `item' from this file
     * collection. The space in the collection is not freed.
     */
    void remove( TSearchRec *item )
        { TSortedCollection::remove( item ); }
    /**
     * Removes (deletes) the given @ref TSearchRec file `item' from the
     * collection and frees the space in the collection.
     */
    void free( TSearchRec *item )
        { TSortedCollection::free( item ); }
    /**
     * Inserts the @ref TSearchRec file referenced by `item' into the
     * collection at the given `index' and moves the following items down
     * one position.
     *
     * The collection will be expanded by @ref delta if the insertion causes
     * the @ref limit to be exceeded.
     */
    void atInsert( ccIndex index, TSearchRec *item )
        { TSortedCollection::atInsert( index, item ); }
    /**
     * Replaces the TSearchRec file found at `index' with the given `item'.
     */
    void atPut( ccIndex index, TSearchRec *item )
        { TSortedCollection::atPut( index, item ); }
    /**
     * Inserts the @ref TSearchRec `item' into the collection, and adjusts the
     * other indexes if necessary.
     *
     * By default, insertions are made at the end of the collection. The index
     * of the inserted `item' is returned.
     */
    virtual ccIndex insert( TSearchRec *item )
        { return TSortedCollection::insert( item ); }
    /**
     * This iterator returns a pointer to the first @ref TSearchRec object
     * in the collection for which the `Test' function returns True.
     */
    TSearchRec *firstThat( ccTestFunc Test, void *arg );
    /**
     * This iterator scans the collection from last item to first. It returns
     * a pointer to the first item (that is, the nearest the end) in the
     * collection for which the `Test' function returns True.
     */
    TSearchRec *lastThat( ccTestFunc Test, void *arg );
private:
    virtual void freeItem( void *item )
        { delete (TSearchRec *)item; }
    /**
     * Performs a standard file string compare and returns a value depending
     * on the results.
     *
     * -# It returns 0 if the file names at `key1' and `key2' are the same
     * -# It returns > 0
     *    - if the `key1' name is lexicographically higher than that at `key2'
     *    - if `key1' name is the directory ".."
     *    - if `key1' is a directory and `key2' is not a directory
     * -# It returns < 0
     *	  - if the `key1' name is lexicographically lower than that at `key2'
     *	  - if `key2' references the directory ".."
     *	  - if `key2' is a directory and `key1' is not a directory
     */
    virtual int compare( void *key1, void *key2 );
    /**
     * Undocumented.
     */
    virtual const char *streamableName() const
        { return name; }
    /**
     * Called for each item in the collection. You'll need to override these
     * in order to read the items correctly.
     */
    virtual void *readItem( ipstream& );
    /**
     * Called for each item in the collection. You'll need to override these
     * in order to write the items correctly.
     */
    virtual void writeItem( void *, opstream& );
protected:
    /**
     * Undocumented.
     */
    TFileCollection( StreamableInit ) : TSortedCollection ( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TFileCollection& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileCollection*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileCollection& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileCollection* cl )
    { return os << (TStreamable *)cl; }

inline TSearchRec *TFileCollection::firstThat( ccTestFunc func, void *arg )
{
    return (TSearchRec *)TSortedCollection::firstThat( ccTestFunc(func), arg );
}

inline TSearchRec *TFileCollection::lastThat( ccTestFunc func, void *arg )
{
    return (TSearchRec *)TSortedCollection::lastThat( ccTestFunc(func), arg );
}

#endif  // Uses_TFileCollection


#if defined( Uses_TSortedListBox ) && !defined( __TSortedListBox )
#define __TSortedListBox

class TRect;
class TScrollBar;
class TEvent;
class TSortedCollection;

/**
 * TSortedListBox is a specialized @ref TListBox derivative that maintains its
 * items in a sorted sequence. It is intended as a base for other list box
 * classes.
 * @short A base for other list box classes
 */
class TSortedListBox: public TListBox
{
public:
    /**
     * Calls @ref TListBox constructor to create a list box with the given
     * size `bounds', number of columns `aNumCols', and vertical scroll bar
     * `aScrollBar'.
     * @see TListBox::TListBox
     *
     * Data member @ref shiftState is set to 0 and the cursor is set at the
     * first item.
     */
    TSortedListBox( const TRect& bounds,
                    ushort aNumCols,
                    TScrollBar *aScrollBar
                  );
    /**
     * Calls @ref TListBox::handleEvent(), then handles the special key and
     * mouse events used to select items from the list.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Calls @ref TListBox::newList() to delete the existing
     * @ref TSortedCollection object associated with this list box and
     * replace it with the collection given by `aList'.
     *
     * The first item of the new collection will receive the focus.
     */
    void newList( TSortedCollection *aList );
#ifndef __UNPATCHED
    void *read( ipstream& is );
#endif
    /**
     * Returns a pointer to the @ref TSortedCollection object currently
     * associated with this sorted list box. This gives access the the
     * private @ref items data member, a pointer to the items to be listed
     * and selected.
     * @see TListBox::list
     *
     * Derived sorted list box classes will typically override list() to
     * provide a pointer to objects of a class derived from
     * @ref TSortedCollection.
     */
    TSortedCollection *list();
protected:
    /**
     * Undocumented.
     */
    uchar shiftState;
private:
    /**
     * You must define this private member function in all derived classes to
     * provide a means of returning the key for the given string `s'. This
     * will depend on the sorting strategy adopted in your derived class. By
     * default, getKey() returns `s'.
     */
    virtual void *getKey( const char *s );
    short searchPos;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TSortedListBox( StreamableInit ) : TListBox ( streamableInit ) {}
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Undocumented.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TSortedListBox& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TSortedListBox*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TSortedListBox& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TSortedListBox* cl )
    { return os << (TStreamable *)cl; }

inline TSortedCollection *TSortedListBox::list()
{
    return (TSortedCollection *)TListBox::list();
}

#endif  // Uses_TSortedListBox

#if defined( Uses_TFileList ) && !defined( __TFileList )
#define __TFileList

class TRect;
class TScrollBar;
class TEvent;

/**
 * TFileList implements a sorted two-column list box of file names (held in a
 * @ref TFileCollection object). You can select (highlight) a file name by
 * mouse or keyboard cursor actions.
 *
 * TFileList inherits most of its functionality from @ref TSortedListBox.
 * The following commands are broadcast by TFileList:
 *
 * <pre>
 * Constant            Value Meaning
 *
 * cmFileFocused       102   A new file was focused in object
 * cmFileDoubleClicked 103   A file was selected in the TFileList object
 * </pre>
 * @short Implements a sorted two-column list box of file names; you can
 * select a file name by mouse or keyboard cursor actions
 */
class TFileList : public TSortedListBox
{
public:
    /**
     * Calls the @ref TSortedListBox constructor to create a two-column
     * TFileList object with the given bounds and, if `aScrollBar' is
     * non-zero, a vertical scrollbar.
     * @see TSortedListBox::TSortedListBox
     */
    TFileList( const TRect& bounds,
               TScrollBar *aScrollBar
             );
    /**
     * Deletes the file list.
     */
    ~TFileList();
    /**
     * Focuses the given item in the list. Calls
     * @ref TSortedListBox::focusItem() and broadcasts a cmFileFocused event.
     */
    virtual void focusItem( short item );
    /**
     * Undocumented.
     */
    virtual void selectItem( short item );
    /**
     * Grabs the @ref TSearchRec object at `item' and sets the file name in
     * `dest'.
     * "\\" is appended if the name is a directory.
     */
    virtual void getText( char *dest, short item, short maxLen );
    /**
     * Calls @ref TSortedListBox::newList() to delete the existing
     * @ref TFileCollection object associated with this list box and replace
     * it with the file collection given by `aList'.
     *
     * The first item of the new collection will receive the focus.
     */
    void newList( TFileCollection *aList );
    /**
     * Allows the separate submission of a relative or absolute path in the
     * `dir' argument. Either "/" or "\\" can be used as subdirectory
     * separators (but "\\" is converted to "/" for output).
     */
    void readDirectory( const char *dir, const char *wildCard );
    /**
     * Expands the `wildCard' string to generate the file collection
     * associated with this file list. The resulting @ref TFileCollection
     * object (a sorted set of @ref TSearchRec objects) is assigned to the
     * private @ref items data member (accessible via the @ref list() member
     * function).
     *
     * If too many files are generated, a warning message box appears.
     * readDirectory() knows about file attributes and will not generate
     * hidden file names.
     */
    void readDirectory( const char *wildCard );
    /**
     * Undocumented.
     */
    virtual ushort dataSize();
    /**
     * Undocumented.
     */
    virtual void getData( void *rec );
    /**
     * Undocumented.
     */
    virtual void setData( void *rec );
    /**
     * Returns the private @ref items data member, a pointer to the
     * @ref TFileCollection object currently associated with this file list
     * box.
     */
    TFileCollection *list();
private:
    virtual void *getKey( const char *s );
    static const char * tooManyFiles;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TFileList( StreamableInit ) : TSortedListBox ( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TFileList& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileList*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileList& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileList* cl )
    { return os << (TStreamable *)cl; }

inline void TFileList::newList( TFileCollection *f )
{
    TSortedListBox::newList( f );
}

inline TFileCollection *TFileList::list()
{
    return (TFileCollection *)TSortedListBox::list();
}

#endif  // Uses_TFileList


#if defined( Uses_TFileInfoPane ) && !defined( __TFileInfoPane )
#define __TFileInfoPane

class TRect;
class TEvent;

/**
 * TFileInfoPane implements a simple, streamable view for displaying file
 * information in the owning file dialog box.
 * @short Implements a simple, streamable view for displaying file information
 * in the owning file dialog box
 */
class TFileInfoPane : public TView
{
public:
    /**
     * Calls TView constructor TView(bounds) to create a file information pane
     * with the given bounds.
     * @see TView::TView
     *
     * @ref evBroadcast flag is set in @ref TView::eventMask.
     */
    TFileInfoPane( const TRect& bounds );
    /**
     * Draws the file info pane in the default palette. The block size and
     * date/time stamp are displayed.
     */
    virtual void draw();
    /**
     * Returns the default palette.
     */
    virtual TPalette& getPalette() const;
    /**
     * Calls @ref TView::handleEvent(), then handles broadcast cmFileFocused
     * events (triggered when a new file is focused in a file list) by
     * displaying the file information pane.
     */
    virtual void handleEvent( TEvent& event );
private:
    /**
     * The file name and attributes for this info pane. @ref TSearchRec is
     * defined as follows:
     *
     * <pre>
     * struct TSearchRec
     * {
     *     uchar attr;
     *     long time;
     *     long size;
     *     char name[MAXFILE+MAXEXT-1];
     * };
     * </pre>
     *
     * where the fields have their obvious DOS file meanings.
     */
    TSearchRec file_block;
    static const char * const months[13];
    static const char * pmText;
    static const char * amText;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TFileInfoPane( StreamableInit ) : TView ( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TFileInfoPane& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileInfoPane*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileInfoPane& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileInfoPane* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileInfoPane

#if defined( Uses_TFileDialog ) && !defined( __TFileDialog )
#define __TFileDialog

const
    int fdOKButton  = 0x0001,      // Put an OK button in the dialog
    fdOpenButton    = 0x0002,      // Put an Open button in the dialog
    fdReplaceButton = 0x0004,      // Put a Replace button in the dialog
    fdClearButton   = 0x0008,      // Put a Clear button in the dialog
    fdHelpButton    = 0x0010,      // Put a Help button in the dialog
    fdNoLoadDir     = 0x0100;      // Do not load the current directory
                                   // contents into the dialog at Init.
                                   // This means you intend to change the
                                   // WildCard by using SetData or store
                                   // the dialog on a stream.

class TEvent;
class TFileInputLine;
class TFileList;

/**
 * TFileDialog implements a file dialog box, history pick list, and input line
 * from which file names (including wildcards) can be input, edited, selected,
 * and opened for editing.
 *
 * The following commands are returned by TFileDialog when executed with a
 * call to @ref TGroup::execView():
 *
 * <pre>
 * Constant       Value Meaning
 *
 * cmFileOpen     1001  Returned when Open pressed
 * cmFileReplace  1002  Returned when Replace pressed
 * cmFileClear    1003  Returned when Clear pressed
 * cmFileInit     1004  Used by @ref valid()
 * </pre>
 * @short Implements a file dialog box, history pick list, and input line from
 * which file names (including wildcards) can be input, edited, selected, and
 * opened for editing
 */
class TFileDialog : public TDialog
{
public:
    /**
     * Creates a fixed-size, framed dialog box with the given title `aTitle'.
     *
     * A @ref TFileInputLine object (referenced by the @ref fileName data
     * member) is created and inserted, labeled with `inputName' and with its
     * @ref TFileInputLine::data field set to `aWildCard'.
     *
     * The `aWildCard' argument is expanded (if necessary) to provide a
     * @ref TFileList object, referenced by the @ref fileList data member.
     *
     * A @ref THistory object is created and inserted with the given history
     * ID `histID'.
     *
     * A vertical scroll bar is created and inserted. Depending on the
     * `aOptions' flags, the appropriate buttons are set up. These options
     * perform the specified operations:
     *
     * <pre>
     * Constant        Value  Meaning
     *
     * fdOKButton      0x0001 Put an OK button in the dialog
     *
     * fdOpenButton    0x0002 Put an Open button in the dialog
     *
     * fdReplaceButton 0x0004 Put a Replace button in the dialog
     *
     * fdClearButton   0x0008 Put a Clear button in the dialog
     *
     * fdHelpButton    0x0010 Put a Help button in the dialog
     *
     * fdNoLoadDir     0x0100 Do not load the current directory contents into
     *                        the dialog when intialized. This means you
     *                        intend to change the wildcard by using
     *                        @ref setData() or intend to store the dialog on
     *                        a stream.
     * </pre>
     *
     * A @ref TFileInfoPane object is created and inserted. If the
     * fdNoLoadDir flag is not set, the files in the current directory are
     * loaded into the file list.
     */
    TFileDialog( const char *aWildCard, const char *aTitle,
                 const char *inputName, ushort aOptions, uchar histId );
    /**
     * Deletes directory, then destroys the file dialog.
     */
    ~TFileDialog();
    /**
     * Undocumented.
     */
    virtual void getData( void *rec );
    /**
     * Takes the fileName->data field and expands it to a full path format.
     * The result is set in `s'.
     * @see TFileDialog::fileName
     * @see TFileInputLine::data
     */
    void getFileName( char *s );
    /**
     * Calls @ref TDialog::handleEvent(), then handles cmFileOpen,
     * cmFileReplace and cmFileClear events.
     *
     * These all call @ref TView::endModal() and pass their commands to the
     * view that opened the file dialog.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Undocumented.
     */
    virtual void setData( void *rec );
    /**
     * Returns True if `command' is cmValid, indicating a successful
     * construction. Otherwise calls @ref TDialog::valid().
     *
     * If this returns True, the current @ref fileName is checked for
     * validity.
     *
     * Valid names will return True. Invalid names invoke an
     * "Invalid file name" message box and return False.
     */
    virtual Boolean valid( ushort command );
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects.
     *
     * shutDown() is overridden in many classes to ensure the proper setting
     * of related data members when @ref destroy() is called.
     */
    virtual void shutDown();
    /**
     * Pointer to the associated input line.
     */
    TFileInputLine *fileName;
    /**
     * Pointer to the associated file list.
     */
    TFileList *fileList;
    /**
     * The current path and file name.
     */
    char wildCard[PATH_MAX];
    /**
     * The current directory.
     */
    const char *directory;
private:
    void readDirectory();
    Boolean checkDirectory( const char * );
    static const char * filesText;
    static const char * openText;
    static const char * okText;
    static const char * replaceText;
    static const char * clearText;
    static const char * cancelText;
    static const char * helpText;
    static const char * invalidDriveText;
    static const char * invalidFileText;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TFileDialog( StreamableInit ) :
        TWindowInit( TFileDialog::initFrame ),
        TDialog ( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TFileDialog& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileDialog*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileDialog& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileDialog* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TFileDialog


#if defined( Uses_TDirEntry ) && !defined( __TDirEntry )
#define __TDirEntry

/**
 * TDirEntry is a simple class providing directory paths and descriptions.
 *
 * TDirEntry objects are stored in @ref TDirCollection objects.
 * @short Simple class providing directory paths and descriptions
 */
class TDirEntry
{
public:
    /**
     * Undocumented.
     */
    TDirEntry( const char *, const char * );
    /**
     * Undocumented.
     */
    ~TDirEntry();
    /**
     * Returns the current directory (the value of the private member
     * directory).
     */
    char *dir() { return directory; }
    /**
     * Returns the current display text (the value of the private member
     * displayText).
     */
    char *text() { return displayText; }
private:
    char *displayText;
    char *directory;
};

inline TDirEntry::TDirEntry( const char *txt, const char *dir ) :
    displayText( newStr( txt ) ), directory( newStr( dir ) )
{
}

inline TDirEntry::~TDirEntry()
{
    delete displayText;
    delete directory;
}

#endif  // Uses_TDirEntry

#if defined( Uses_TDirCollection ) && !defined( __TDirCollection )
#define __TDirCollection

class TDirEntry;

/**
 * TDirCollection is a simple @ref TCollection derivative used for storing
 * @ref TDirEntry objects.
 *
 * TDirCollection is a streamable class, inheriting @ref TStreamable from
 * its base class @ref TCollection.
 * @short Simple TCollection derivative used for storing TDirEntry objects
 */
class TDirCollection : public TCollection
{
public:
    /**
     * Calls the base @ref TCollection constructor to create a directory
     * collection with the given @ref limit `aLimit' and @ref delta `aDelta'.
     */
    TDirCollection( ccIndex aLimit, ccIndex aDelta) :
        TCollection( aLimit, aDelta ) {}
    /**
     * Returns a pointer to the @ref TDirEntry object indexed by `index' in
     * this directory collection.
     * @see TCollection::at
     */
    TDirEntry *at( ccIndex index )
        { return (TDirEntry *)TCollection::at( index );}
    /**
     * Returns the index of the given `item' in this directory collection.
     * @see TCollection::indexOf
     */
    virtual ccIndex indexOf( TDirEntry *item )
        { return TCollection::indexOf( item ); }
    /**
     * Removes (deletes) the given `item' from this collection. The space in
     * the collection is not freed.
     * @see TCollection::remove
     */
    void remove( TDirEntry *item )
        { TCollection::remove( item ); }
    /**
     * Removes (deletes) the given `item' from the collection and frees the
     * space in the collection.
     * @see TCollection::free
     */
    void free( TDirEntry *item )
        { TCollection::free( item ); }
    /**
     * Inserts the given `item' into the collection at the given `index' and
     * moves the following items down one position. The collection will be
     * expanded by @ref delta if the insertion causes the @ref limit to be
     * exceeded.
     * @see TCollection::atInsert
     */
    void atInsert( ccIndex index, TDirEntry *item )
        { TCollection::atInsert( index, item ); }
    /**
     * Replaces the item at `index' with the given `item'.
     * @see TCollection::atPut
     */
    void atPut( ccIndex index, TDirEntry *item )
        { TCollection::atPut( index, item ); }
    /**
     * Inserts the `item' into the collection, and adjust the other indexes
     * if necessary. By default, insertions are made at the end of the
     * collection. The index of the inserted item is returned.
     * @see TCollection::insert
     */
    virtual ccIndex insert( TDirEntry *item )
        { return TCollection::insert( item ); }
    /**
     * This iterator returns a pointer to the first @ref TDirEntry object
     * in the collection for which the `Test' function returns True.
     */
    TDirEntry *firstThat( ccTestFunc Test, void *arg );
    /**
     * This iterator scans the collection from the last @ref TDirEntry object
     * to first. It returns a pointer to the first (that is, the nearest to
     * the end) item in the collection for which the `Test' function returns
     * True.
     */
    TDirEntry *lastThat( ccTestFunc Test, void *arg );
private:
    virtual void freeItem( void *item )
        { delete (TDirEntry *)item; }
    virtual const char *streamableName() const
        { return name; }
    /**
     * Called for each item in the collection.
     *
     * You'll need to override these in everything derived from
     * @ref TCollection or @ref TSortedCollection in order to read the items
     * correctly.
     *
     * TSortedCollection already overrides this function.
     */
    virtual void *readItem( ipstream& );
    /**
     * Called for each item in the collection.
     *
     * You'll need to override these in everything derived from
     * @ref TCollection or @ref TSortedCollection in order to write the
     * items correctly.
     *
     * @ref TSortedCollection already overrides this function.
     */
    virtual void writeItem( void *, opstream& );
protected:
    /**
     * Undocumented.
     */
    TDirCollection( StreamableInit ) : TCollection ( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TDirCollection& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TDirCollection*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDirCollection& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDirCollection* cl )
    { return os << (TStreamable *)cl; }

inline TDirEntry *TDirCollection::firstThat( ccTestFunc func, void *arg )
{
    return (TDirEntry *)TCollection::firstThat( ccTestFunc(func), arg );
}

inline TDirEntry *TDirCollection::lastThat( ccTestFunc func, void *arg )
{
    return (TDirEntry *)TCollection::lastThat( ccTestFunc(func), arg );
}

#endif  // Uses_TDirCollection


#if defined( Uses_TDirListBox ) && !defined( __TDirListBox )
#define __TDirListBox

class TRect;
class TScrollBar;
class TEvent;
class TDirCollection;

/**
 * TDirListBox is a specialized derivative of @ref TListBox for displaying and
 * selecting directories stored in a @ref TDirCollection object.
 *
 * By default, these are displayed in a single column with an optional
 * vertical scroll bar.
 * @short Specialized derivative of TListBox for displaying and selecting
 * directories stored in a TDirCollection object
 */
class TDirListBox : public TListBox
{
public:
   /**
    * Calls TListBox::TListBox(bounds, 1, aScrollBar) to create a
    * single-column list box with the given bounds and vertical scroll bar.
    * @see TListBox::TListBox
    */
    TDirListBox( const TRect& bounds, TScrollBar *aScrollBar );
    /**
     * Calls its base destructor to dispose of the list box.
     * @see TListBox::~TListBox
     */
    ~TDirListBox();
    /**
     * Grabs the text string at index `item' and copies it to `dest'.
     */
    virtual void getText( char *dest, short item, short maxLen );
    /**
     * Handles double-click mouse events with putEvent(cmChangeDir).
     * @see putEvent
     *
     * This allows a double click to change to the selected directory. Other
     * events are handled by @ref TListBox::handleEvent().
     */
//    virtual void handleEvent( TEvent& );
    /**
     * Returns True if `item' is selected, otherwise returns False.
     */
    virtual Boolean isSelected( short item );
    /**
     * Undocumented.
     */
    virtual void selectItem( short item );
    /**
     * Deletes the existing @ref TDirEntry object associated with this list
     * box and replaces it with the file collection given by `aList'.
     *
     * The first item of the new collection will receive the focus.
     */
    void newDirectory( const char *aList );
    /**
     * By default, calls the ancestral @ref TListBox::setState().
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Returns a pointer to the @ref TDirCollection object currently
     * associated with this directory list box.
     */
    TDirCollection *list();
    /**
     * Undocumented.
     */
    static const char * pathDir;
    /**
     * Undocumented.
     */
    static const char * firstDir;
    /**
     * Undocumented.
     */
    static const char * middleDir;
    /**
     * Undocumented.
     */
    static const char * lastDir;
    /**
     * Undocumented.
     */
    static const char * graphics;
private:
    void showDrives( TDirCollection * );
    void showDirs( TDirCollection * );
    char dir[PATH_MAX];
    ushort cur;
    static const char * drives;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TDirListBox( StreamableInit ): TListBox( streamableInit ) {}
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
inline ipstream& operator >> ( ipstream& is, TDirListBox& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TDirListBox*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDirListBox& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TDirListBox* cl )
    { return os << (TStreamable *)cl; }

inline TDirCollection *TDirListBox::list()
{
    return (TDirCollection *)TListBox::list();
}

#endif  // Uses_TDirListBox

#if defined( Uses_TChDirDialog ) && !defined( __TChDirDialog )
#define __TChDirDialog

const
    int cdNormal = 0x0000, // Option to use dialog immediately
    cdNoLoadDir  = 0x0001, // Option to init the dialog to store on a stream
    cdHelpButton = 0x0002; // Put a help button in the dialog

class TEvent;
class TInputLine;
class TDirListBox;
class TButton;

/**
 * TChDirDialog implements a dialog box labeled "Change Directory", used to
 * change the current working directory.
 * @see TDialog
 *
 * An input line is provided to accept a directory name from the user. A
 * history window and directory list box with vertical scroll bar are
 * available to show recent directory selections and a tree of all available
 * directories.
 * @see TDirListBox
 * @see THistoryWindow
 * @see TInputLine
 *
 * Mouse and keyboard selections can be made in the usual way by highlighting
 * and clicking. The displayed options are:
 *
 * -# Directory name
 * -# Directory tree
 * -# OK (the default)
 * -# Chdir
 * -# Revert
 * -# Help
 *
 * Method @ref TChDirDialog::handleEvent() generates the appropriate commands
 * for these selections.
 *
 * Note: @ref TDirListBox is a friend of TChDirDialog, so that the member
 * functions of @ref TDirListBox can access the private members of
 * TChDirDialog.
 * @short Dialog box used to change the current working directory
 */
class TChDirDialog : public TDialog
{
public:
    friend class TDirListBox;
    /**
     * Constructor.
     *
     * Creates a change directory dialog object with the given history
     * identifier `histId'. The `aOptions' argument is a bitmap of the
     * following flags:
     *
     * <pre>
     * Constant     Value Meaning
     *
     * cdNormal     0x00  Option to use the dialog immediately.
     *
     * cdNoLoadDir  0x01  Option to initialize the dialog without loading the
     *                    current directory into the dialog. Used if you
     *                    intend using @ref setData() to reset the directory
     *                    or prior to storage on a stream.
     *
     * cdHelpButton 0x02  Option to put a help button in the dialog.
     * </pre>
     *
     * The constructor creates and inserts:
     *
     * -# a @ref TInputLine object (labeled "Directory ~n~ame")
     * -# a @ref THistory object
     * -# a vertical scroll bar, see @ref TScrollBar
     * -# a TDirListBox object (labeled "Directory ~t~ree")
     * -# three buttons "O~K~", "~C~hdir" and "~R~evert", see @ref TButton
     *
     * If `aOptions' has the cdHelpButton flag set, a fourth help button is
     * created. Unless the cdNoLoadDir option is set, the dialog box is loaded
     * with the current directory.
     *
     * Unsigned short `histId' is an arbitrary positive integer used to
     * identify which history set to use. The library can use multiple history
     * sets and all views with the same history identifier will share the same
     * history set.
     */
    TChDirDialog( ushort aOptions, ushort histId );
    /**
     * Returns the size of the data record of this dialog.
     *
     * By default, dataSize() returns 0. Override to return the size (in
     * bytes) of the data used by @ref getData() and @ref setData() to store
     * and retrieve dialog box input data.
     * @see TGroup::dataSize
     */
    virtual ushort dataSize();
    /**
     * Reads the data record of this dialog.
     *
     * By default, getData() does nothing. Override to copy @ref dataSize()
     * bytes from the view to `rec'. Used in combination with @ref dataSize()
     * and @ref setData() to store and retrieve dialog box input data.
     * @see TGroup::getData
     */
    virtual void getData( void *rec );
    /**
     * Standard TChDirDialog event handler.
     *
     * Calls @ref TDialog::handleEvent() then processes cmRevert (restore
     * previously current directory) and cmChangeDir (switch to selected
     * directory) events. The dialog is redrawn if necessary.
     */
    virtual void handleEvent( TEvent& );
    /**
     * Writes the data record of this dialog.
     *
     * By default, setData() does nothing. Override to copy @ref dataSize()
     * bytes from `rec' to the view. Used in combination with @ref dataSize()
     * and @ref getData() to store and retrieve dialog box input data.
     * @see TGroup::setData
     */
    virtual void setData( void *rec );
    /**
     * Checks if the command `command' is valid.
     *
     * The return value is True if `command' is not cmOK.  Otherwise (the OK
     * button was pressed) the return value depends on path existence. The
     * function returns True if the path exists. An invalid directory invokes
     * the "Invalid directory" message box and returns False.
     * @see TDialog::valid
     */
    virtual Boolean valid( ushort command );
    /**
     * Releases TChDirDialog resources.
     *
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when @ref destroy() is called.
     * @see TObject::shutDown
     *
     * This method releases all the resources allocated by the TChDirDialog
     * dialog. It sets private pointers TChDirDialog::dirList,
     * TChDirDialog::dirInput, TChDirDialog::okButton, and
     * TChDirDialog::chDirButton to 0 and then calls @ref TDialog::shutDown().
     */
    virtual void shutDown();
private:
    void setUpDialog();
    TInputLine *dirInput;
    TDirListBox *dirList;
    TButton *okButton;
    TButton *chDirButton;
    static const char * changeDirTitle;
    static const char * dirNameText;
    static const char * dirTreeText;
    static const char * okText;
    static const char * chdirText;
    static const char * revertText;
    static const char * helpText;
    static const char * drivesText;
    static const char * invalidText;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Constructor.
     *
     * Used to recover the view from a stream.
     */
    TChDirDialog( StreamableInit ) :
        TWindowInit( TChDirDialog::initFrame ),
        TDialog( streamableInit ) {}
    /**
     * Used to store the view in a stream.
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Used to recover the view from a stream.
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Creates a new TChDirDialog.
     *
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TChDirDialog& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TChDirDialog*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TChDirDialog& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TChDirDialog* cl )
    { return os << (TStreamable *)cl; }

#endif  // Uses_TChDirDialog
