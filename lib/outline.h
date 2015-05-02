/*
 * outline.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TOutlineViewer ) && !defined( __TOutlineViewer )
#define __TOutlineViewer

/** \file outline.h
 * outline.h
 */

const int
  ovExpanded = 0x01,
  ovChildren = 0x02,
  ovLast     = 0x04;

const int
  cmOutlineItemSelected = 301;

/**
 * @ref TOutline objects use records of type TNode to hold the lists of linked
 * strings that form the outline.
 * @see TOutlineViewer
 * @short Implements a node of the outline
 */
class TNode
{
public:
    /**
     * Creates a node for an outline list.
     */
    inline TNode(char* aText);
    /**
     * Creates a node for an outline list. Sets the new node's @ref text,
     * @ref childList, and @ref next fields to `aText', `aChildren', and
     * `aNext', respectively.
     */
    inline TNode(char* aText, TNode* aChildren, TNode* aNext,
        Boolean initialState = True);
    /**
     * Disposes of an outline node.
     */
    virtual ~TNode();
    /**
     * Field next points to the next node at the same outline level as the
     * current node.
     */
    TNode* next;
    /**
     * Each node record holds the text for that item in the outline in its
     * text field.
     */
    char* text;
    /**
     * Field childList points to the first in a list of subordinate nodes, or
     * holds 0 if there are no items subordinate to the node.
     */
    TNode* childList;
    /**
     * Field expanded is True if the outline view shows the subordinate views
     * listed in @ref childList or False if the subordinate nodes are hidden.
     */
    Boolean expanded;
};

inline TNode::TNode(char* aText) :
    next(0), text(newStr(aText)), childList(0), expanded(True)
{
}

inline TNode::TNode( char* aText, TNode* aChildren,
                     TNode* aNext, Boolean initialState ) :
    next(aNext), text(newStr(aText)),
    childList(aChildren), expanded(initialState)
{
}

inline TNode::~TNode() {
  delete [] text;
}

/* ------------------------------------------------------------------------*/
/*      class TOutlineViewer                                               */
/*                                                                         */
/*      Palette layout                                                     */
/*        1 = Normal color                                                 */
/*        2 = Focus color                                                  */
/*        3 = Select color                                                 */
/*        4 = Not expanded color                                           */
/* ------------------------------------------------------------------------*/

class TRect;
class TScrollBar;
class TEvent;

/**
 * The outline viewer object type provides the abstract methods for
 * displaying, expanding, and iterating the items in an outline.
 *
 * TOutlineViewer, however, makes no assumptions about the actual items in the
 * outline. Descendants of TOutlineViewer, such as @ref TOutline, display
 * outlines of specific kinds of items.
 * @short Implements an outline viewer
 */
