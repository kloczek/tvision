/*
 * editors.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_EDIT_COMMAND_CODES )
#define TV_EDIT_COMMAND_CODES

/** \file editors.h
 * editors.h
 */

const
  int ufUpdate = 0x01,
  ufLine   = 0x02,
  ufView   = 0x04;

const
  int smExtend = 0x01,
  smDouble = 0x02;

const unsigned
  sfSearchFailed = UINT_MAX;

const
  int cmFind        = 82,
  cmReplace     = 83,
  cmSearchAgain = 84;

const
  int cmCharLeft    = 500,
  cmCharRight   = 501,
  cmWordLeft    = 502,
  cmWordRight   = 503,
  cmLineStart   = 504,
  cmLineEnd     = 505,
  cmLineUp      = 506,
  cmLineDown    = 507,
  cmPageUp      = 508,
  cmPageDown    = 509,
  cmTextStart   = 510,
  cmTextEnd     = 511,
  cmNewLine     = 512,
  cmBackSpace   = 513,
  cmDelChar     = 514,
  cmDelWord     = 515,
  cmDelStart    = 516,
  cmDelEnd      = 517,
  cmDelLine     = 518,
  cmInsMode     = 519,
  cmStartSelect = 520,
  cmHideSelect  = 521,
  cmIndentMode  = 522,
  cmUpdateTitle = 523;

  /** \var edOutOfMemory
   * @see TEditor::doSearchReplace
   */
const int
  edOutOfMemory   = 0,

  /** \var edReadError
   * @see TEditor::doSearchReplace
   */
  edReadError     = 1,

  /** \var edWriteError
   * @see TEditor::doSearchReplace
   */
  edWriteError    = 2,

  /** \var edCreateError
   * @see TEditor::doSearchReplace
   */
  edCreateError   = 3,

  /** \var edSaveModify
   * @see TEditor::doSearchReplace
   */
  edSaveModify    = 4,

  /** \var edSaveUntitled
   * @see TEditor::doSearchReplace
   */
  edSaveUntitled  = 5,

  /** \var edSaveAs
   * @see TEditor::doSearchReplace
   */
  edSaveAs        = 6,

  /** \var edFind
   * @see TEditor::doSearchReplace
   */
  edFind          = 7,

  /** \var edSearchFailed
   * @see TEditor::doSearchReplace
   */
  edSearchFailed  = 8,

  /** \var edReplace
   * @see TEditor::doSearchReplace
   */
  edReplace       = 9,

  /** \var edReplacePrompt
   * @see TEditor::doSearchReplace
   */
  edReplacePrompt = 10;

  /** \var efCaseSensitive
   * Default to case-sensitive search.
   * @see TEditor::editorFlags
   */
const int
  efCaseSensitive   = 0x0001,

  /** \var efWholeWordsOnly
   * Default to whole words only search.
   * @see TEditor::editorFlags
   */
  efWholeWordsOnly  = 0x0002,

  /** \var efPromptOnReplace
   * Prompt on replace.
   * @see TEditor::editorFlags
   */
  efPromptOnReplace = 0x0004,

  /** \var efReplaceAll
   * Replace all occurrences.
   * @see TEditor::editorFlags
   */
  efReplaceAll      = 0x0008,

  /** \var efDoReplace
   * Do replace.
   * @see TEditor::editorFlags
   */
  efDoReplace       = 0x0010,

  /** \var efBackupFiles
   * Create backup files with a trailing ~ on saves.
   * @see TEditor::editorFlags
   */
  efBackupFiles     = 0x0100;

  /** \var maxLineLength
   * Maximum allowed line length for text in a TEditor view.
   */
const
  int maxLineLength = 256;

#endif  // TV_EDIT_COMMAND_CODES

/** \enum TEditorDialog
 * The TEditorDialog data type is a pointer to function returning ushort
 * and taking one int argument and a variable number of additional
 * arguments.
 *
 * Since dialog boxes are very application-dependent, a @ref TEditor object
 * does not display its own dialog boxes directly. Instead it controls
 * them through this function pointer.
 *
 * The various dialog values, passed in the first int argument, are
 * self-explanatory: @ref edOutOfMemory, @ref edReadError, @ref edWriteError,
 * @ref edCreateError, @ref edSaveModify, @ref edSaveUntitled, @ref edSaveAs,
 * @ref edFind, @ref edSearchFailed, @ref edReplace and @ref edReplacePrompt.
 * @see TEditor::editorDialog
 */
typedef ushort (*TEditorDialog)( int, ... );

/** \fn defEditorDialog( int dialog, ... )
 * Since dialog boxes are very application-dependent, a @ref TEditor object
 * does not display its own dialog boxes directly. Instead it controls
 * them through the @ref TEditorDialog function pointer.
 * @see TEditor::editorDialog
 *
 * This is the default dialog; it simply returns cmCancel.
 */
ushort defEditorDialog( int dialog, ... );

#if defined( Uses_TIndicator ) && !defined( TV_TIndicator )
#define TV_TIndicator

class TRect;