class TOutlineViewer : public TScroller
{
public:
    /**
     * Constructs an outline viewer object by first calling the constructor
     * inherited from @ref TScroller, passing `bounds', `aHScrollBar' and
     * `aVScrollBar'. Sets @ref growMode to @ref gfGrowHiX + @ref gfGrowHiY
     * and sets @ref foc data member to zero.
     */
    TOutlineViewer(const TRect& bounds, TScrollBar* aHScrollBar,
        TScrollBar* aVScrollBar);
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TOutlineViewer(StreamableInit s);
    /**
     * adjust() is an abstract method that descendant outline viewer types must
     * override to show the child nodes of `node' if `expand' is True, or hide
     * them if `expand' is False. Called when the user expands or collapses
     * `node'. If @ref hasChildren() returns False for `node', adjust() will
     * never be called for that node.
     */
    virtual void adjust(TNode* node, Boolean expand)=0;
    /**
     * Called to draw the outline view. Essentially, draw() calls
     * @ref getGraph() to get the graphical part of the outline, then appends
     * the string returned from @ref getText(). The line containing the
     * focused node in the outline displays in a distinct color. Nodes whose
     * child nodes are not displayed are highlighted.
     */
    virtual void draw();
    /**
     * Called whenever a node receives focus. The `i' parameter indicates the
     * position of the newly focused node in the outline. By default,
     * focused() just sets @ref foc data member to `i'.
     */
    virtual void focused(int i);
    /**
     * getChild() is an abstract method (pure virtual) that descendant outline
     * viewer types must override to return a pointer to the i-th child of the
     * given `node'. If @ref hasChildren() returns False, indicating that
     * `node' has no child nodes, getChild() will not be called for that
     * node. You can safely assume that when an outline viewer calls
     * getChild(), the given node has at least `i' child nodes.
     */
    virtual TNode* getChild(TNode* node, int i)=0;
    /**
     * Returns a string of graphics characters to display to the left of the
     * text returned by @ref getText(). By default, getGraph() calls
     * @ref createGraph() with the default character values. You only need to
     * override getGraph() if you want to change the appearance of the
     * outline. For example, instead of calling createGraph() to show the
     * hierarchy, you might return a string of characters to merely indent
     * the text by a given amount for each level.
     */
    virtual char* getGraph(int level, long lines, ushort flags);
    /**
     * getNumChildren() is an abstract method (pure virtual) that descendant
     * outline viewer types must override to return the number of child nodes
     * in `node'. If @ref hasChildren() returns False for `node',
     * getNumChildren() will never be called.
     */
    virtual int getNumChildren(TNode* node)=0;
    /**
     * Returns a pointer to the i-th node in the outline; that is, the node
     * shown `i' lines from the top of the complete outline.
     */
    virtual TNode* getNode(int i);
    /**
     * Returns a reference to the default outline palette.
     */
    virtual TPalette& getPalette() const;
    /**
     * getRoot() is an abstract method that descendant outline viewer types
     * must override to return a pointer to the root node of the outline.
     */
    virtual TNode* getRoot()=0;
    /**
     * getText() is an abstract method (pure virtual) that descendant outline
     * viewer types must override to return the text of `node'.
     */
    virtual char* getText(TNode* node)=0;
    /**
     * Handles most events for the outline viewer by calling the handleEvent()
     * method inherited from @ref TScroller, then handles certain mouse and
     * keyboard events.
     * @see TScroller::handleEvent
     */
    virtual void handleEvent(TEvent& event);
    /**
     * hasChildren() is an abstract method (pure virtual) that descendant
     * outline viewers must override to return True if the given `node' has
     * child nodes and False if `node' has no child nodes.
     *
     * If hasChildren() returns False for a particular node, the following
     * functions are never called for that node: @ref adjust(),
     * @ref expandAll(), @ref getChild(), @ref getNumChildren() and
     * @ref isExpanded(). Those methods can assume that if they are called,
     * there are child nodes for them to act on.
     */
    virtual Boolean hasChildren(TNode* node)=0;
    /**
     * isExpanded() is an abstract method (pure virtual) that descendant
     * outline viewer types must override to return True if node's child nodes
     * should be displayed. If @ref hasChildren() returns False for `node',
     * isExpanded() will never be called for that node.
     */
    virtual Boolean isExpanded(TNode* node)=0;
    /**
     * Returns True if node `i' is selected. By default, TOutlineViewer
     * assumes a single-selection outline, so it returns True if that node
     * is focused. You can override isSelected() to handle multiple
     * selections.
     */
    virtual Boolean isSelected(int i);
    /**
     * Called whenever a node is selected by the user, either by keyboard
     * control or by the mouse. The `i' parameter indicates the position in
     * the outline of the newly selected node. By default, selected() does
     * nothing; descendant types can override selected() to perform some
     * action in response to selection.
     */
    virtual void selected(int i);
    /**
     * Sets or clears the `aState' state flags for the view by calling the
     * setState() method inherited from @ref TScroller. If the new state
     * includes a focus change, setState() calls @ref drawView() to redraw
     * the outline.
     * @see TScroller::setState
     */
    virtual void setState(ushort aState, Boolean enable);
    /**
     * Updates the limits of the outline viewer. The limit in the vertical
     * direction is number of nodes in the outline. The limit in the
     * horizontal direction is the length of the longest displayed line.
     *
     * Your program should call update() whenever the data in the outline
     * changes. TOutlineViewer assumes that the outline is empty, so if the
     * outline becomes non-empty during initialization, you must explicitly
     * call update().
     *
     * Also, if during the operation of the outline viewer the data being
     * displayed change, you must call update() and the inherited
     * @ref drawView().
     */
    void update();
    /**
     * If `node' has child nodes, expandAll() recursively expands `node' by
     * calling @ref adjust() with the expand parameter True, then expands all
     * its child nodes by calling expandAll() for each of them.
     */
    void expandAll(TNode* node);
    /**
     * firstThat() iterates over the nodes of the outline, calling the
     * function pointed to by `test' until `test' returns True. `test' must
     * point to a function with the following syntax:
     *
     * <pre>
     * Boolean myIter(TOutlineViewer* ov, TNode* cur, int level, int position,
     *     long lines, ushort flags);
     * </pre>
     *
     * The parameters are documented in @ref forEach().
     */
    TNode* firstThat(Boolean (*test)(TOutlineViewer* ov, TNode* node,
        int level, int position, long lines, ushort flags));
    /**
     * Iterates over all the nodes. `action' points to a function that
     * forEach() calls for each node in the outline. The syntax for the
     * iterator function is as follows:
     *
     * <pre>
     * Boolean myIter(TOutlineViewer* ov, TNode* cur, int level, int position,
     *     long lines, ushort flags);
     * </pre>
     *
     * The parameters are as follows:
     *
     * <pre>
     * Name     Description
     *
     * cur      A pointer to the node being checked.
     * 
     * level    The level of the node (how many nodes are above it),
     *          zero-based. This can be used to a call to either
     *          @ref getGraph() or @ref createGraph().
     *
     * position The display order position of the node in the list. This can
     *          be used in a call to @ref focused() or @ref selected(). If in
     *          range, position - delta.Y is location the node is displayed
     *          on the view.
     *
     * lines    Bits indicating the active levels. This can be used in a call
     *          to @ref getGraph() or @ref createGraph(). It dicatates which
     *          horizontal lines need to be drawn.
     *
     * flags    Various flags for drawing. Can be used in a call to
     *          @ref getGraph() or @ref createGraph().
     * </pre>
     * @see TOutlineViewer::firstThat
     */
    TNode* forEach(Boolean (*action)(TOutlineViewer* ov, TNode* cur, int level,
        int position, long lines, ushort flags));
    /**
     * Undocumented.
     */
    char* createGraph(int level, long lines, ushort flags, int levWidth,
        int endWidth, const char* chars);
    /**
     * Indicates the item number of the focused node in the outline.
     */
    int foc;
    /**
     * Undocumented.
     */
    static const char* graphChars;
protected:
    /**
     * Undocumented.
     */
    static void disposeNode(TNode* node);
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
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable *build();
    /**
     * Undocumented.
     */
    static const char * const name;
private:
    void adjustFocus(int newFocus);
    TNode* iterate(Boolean (*action)(TOutlineViewer*, TNode*, int, int, long,
        ushort), Boolean checkResult);
};

inline TOutlineViewer::TOutlineViewer( StreamableInit s) :
    TScroller(s)
{
}

#endif // Uses_TOutlineViewer

#if defined( Uses_TOutline ) && !defined( __TOutline )
#define __TOutline

/* ------------------------------------------------------------------------*/
/*      class TOutline                                                     */
/*                                                                         */
/*      Palette layout                                                     */
/*        1 = Normal color                                                 */
/*        2 = Focus color                                                  */
/*        3 = Select color                                                 */
/*        4 = Not expanded color                                           */
/* ------------------------------------------------------------------------*/

class TRect;
class TScrollBar;
class TEvent;

/**
 * TOutline implements a simple but useful type of outline viewer.
 *
 * It assumes that the outline is a linked list of records of type @ref TNode,
 * so each node consists of a text string (@ref TNode::text), a pointer to
 * any child nodes (@ref TNode::childList), and a pointer to the next node at
 * the same level (@ref TNode::next).
 * @short Implements a simple outline viewer
 */
class TOutline : public TOutlineViewer
{
public:
    /**
     * Constructs an outline view by passing `bounds', `aHScrollBar' and
     * `aVScrollBar' to the constructor inherited from @ref TOutlineViewer.
     *
     * Sets @ref root to `aRoot', then calls @ref update() to set the
     * scrolling limits of the view based on the data in the outline.
     */
    TOutline(const TRect& bounds, TScrollBar* aHScrollBar,
        TScrollBar* aVScrollBar, TNode* aRoot);
    /**
     * Disposes of the outline view by first disposing of the @ref root node,
     * which recursively disposes of all child nodes, then calling the
     * destructor inherited from TView.
     */
    ~TOutline();
    /**
     * Sets the expanded field of `node' to the value passed in `expand'. If
     * `expand' is True, this causes the child nodes linked to `node' to be
     * displayed. If `expand' is False, node's child nodes are hidden.
     */
    virtual void adjust(TNode* node, Boolean expand);
    /**
     * Returns @ref root, which points to the top of the list of nodes for the
     * outline.
     */
    virtual TNode* getRoot();
    /**
     * Returns the number of nodes in node's @ref TNode::childList, or zero if
     * @ref TNode::childList is 0.
     */
    virtual int getNumChildren(TNode* node);
    /**
     * Returns a pointer to the i-th child in node's @ref TNode::childList.
     */
    virtual TNode* getChild(TNode* node, int i);
    /**
     * Returns the string pointed to by node's @ref TNode::text field.
     */
    virtual char* getText(TNode* node);
    /**
     * Returns the value of node's @ref TNode::expanded field.
     */
    virtual Boolean isExpanded(TNode* node);
    /**
     * Returns True if node's @ref TNode::childList is non-0; otherwise
     * returns False.
     */
    virtual Boolean hasChildren(TNode* node);
    /**
     * Points to the root node of the outline tree.
     */
    TNode* root;
protected:
    /**
     * Writes to the output stream `os'.
     */
    virtual void write( opstream& os );
    /**
     * Reads from the input stream `is'.
     */
    virtual void *read( ipstream& is );
    /**
     * Undocumented.
     */
    virtual void writeNode( TNode*, opstream& );
    /**
     * Undocumented.
     */
    virtual TNode* readNode( ipstream& );
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
    TOutline( StreamableInit );
public:
    /**
     * Called to create an object in certain stream-reading situations.
     */
    static TStreamable* build();
    /**
     * Undocumented.
     */
    static const char* const name;
private:
    virtual const char *streamableName() const
        { return name; }
};

inline TOutline::TOutline( StreamableInit s ) : TOutlineViewer( s )
{
}

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TOutline& o )
    { return is >> (TStreamable&)o; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, TOutline*& o )
    { return is >> (void *&)o; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TOutline& o )
    { return os << (TStreamable&)o; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, TOutline* o )
    { return os << (TStreamable*)o; }

#endif // Uses_TOutline