/**
 * TIndicator is the line and column counter in the lower left corner of the
 * edit window.
 * It is initialized by the @ref TEditWindow constructor and passed as the
 * fourth argument to the @ref TEditor constructor.
 * @short The line and column counter in the lower left corner of the edit
 * window
 */
class TIndicator : public TView
{
public:
    /**
     * Creates a TIndicator object.
     */
    TIndicator( const TRect& );
    /**
     * Draws the indicator. If variable @ref modified is True, a special
     * character (ASCII value 15, a star on PC graphic adapters) is displayed.
     */
    virtual void draw();
    /**
     * Returns the TIndicator default palette.
     */
    virtual TPalette& getPalette() const;
    /**
     * Draws the indicator in the frame-dragging color if the view is being
     * dragged.
     */
    virtual void setState( ushort, Boolean );
    /**
     * Method called by @ref TEditor to update and display the values of the
     * data members of the associated TIndicator object.
     * @see TIndicator::location
     * @see TIndicator::modified
     */
    void setValue( const TPoint& aLocation, Boolean aModified );
    /**
     * Undocumented.
     */
    static char dragFrame;
    /**
     * Undocumented.
     */
    static char normalFrame;
protected:
    /**
     * Stores the location to display. Updated by a TEditor.
     */
    TPoint location;
    /**
     * True if the associated TEditor has been modified.
     */
    Boolean modified;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Undocumented.
     */
    TIndicator( StreamableInit );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TIndicator& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TIndicator*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TIndicator& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TIndicator* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TIndicator


#if defined( Uses_TEditor ) && !defined( TV_TEditor )
#define TV_TEditor

class TRect;
class TScrollBar;
class TIndicator;
class TEvent;

/**
 * This class implements a text editor.
 *
 * TEditor is the base class for all editors. It implements most of the
 * editor's functionality. If a TEditor object is created, it allocates a
 * buffer of the given size out of the heap. The buffer is initially empty.
 * @short Implements a text editor
 */
class TEditor : public TView
{
public:
    /**
     * Constructor.
     *
     * Calls TView::TView(bounds) by creating a view with the given bounds.
     * The @ref hScrollBar, @ref vScrollBar, @ref indicator and @ref bufSize
     * data members are set from the given arguments.
     * @see TView::TView
     *
     * The scroll bar and indicator arguments can be set to 0 if you do not
     * want these objects. The following default values are set:
     *
     * <pre>
     * Variable   Value                     Description
     *
     * canUndo    True                      @ref TEditor::canUndo
     * selecting  False                     @ref TEditor::selecting
     * overwrite  False                     @ref TEditor::overwrite
     * autoIndent False                     @ref TEditor::autoIndent
     * lockCount  0                         @ref TEditor::lockCount
     * keyState   0                         @ref TEditor::keyState
     * growMode   @ref gfGrowHiX | @ref gfGrowHiY     @ref TView::growMode
     * options    @ref ofSelectable              @ref TView::options
     * eventMask  @ref evMouseDown | @ref evKeyDown |
     *            @ref evCommand | @ref evBroadcast   @ref TView::eventMask
     * </pre>
     *
     * `aBufSize' is the buffer initial size; 0 is its common value.
     * The buffer is allocated and cleared. If insufficient memory exists,
     * the @ref edOutOfMemory dialog box is displayed using
     * @ref editorDialog(), and @ref isValid data member is set False.
     * Otherwise @ref isValid is set True.
     *
     * The data members associated with the editor buffer are initialized in
     * the obvious way: @ref bufLen to 0, @ref gapLen to @ref bufSize,
     * @ref selStart to 0, @ref modified to False, and so on.
     */
    TEditor( const TRect& bounds, TScrollBar *aHScrollBar,
    	TScrollBar *aVScrollBar, TIndicator *aIndicator, uint aBufSize );
    /**
     * Destructor.
     *
     * Destroys the editor and deletes the buffer.
     */
    virtual ~TEditor();
    /**
     * Used internally by @ref TObject::destroy() to ensure correct
     * destruction of derived and related objects. shutDown() is overridden
     * in many classes to ensure the proper setting of related data members
     * when @ref destroy() is called.
     */
    virtual void shutDown();
    /**
     * Returns the p'th character in the file, factoring in the gap.
     */
    char bufChar( uint p );
    /**
     * Returns the offset into buffer of the p'th character in the file,
     * factoring in the gap.
     */
    uint bufPtr( uint p );
    /**
     * Changes the views bounds, adjusting the @ref delta value and redrawing
     * the scrollbars and view if necessary.
     *
     * Overridden to ensure the file stays within view if the parent size
     * changes.
     */
    virtual void changeBounds( const TRect& bounds );
    /**
     * Used by @ref handleEvent() to provide basic editing operations by
     * converting various key events to command events. You can change or
     * extend these default key bindings by overriding the convertEvent()
     * member function.
     */
    virtual void convertEvent( TEvent& event );
    /**
     * Returns True if the @ref cursor (insertion point) is visible within the
     * view.
     */
    Boolean cursorVisible();
    /**
     * Deletes the selection if one exists. For example, after a successful
     * @ref clipCopy(), the selected block is deleted.
     */
    void deleteSelect();
    /**
     * Deletes the buffer.
     */
    virtual void doneBuffer();
    /**
     * Overrides @ref TView::draw() to draw the editor.
     */
    virtual void draw();
    /**
     * Returns the default TEditor palette. Override if you wish to change the
     * palette of the editor.
     */
    virtual TPalette& getPalette() const;
    /**
     * Provides the event handling for the editor. Override if you wish to
     * extend the commands the editor handles.
     *
     * The default handler calls @ref TView::handleEvent(), then converts all
     * relevant editing key events to command events by calling
     * @ref convertEvent().
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Allocates a buffer of size @ref bufSize and sets @ref buffer to point
     * at it.
     */
    virtual void initBuffer();
    /**
     * This is the lowest-level text insertion member function. It inserts
     * `length' bytes of text from the array `p' (starting at p[offset]) into
     * the buffer (starting at the @ref curPtr).
     *
     * If `allowUndo' is set True, insertBuffer() records undo information. If
     * `selectText' is set True, the inserted text will be selected.
     * insertBuffer() returns True for a successful operation.
     *
     * Failure invokes a suitable dialog box and returns False. insertBuffer()
     * is used by @ref insertFrom() and @ref insertText(); you will seldom
     * need to call it directly.
     */
    Boolean insertBuffer( char *p, uint offset, uint length,
    	Boolean allowUndo, Boolean selectText );
    /**
     * Inserts the current block-marked selection from the argument `editor'
     * into this editor.
     * This member function implements @ref clipCut(), @ref clipCopy(), and
     * @ref clipPaste().
     *
     * The implementation may help you understand the insertFrom() and
     * @ref insertBuffer() functions:
     *
     * <pre>
     * Boolean TEditor::insertFrom(TEditor *editor)
     * {
     *     return insertBuffer(editor->buffer,
     *         editor->bufPtr(editor->selStart),
     *         editor->selEnd - editor->selStart, canUndo, isClipboard());
     * }
     * </pre>
     *
     * Note the the `allowUndo' argument is set to the value of the data
     * member @ref canUndo. The `selectText' argument will be True if there
     * is an active clipboard for this editor.
     */
    virtual Boolean insertFrom( TEditor *editor );
    /**
     * Copies `length' bytes from the given text into this object's buffer.
     *
     * If `selectText' is True, the inserted text will be selected. This is a
     * simplified version of @ref insertBuffer().
     */
    Boolean insertText( const void *text, uint length, Boolean selectText );
    /**
     * Move column `x' and line `y' to the upper-left corner of the editor.
     */
    void scrollTo( int x, int y );
    /**
     * Search for the given string `findStr' in the editor @ref buffer
     * (starting at @ref curPtr) with the given options in `opts'. The valid
     * options are:
     *
     * <pre>
     * Name              Value  Description
     *
* @ref efCaseSensitive   0x0001 Case sensitive search
* @ref efWholeWordsOnly  0x0002 Whole words only search
     * </pre>
     *
     * Returns True if a match is found; otherwise returns False. If a match
     * is found, the matching text is selected.
     */
    Boolean search( const char *findStr, ushort opts );
    /**
     * Should be called before changing the buffer size to `newSize'. It
     * should return True if the the buffer can be of this new size.
     *
     * By default, it returns True if `newSize' is less than or equal to
     * @ref bufSize.
     */
    virtual Boolean setBufSize( uint newSize );
    /**
     * Enables or disables the given command depending on whether `enable' is
     * True or False and whether the editor is @ref sfActive.
     * @see TView::state
     *
     * The command is always disabled if the editor is not the selected view.
     * Offers a convenient alternative to @ref enableCommands() and
     * @ref disableCommands().
     */
    void setCmdState( ushort command, Boolean enable );
    /**
     * Sets the selection to the given offsets into the file, and redraws the
     * view as needed.
     *
     * This member function will either place the cursor in front of or behind
     * the selection, depending on the value (True or False, respectively) of
     * `curStart'.
     */
    void setSelect( uint newStart, uint newEnd, Boolean curStart );
    /**
     * Overrides @ref TView::setState() to hide and show the indicator and
     * scroll bars.
     *
     * It first calls @ref TView::setState() to enable and disable commands.
     * If you wish to enable and disable additional commands, override
     * @ref updateCommands() instead. This is called whenever the command
     * states should be updated.
     */
    virtual void setState( ushort aState, Boolean enable );
    /**
     * Forces the cursor to be visible. If `center' is True, the cursor is
     * forced to be in the center of the screen in the y (line) direction.
     * The x (column) position is not changed.
     */
    void trackCursor( Boolean center );
    /**
     * Undoes the changes since the last cursor movement.
     */
    void undo();
    /**
     * Called whenever the commands should be updated. This is used to enable
     * and disable commands such as cmUndo and cmCopy.
     */
    virtual void updateCommands();
    /**
     * Returns whether the view is valid for the given command. By default it
     * returns the value of @ref isValid, which is True if @ref buffer is
     * not 0.
     */
    virtual Boolean valid( ushort command );
    /**
     * Calculates and returns the actual cursor position by examining the
     * characters in the buffer between `p' and `target'.
     * Any tab codes encountered are counted as spaces modulo the tab setting.
     * @see TEditor::charPtr
     */
    uint charPos( uint p, uint target );
    /**
     * The reverse of @ref charPos().
     * Calculates and returns the buffer position corresponding to a cursor
     * position.
     */
    uint charPtr( uint p, uint target );
    /**
     * Returns False if this editor has no active clipboard.
     * Otherwise, copies the selected text from the editor to the clipboard
     * using clipboard->insertFrom(this).
     * @see TEditor::insertFrom
     *
     * The selected text is deselected (highlight removed) and the view
     * redrawn. Returns True if all goes well.
     */
    Boolean clipCopy();
    /**
     * The same as for @ref clipCopy(), but the selected text is deleted after
     * being copied to the clipboard.
     */
    void clipCut();
    /**
     * The reverse of @ref clipCopy(): the contents of the clipboard (if any)
     * are copied to the current position of the editor using
     * insertFrom(clipboard).
     * @see TEditor::insertFrom
     */
    void clipPaste();
    /**
     * If `delSelect' is True and a current selection exists, the current
     * selection is deleted; otherwise, the range `startPtr' to `endPtr' is
     * selected and deleted.
     */
    void deleteRange( uint startPtr, uint endPtr, Boolean delSelect );
    /**
     * If @ref updateFlags data member is 0, nothing happens. Otherwise, the
     * view and its scrollbars are updated and redrawn depending on the state
     * of the @ref updateFlags bits.
     *
     * For example, if ufView is set, the view is redrawn with
     * @ref drawView(). If the view is @ref sfActive, the command set is
     * updated with @ref updateCommands().
     * @see TView::state
     *
     * After these updates, @ref updateFlags is set to 0.
     */
    void doUpdate();
    /**
     * Can be used in both find and find/replace operations, depending on the
     * state of @ref editorFlags data member bits and user-dialog box
     * interactions.
     *
     * If @ref efDoReplace is not set, doSearchReplace() acts as a simple
     * search for @ref findStr data member, with no replacement. Otherwise,
     * this function aims at replacing occurrences of @ref findStr with
     * @ref replaceStr data member.
     *
     * In all cases, if the target string is not found, an
     * editorDialog(@ref edSearchFailed) call is invoked.
     * @see TEditor::editorDialog
     *
     * If @ref efPromptOnReplace is set in @ref editorFlags, an
     * @ref edReplacePrompt dialog box appears. Replacement then depends on
     * the user response. If @ref efReplaceAll is set, replacement proceeds
     * for all matching strings without prompting until a cmCancel command is
     * detected.
     */
    void doSearchReplace();
    /**
     * Draws `count' copies of the line at `linePtr', starting at line
     * position `y'.
     */
    void drawLines( int y, uint count, uint linePtr );
    /**
     * Formats the line at `linePtr' in the given color and sets result in
     * `buff'. Used by @ref drawLines().
     */
    void formatLine(void *buff, uint linePtr, int x, ushort color);
    /**
     * Finds occurrences of the existing @ref findStr or a new user-supplied
     * string. find() displays an editor dialog inviting the input of a find
     * string or the acceptance of the existing @ref findStr.
     *
     * If a new find string is entered, it will replace the previous
     * @ref findStr (unless the user cancels).
     * find() first creates a @ref TFindDialogRec object defined as follows:
     *
     * <pre>
     * struct TFindDialogRec
     * {
     *     TFindDialogRec(const char *str, ushort flgs)
     *     {
     *         strcpy(find, str);
     *         options = flgs;
     *     }
     *     char find[80];
     *     ushort options;
     * };
     * </pre>
     *
     * The constructor is called with `str' set to the current @ref findStr,
     * and `flgs' set to the current @ref editorFlags.
     *
     * The @ref edFind editor dialog then invites change or acceptance of the
     * @ref findStr. Finally, @ref doSearchReplace() is called for a simple
     * find-no-replace (@ref efDoReplace switched off).
     */
    void find();
    /**
     * Returns the buffer character pointer corresponding to the point `m' on
     * the screen.
     */
    uint getMousePtr( TPoint m );
    /**
     * Returns True if a selection has been made; that is, if @ref selStart
     * does not equal @ref selEnd. If these two data members are equal, no
     * selection exists, and False is returned.
     */
    Boolean hasSelection();
    /**
     * Sets selecting to False and hides the current selection with
     * setSelect(curPtr, curPtr, False).
     * @see TEditor::curPtr
     * @see TEditor::setSelect
     */
    void hideSelect();
    /**
     * Returns True if this editor has an attached clipboard; otherwise
     * returns False.
     */
    Boolean isClipboard();
    /**
     * Returns the buffer pointer (offset) of the end of the line containing
     * the given pointer `p'.
     */
    uint lineEnd( uint p );
    /**
     * Moves the line containing the pointer (offset) `p' up or down `count'
     * lines depending on the sign of `count'.
     */
    uint lineMove( uint p, int count );
    /**
     * Returns the buffer pointer (offset) of the start of the line contaning
     * the given pointer `p'.
     */
    uint lineStart( uint p );
    /**
     * Increments the semaphore @ref lockCount.
     */
    void lock();
    /**
     * Inserts a newline at the current pointer. If @ref autoIndent is set,
     * appropriate tabs (if needed) are also inserted at the start of the new
     * line.
     */
    void newLine();
    /**
     * Returns the buffer offset for the character following the one at the
     * given offset `p'.
     */
    uint nextChar( uint p );
    /**
     * Returns the buffer offset for the start of the line following the line
     * containing the given offset `p'.
     */
    uint nextLine( uint p );
    /**
     * Returns the buffer offset for the start of the word following the word
     * containing the given offset `p'.
     */
    uint nextWord( uint p );
    /**
     * Returns the buffer offset for the character preceding the one at the
     * given offset `p'.
     */
    uint prevChar( uint p );
    /**
     * Returns the buffer offset for the start of the line preceding the line
     * containing the given offset `p'.
     */
    uint prevLine( uint p );
    /**
     * Returns the buffer offset corresponding to the start of the word
     * preceding the word containing the given offset `p'.
     */
    uint prevWord( uint p );
    /**
     * Replaces occurrences of the existing @ref findStr (or a new
     * user-supplied find string) with the existing @ref replaceStr (or a
     * new user-supplied replace string).
     *
     * replace() displays an editor dialog inviting the input of both strings
     * or the acceptance of the existing @ref findStr and @ref replaceStr.
     * If new strings are entered, they will replace the previous values
     * (unless the user cancels).
     *
     * replace() first creates a @ref TReplaceDialogRec object defined as
     * follows:
     *
     * <pre>
     * struct TReplaceDialogRec
     * {
     *     TReplaceDialogRec(const char *str, const char *rep, ushort flgs)
     *     {
     *         strcpy(find, str);
     *         strcpy(replace, rep);
     *         options = flgs;
     *     }
     *     char find[80];
     *     char replace[80];
     *     ushort options;
     * };
     * </pre>
     *
     * The constructor is called with `str' and `rep' set to the current
     * @ref findStr and @ref replaceStr, and with `flg' set to the current
     * @ref editorFlags.
     *
     * The @ref edReplace editor dialog then invites change or acceptance of
     * the two strings.
     * @see TEditor::editorDialog
     *
     * Finally, @ref doSearchReplace() is called for a find-replace operation
     * (@ref efDoReplace switched on).
     */
    void replace();
    /**
     * Sets @ref bufLen to `length', then adjusts @ref gapLen and @ref limit
     * accordingly. @ref selStart, @ref selEnd, @ref curPtr, delta.x, delta.y,
     * @ref drawLine, @ref drawPtr, @ref delCount, and @ref insCount are all
     * set to 0.
     * @see TEditor::delta
     *
     * @ref curPos is set to @ref delta, @ref modified is set to False, and
     * the view is updated and redrawn as needed.
     *
     * The TEditor constructor calls setBufLen(0). setBufLen() is also used
     * by @ref insertBuffer().
     * @see TEditor::TEditor
     */
    void setBufLen( uint length );
    /**
     * Calls @ref setSelect() and repositions @ref curPtr to the offset `p'.
     * Some adjustments may be made, depending on the value of `selectMode',
     * if @ref curPtr is at the beginning or end of a selected text.
     */
    void setCurPtr( uint p, uchar selectMode );
    /**
     * Called by @ref handleEvent() when a Ctrl-K Ctrl-B selection is detected.
     * Hides the previous selection and sets @ref selecting to True.
     */
    void startSelect();
    /**
     * Toggles the @ref overwrite data member from True to False and from
     * False to True. Changes the cursor shape by calling @ref setState().
     */
    void toggleInsMode();
    /**
     * Decrements the data member @ref lockCount until it reaches the value
     * 0, at which point a @ref doUpdate() is triggered. The lock/unlock
     * mechanism prevents over-frequent redrawing of the view.
     */
    void unlock();
    /**
     * Sets `aFlags' in the @ref updateFlags data member. If @ref lockCount
     * is 0, calls @ref doUpdate().
     */
    void update( uchar aFlags );
    /**
     * Called by @ref handleEvent() in response to a cmScrollBarChanged
     * broadcast event. If the scroll bar's current value is different from
     * `d', the scroll bar is redrawn.
     */
    void checkScrollBar( const TEvent& event, TScrollBar *p, int& d );
    /**
     * Pointer to the horizontal scroll bar; 0 if the scroll bar does not
     * exist.
     */
    TScrollBar *hScrollBar;
    /**
     * Pointer to the vertical scroll bar; 0 if the scroll bar does not exist.
     */
    TScrollBar *vScrollBar;
    /**
     * Pointer to the indicator; 0 if the indicator does not exist.
     */
    TIndicator *indicator;
    /**
     * Pointer to the buffer used to hold the text.
     */
    char *buffer;
    /**
     * Size of the buffer (in bytes).
     */
    uint bufSize;
    /**
     * The amount of text stored between the start of the buffer and the
     * current cursor position.
     */
    uint bufLen;
    /**
     * The size of the "gap" between the text before the cursor and the text
     * after the cursor.
     */
    uint gapLen;
    /**
     * The offset of the start of the text selected by Ctrl-K Ctrl-B.
     */
    uint selStart;
    /**
     * The offset of the end of the text selected by Ctrl-K Ctrl-K.
     */
    uint selEnd;
    /**
     * Offset of the cursor.
     */
    uint curPtr;
    /**
     * The line/column location of the cursor in the file.
     */
    TPoint curPos;
    /**
     * The top line and leftmost column shown in the view.
     */
    TPoint delta;
    /**
     * The maximum number of columns to display, and the number of lines in
     * the file. Records the limits of the scroll bars.
     */
    TPoint limit;
    /**
     * Column position on the screen where inserted characters are drawn.
     * Used internally by @ref draw().
     */
    int drawLine;
    /**
     * Buffer offset corresponding to the current cursor. Used internally by
     * @ref draw().
     */
    uint drawPtr;
    /**
     * Number of characters in the end of the gap that were deleted from the
     * text. Used to implement @ref undo().
     */
    uint delCount;
    /**
     * Number of characters inserted into the text since the last cursor
     * movement. Used to implement @ref undo().
     */
    uint insCount;
    /**
     * True if the view is valid. Used by the @ref valid() function.
     */
    Boolean isValid;
    /**
     * True if the editor is to support undo. Otherwise False.
     */
    Boolean canUndo;
    /**
     * True if the buffer has been modified.
     */
    Boolean modified;
    /**
     * True if the editor is in selecting mode (that is, Ctrl-K Ctrl-B has
     * been pressed).
     */
    Boolean selecting;
    /**
     * True if in overwrite mode; otherwise the editor is in insert mode.
     */
    Boolean overwrite;
    /**
     * True if the editor is in autoindent mode.
     */
    Boolean autoIndent;
    /**
     * The TEditorDialog data type is a pointer to function returning ushort
     * and taking one int argument and a variable number of additional
     * arguments. It is defined in `editors.h' as follows:
     *
     * <pre>
     * typedef ushort (*TEditorDialog)(int, ...);
     * </pre>
     *
     * Variable editorDialog is a function pointer used by TEditor objects to
     * display various dialog boxes.
     *
     * Since dialog boxes are very application-dependent, a TEditor object
     * does not display its own dialog boxes directly. Instead it controls
     * them through this function pointer.
     *
     * The various dialog values, passed in the first int argument, are
     * self-explanatory: @ref edOutOfMemory, @ref edReadError,
     * @ref edWriteError, @ref edCreateError, @ref edSaveModify,
     * @ref edSaveUntitled, @ref edSaveAs, @ref edFind, @ref edSearchFailed,
     * @ref edReplace and @ref edReplacePrompt.
     *
     * The default editorDialog, @ref defEditorDialog(), simply returns
     * cmCancel.
     */
    static TEditorDialog editorDialog;
    /**
     * Variable editorFlags contains various flags for use in the editor:
     *
     * <pre>
     * Name              Value  Description
     *
* @ref efCaseSensitive   0x0001 Default to case-sensitive search
* @ref efWholeWordsOnly  0x0002 Default to whole words only search
* @ref efPromptOnReplace 0x0004 Prompt on replace
* @ref efReplaceAll      0x0008 Replace all occurrences
* @ref efDoReplace       0x0010 Do replace
* @ref efBackupFiles     0x0100 Create backup files with a trailing ~ on saves
     * </pre>
     *
     * The default value is @ref efBackupFiles | @ref efPromptOnReplace.
     */
    static ushort editorFlags;
    /**
     * Stores the last string value used for a find operation.
     */
    static char findStr[maxFindStrLen];
    /**
     * Stores the last string value of a replace operation.
     */
    static char replaceStr[maxReplaceStrLen];
    /**
     * Pointer to the clipboard.
     *
     * Any TEditor can be the clipboard; it just needs be assigned to this
     * variable. The clipboard should not support undo (i.e., its @ref canUndo
     * should be false).
     */
    static TEditor * clipboard;
    /**
     * Holds the lock count semaphore that controls when a view is redrawn.
     * lockCount is incremented by @ref lock() and decremented by
     * @ref unlock().
     */
    uchar lockCount;
    /**
     * A set of flags indicating the state of the editor.
     * @ref doUpdate() and other member functions examine these flags to
     * determine whether the view needs to be redrawn.
     */
    uchar updateFlags;
    /**
     * Indicates that a special key, such as Ctrl-K, has been pressed. Used
     * by @ref handleEvent() to keep track of "double" control keys such
     * as Ctrl-K-H and Ctrl-K-B.
     */
    int keyState;
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     *
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TEditor( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads an editor object from the input stream `is'.
     */
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Called to create an object in certain stream reading situations.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TEditor& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TEditor*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TEditor& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TEditor* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TEditor

#if defined( Uses_TMemo ) && !defined( TV_TMemo )
#define TV_TMemo

class TEvent;

/**
 * Data structure used by @ref TMemo.
 * @short Data structure used by TMemo
 */
struct TMemoData
{
    /**
     * Undocumented.
     */
    ushort length;
    /**
     * Undocumented.
     */
    char buffer[1];
};

/**
 * TMemo, which is derived from TEditor, is designed for insertion into a
 * dialog or form.
 * @short Derived from TEditor and designed for insertion into a dialog or
 * form
 */
class TMemo : public TEditor
{
public:
    /**
     * Constructor.
     * @see TEditor::TEditor
     */
    TMemo( const TRect& bounds, TScrollBar *aHScrollBar,
    	TScrollBar *aVScrollBar, TIndicator *aIndicator, uint aBufSize );
    virtual void getData( void *rec );
    virtual void setData( void *rec );
    /**
     * Used with @ref getData() and @ref setData(), which are inherited from
     * TView and redefined. By default it returns (sizeof(ushort) +
     * @ref bufSize).
     */
    virtual ushort dataSize();
    /**
     * Returns the default memo palette.
     */
    virtual TPalette& getPalette() const;
    /**
     * Prevents TMemo from handling kbTab key events; otherwise handles
     * events the same as a @ref TEditor.
     * @see TEditor::handleEvent
     */
    virtual void handleEvent( TEvent& );
private:
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     *
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TMemo( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& );
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
inline ipstream& operator >> ( ipstream& is, TMemo& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TMemo*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TMemo& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TMemo* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TMemo


#if defined( Uses_TFileEditor ) && !defined( TV_TFileEditor )
#define TV_TFileEditor

#include <limits.h>

class TRect;
class TScrollBar;
class TIndicator;
class TEvent;

/**
 * TFileEditor is a specialized derivative of TEditor for editing the contents
 * of a file.
 * @short Specialized derivative of TEditor for editing the contents of a file
 */
class TFileEditor : public TEditor
{
public:
    /**
     * The name of the file being edited.
     */
    char fileName[PATH_MAX];
    /**
     * Creates a TFileEditor object with the given scroll bars and indicator
     * and loads the contents of the file given in `aFileName'.
     *
     * If the file is not found or invalid, an error message will be displayed
     * and the object's @ref valid() member function will return False.
     *
     * The @ref options variable is set to @ref ofSelectable and the
     * @ref eventMask is set to allow the handling of broadcast events.
     *
     * Any of `aHScrollBar', `aVScrollBar' or `aIndicator' arguments can be
     * set to 0 if you do not want them.
     */
    TFileEditor( const TRect& bounds,
                 TScrollBar *aHScrollBar,
                 TScrollBar *aVScrollBar,
                 TIndicator *aIndicator,
                 const char *aFileName
               );
    /**
     * Deletes the buffer.
     */
    virtual void doneBuffer();
    /**
     * Calls @ref TEditor::handleEvent(), then handles cmSave and cmSaveAs
     * commands.
     *
     * The cmSave command invokes @ref save(); the cmSaveAs command invokes
     * @ref saveAs().
     */
    virtual void handleEvent( TEvent& );
    /**
     * Allocates @ref bufSize bytes of memory for the file editor buffer.
     */
    virtual void initBuffer();
    /**
     * Reads the @ref fileName file from disk and checks for errors.
     *
     * Returns True if all is well; otherwise returns False. Depending on the
     * reason for failure, the @ref edOutOfMemory or @ref edReadError dialog
     * box is displayed with a call to @ref editorDialog().
     */
    Boolean loadFile();
    /**
     * Calls @ref saveAs() if the file being edited is "Untitled" (that is, no
     * @ref fileName is allocated) and returns the return value from
     * @ref saveAs().
     *
     * If there is a valid @ref fileName, @ref saveFile() is invoked, and
     * save() returns the return value of @ref saveFile().
     */
    Boolean save();
    /**
     * Invokes the @ref edSaveAs dialog, which prompts for a "save as" file
     * name. If a valid file name is supplied, the current text will be saved
     * with this name using the @ref saveFile() member function.
     * @see editorDialog
     *
     * The file editor's owner is informed of this event via a broadcast
     * cmUpdateTitle message. saveAs() returns True if the @ref saveFile()
     * call is successful, otherwise False is returned.
     *
     * False is also returned if the @ref edSaveAs dialog is cancelled.
     */
    Boolean saveAs();
    /**
     * Saves the @ref fileName file to disk. Returns False if the save fails;
     * otherwise returns True. If @ref editorFlags has the @ref efBackupFiles
     * bit set, a backup file is created.
     *
     * The @ref edCreateError or @ref edWriteError dialog box will be
     * displayed to indicate the reason for failure if save fails.
     * @see editorDialog
     */
    Boolean saveFile();
    /**
     * Overrides @ref TEditor::setBufSize() to grow and shrink the buffer.
     *
     * Will grow and shrink the buffer in 4K byte increments. @ref gapLen
     * data member is adjusted appropriately.
     */
    virtual Boolean setBufSize( uint newSize );
    /**
     * Undocumented.
     */
    virtual void shutDown();
    /**
     * Calls @ref TEditor::updateCommands(), then enables the cmSave and
     * cmSaveAs commands. These commands are only valid if the selected view
     * is an editor, otherwise they should be disabled.
     */
    virtual void updateCommands();
    /**
     * Overrides @ref TEditor::valid() to warn that the file might need saving
     * before the program exits.
     *
     * The @ref edSaveUntitled or @ref edSaveModify dialogs are displayed as
     * appropriate.
     * @see editorDialog
     *
     * Returns False if the user cancels the save.
     */
    virtual Boolean valid( ushort );
private:
    static const char * backupExt;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     *
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TFileEditor( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& );
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
inline ipstream& operator >> ( ipstream& is, TFileEditor& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TFileEditor*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileEditor& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TFileEditor* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TFileEditor


#if defined( Uses_TEditWindow ) && !defined( TV_TEditWindow )
#define TV_TEditWindow

class TFileEditor;

/**
 * TEditWindow is a window designed to hold a @ref TFileEditor or the
 * clipboard.
 *
 * It will change its title to display the file name being edited and will
 * initialize scroll bars and an indicator for the editor.
 * @see TIndicator
 * @short Designed to hold a TFileEditor or the clipboard
 */
class TEditWindow : public TWindow
{
public:
    /**
     * Creates a TEditWindow object that will edit the given file name with
     * window number `aNumber'. Initializes a framed, tileable window with
     * scroll bars and an indicator. If `fileName' is 0, it is assumed to be
     * an untitled file.
     *
     * Note: if @ref editor data member is equal to @ref TEditor::clipboard
     * static data member, the editor is assumed to be the clipboard.
     */
    TEditWindow( const TRect& bounds, const char *fileName, int aNumber );
    /**
     * Overrides @ref TWindow::close() to hide rather than close the window
     * if the editor is a clipboard.
     */
    virtual void close();
    /**
     * Returns the name of the file being edited, or "Clipboard" if the editor
     * is the clipboard.
     */
    virtual const char *getTitle( short );
    /**
     * Handles cmUpdateTitle to redraw the frame of the window. Used in
     * @ref TFileEditor::saveAs() to change the title of the window if the
     * file being edited changes names.
     */
    virtual void handleEvent( TEvent& event );
    /**
     * Undocumented.
     */
    virtual void sizeLimits( TPoint& min, TPoint& max );
    /**
     * Pointer to the editor associated with this window.
     */
    TFileEditor *editor;
private:
    static const char * clipboardTitle;
    static const char * untitled;
    virtual const char *streamableName() const
        { return name; }
protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     *
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TEditWindow( StreamableInit );
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& );
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
inline ipstream& operator >> ( ipstream& is, TEditWindow& cl )
    { return is >> static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TEditWindow*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TEditWindow& cl )
    { return os << static_cast<TStreamable&>(cl); }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TEditWindow* cl )
    { return os << static_cast<TStreamable *>(cl); }

#endif  // Uses_TEditWindow


#if defined( Uses_TFindDialogRec ) && !defined( TV_TFindDialogRec )
#define TV_TFindDialogRec

#include <string.h>

/**
 * Data record used by TEditor.
 * @see TEditor::find
 * @see TReplaceDialogRec
 * @see TView::dataSize
 * @see TView::getData
 * @see TView::setData
 */
struct TFindDialogRec
{
    /**
     * Undocumented.
     */
    TFindDialogRec( const char *str, ushort flgs )
        {
        strcpy( find, str );
        options = flgs;
        }
    /**
     * Undocumented.
     */
    char find[maxFindStrLen];
    /**
     * Undocumented.
     */
    ushort options;
};

#endif  // Uses_TFindDialogRec

#if defined( Uses_TReplaceDialogRec ) && !defined( TV_TReplaceDialogRec )
#define TV_TReplaceDialogRec

#include <string.h>

/**
 * Data record used by TEditor.
 * @see TEditor::replace
 * @see TFindDialogRec
 * @see TView::dataSize
 * @see TView::getData
 * @see TView::setData
 */
struct TReplaceDialogRec
{
    /**
     * Undocumented.
     */
    TReplaceDialogRec( const char *str, const char *rep, ushort flgs )
        {
        strcpy( find, str );
        strcpy( replace, rep );
        options = flgs;
        }
    /**
     * Undocumented.
     */
    char find[maxFindStrLen];
    /**
     * Undocumented.
     */
    char replace[maxReplaceStrLen];
    /**
     * Undocumented.
     */
    ushort options;
};

#endif  // Uses_TReplaceDialogRec
