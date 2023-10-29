/*
 * TOutline.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define cpOutlineViewer "\x6\x7\x3\x8"
#define Uses_TOutlineViewer
#define Uses_TOutline
#define Uses_TEvent
#define Uses_TDrawBuffer
#define Uses_TKeys
#include <tv.h>

#include <string.h>

TOutlineViewer::TOutlineViewer(const TRect & bounds, TScrollBar *aHScrollBar,
			       TScrollBar *aVScrollBar):TScroller(bounds,
								  aHScrollBar,
								  aVScrollBar)
{
	growMode = gfGrowHiX + gfGrowHiY;
	foc = 0;
}

// Called internally to ensure the focus is within range and displayed

void TOutlineViewer::adjustFocus(int newFocus)
{
	if (newFocus < 0)
		newFocus = 0;
	else if (newFocus >= limit.y)
		newFocus = limit.y - 1;
	if (foc != newFocus)
		focused(newFocus);
	if (newFocus < delta.y)
		scrollTo(delta.x, newFocus);
	else if ((newFocus - size.y) >= delta.y)
		scrollTo(delta.x, newFocus - size.y + 1);
}

static TDrawBuffer dBuf;
static int auxPos;

// Called to draw the outline

Boolean drawTree(TOutlineViewer *beingDrawn, TNode *cur, int level,
		 int position, long lines, ushort flags)
{
	ushort color;
	char s[256];
	char *graph;

	if (position >= beingDrawn->delta.y) {
		if (position >= beingDrawn->delta.y + beingDrawn->size.y)
			return True;

		if ((position == beingDrawn->foc)
		    && ((beingDrawn->state & sfFocused) != 0))
			color = beingDrawn->getColor(0x0202);
		else if (beingDrawn->isSelected(position))
			color = beingDrawn->getColor(0x0303);
		else
			color = beingDrawn->getColor(0x0401);
		dBuf.moveChar(0, ' ', color, beingDrawn->size.x);

		graph = beingDrawn->getGraph(level, lines, flags);
		strcpy(s, graph);
		delete graph;

		if ((flags & ovExpanded) == 0) {
			strcat(s, "~");
			strcat(s, beingDrawn->getText(cur));
			strcat(s, "~");
		} else
			strcat(s, beingDrawn->getText(cur));
		if (beingDrawn->delta.x <= (int)strlen(s))
			dBuf.moveCStr(0, &s[beingDrawn->delta.x], color);
		else
			dBuf.moveCStr(0, "", color);
		beingDrawn->writeLine(0, position - beingDrawn->delta.y,
				      beingDrawn->size.x, 1, dBuf);
		auxPos = position;
	}

	return False;
}

void TOutlineViewer::draw()
{
	ushort nrmColor = getColor(0x0401);

	firstThat(drawTree);
	dBuf.moveChar(0, ' ', nrmColor, size.x);
	writeLine(0, auxPos + 1, size.x, size.y - (auxPos - delta.y), dBuf);
}

// ExpandAll expands the current node and all child nodes

void TOutlineViewer::expandAll(TNode *node)
{
	int i, n;

	if (hasChildren(node)) {
		adjust(node, True);
		n = getNumChildren(node) - 1;
		for (i = 0; i <= n; i++)
			expandAll(getChild(node, i));
	}
}

/*Draws a graph string suitable for returning from getGraph.  Level
  indicates the outline level.    Lines is the set of bits decribing
  the which levels have a "continuation" mark (usually a vertical
  lines).  If bit 3 is set, level 3 is continued beyond this level.
  Flags gives extra information about how to draw the end of the
  graph (see the ovXXX constants).    LevWidth is how many characters
  to indent for each level.     endWidth is the length the end characters.

  The graphics is divided into two parts: the level marks, and the end
  or node graphic.    The level marks consist of the Level Mark character
  separated by Level Filler.  What marks are present is determined by
  Lines.  The end graphic is constructed by placing on of the End First
  charcters followed by endWidth-4 End Filler characters, followed by the
  End Child character, followed by the Retract/Expand character.  If
  endWidth equals 2, End First and Retract/Expand are used.     If endWidth
  equals 1, only the Retract/Expand character is used.    Which characters
  are selected is determined by Flags.

  The layout for the characters in the Chars is:

   1: Level Filler
     Typically a space.     Used between level markers.
   2: Level Mark
     Typically a vertical bar.    Used to mark the levels currenly active.
   3: End First (not last child)
     Typically a sideways T.  Used as the first character of the end part
     of a node graphic if the node is not the last child of the parent.
   4: End First (last child)
     Typically a L shape.  Used as the first character of the end part
     of a node graphic if the node is the last child of the parent.
   5: End Filler
     Typically a horizontal line.  Used as filler for the end part of a
     node graphic.
   6: End Child position
     Typically not used.  If endWidth > LevWidth this character will
     be placed on top of the markers for next level.  If used it is
     typically a T.
   7: Retracted character
     Typically a '+'.  Displayed as the last character of the end
     node if the level has children and they are not expanded.
   8: Expanded character
     Typically as straight line. Displayed as the last character of
     the end node if the level has children and they are expanded.

*/

char *TOutlineViewer::createGraph(int level, long lines, ushort flags,
				  int levWidth, int endWidth, const char *chars)
{
	static const int
	    FillerOrBar = 0, YorL = 2, StraightOrTee = 4, retracted = 6;

	char *graph = new char[level * levWidth + endWidth + 1];
	char *p;

	Boolean expanded = Boolean((flags & ovExpanded) != 0);
	Boolean children = Boolean((flags & ovChildren) != 0);
	Boolean last = Boolean((flags & ovLast) != 0);

	for (p = graph; level > 0; level--, lines >>= 1) {
		*p++ =
		    (lines & 1) ? chars[FillerOrBar + 1] : chars[FillerOrBar];
		memset(p, chars[FillerOrBar], levWidth - 1);
		p += levWidth - 1;
	}

	if (--endWidth > 0) {
		*p++ = last ? chars[YorL + 1] : chars[YorL];
		if (--endWidth > 0) {
			if (--endWidth > 0) {
				memset(p, chars[StraightOrTee], endWidth);
				p += endWidth;
			}
			*p++ =
			    children ? chars[StraightOrTee +
					     1] : chars[StraightOrTee];
		}
		*p++ = expanded ? chars[retracted + 1] : chars[retracted];
	}
	*p = 0;

	return graph;
}

/*
  FirstThat iterates over the nodes of the outline until the given
  local function returns true. The declaration for the local function
  must look like (save for the names, of course):

    function MyIter(Cur: Pointer; Level, Position: Integer;
      Lines: LongInt; Flags: Word); far;

  The parameters are as follows:

    Cur:       A pointer to the node being checked.
    Level:     The level of the node (how many node above it it has)
               Level is 0 based.  This can be used to a call to
               either getGraph or createGraph.
    Position:  The display order position of the node in the list.
               This can be used in a call to Focused or Selected.
               If in range, Position - Delta.Y is location the node
               is displayed on the view.
    Lines:     Bits indicating the active levels.    This can be used in a
               call to getGraph or createGraph. It dicatates which
               horizontal lines need to be drawn.
    Flags:     Various flags for drawing (see ovXXXX flags).  Can be used
               in a call to getGraph or createGraph.
*/
TNode *TOutlineViewer::
firstThat(Boolean(*test) (TOutlineViewer *, TNode *, int, int, long, ushort))
{
	return iterate(test, True);
}

// Called whenever Node is receives focus

void TOutlineViewer::focused(int i)
{
	foc = i;
}

/*
  Internal function used by both FirstThat and ForEach to do the
  actual iteration over the data. See FirstThat for more details }
*/

TNode *traverseTree(TOutlineViewer *outLine,
		    Boolean(*action) (TOutlineViewer *, TNode *, int, int, long,
				      ushort), int &position,
		    Boolean & checkResult, TNode *cur, int level, long lines,
		    Boolean lastChild)
{

	Boolean result;
	int j, childCount;
	TNode *ret;
	ushort flags;
	Boolean children;

	if (cur == nullptr)
		return nullptr;

	children = outLine->hasChildren(cur);

	flags = 0;
	if (lastChild)
		flags |= ovLast;

	if (children && outLine->isExpanded(cur))
		flags |= ovChildren;

	if (!children || outLine->isExpanded(cur))
		flags |= ovExpanded;

	position++;

	result = (*action) (outLine, cur, level, position, lines, flags);
	if (checkResult && result)
		return cur;

	if (children && outLine->isExpanded(cur)) {
		childCount = outLine->getNumChildren(cur);

		if (!lastChild)
			lines |= 1 << level;

		for (j = 0; j < childCount; j++) {
			ret =
			    traverseTree(outLine, action, position, checkResult,
					 outLine->getChild(cur, j), level + 1,
					 lines, Boolean(j == (childCount - 1)));
			if (ret)
				return ret;
		}
	}
	return nullptr;
}

TNode *TOutlineViewer::
iterate(Boolean(*action) (TOutlineViewer *, TNode *, int, int, long, ushort),
	Boolean checkResult)
{
	int position = -1;
	return traverseTree(this, action, position, checkResult,
			    getRoot(), 0, 0, True);
}

// Iterates over all the nodes.     See FirstThat for a more details

TNode *TOutlineViewer::
forEach(Boolean(*action) (TOutlineViewer *, TNode *, int, int, long, ushort))
{
	return iterate(action, False);
}

// Returns the outline palette

TPalette & TOutlineViewer::getPalette()const
{
	static TPalette p(cpOutlineViewer, sizeof(cpOutlineViewer));
	return p;
}

/*
  Called to retrieve the characters to display prior to the
  text returned by GetText.     Can be overridden to return
  change the appearance of the outline. My default calls
  createGraph with the default.
*/

char *TOutlineViewer::getGraph(int level, long lines, ushort flags)
{
	static const int levelWidth = 3, endWidth = levelWidth;

	return createGraph(level, lines, flags, levelWidth, endWidth,
			   graphChars);
}

static Boolean isNode(TOutlineViewer *, TNode *, int, int position, long,
		      ushort)
{
	return Boolean(auxPos == position);
}

// Returns a pointer to the node that is to be shown on line i

TNode *TOutlineViewer::getNode(int i)
{
	auxPos = i;

	return firstThat(isNode);
}

/*
  Returns if Node is selected.    By default, returns true if Node is
  Focused (i.e. single selection).    Can be overriden to handle
  multiple selections. }
*/
Boolean TOutlineViewer::isSelected(int i)
{
	return (foc == i) ? True : False;
}

static long focLines;
static ushort focFlags;
static int focLevel;

static Boolean isFocused(TOutlineViewer *focusCheck, TNode *, int level,
			 int position, long lines, ushort flags)
{
	if (position == focusCheck->foc) {
		focLevel = level;
		focLines = lines;
		focFlags = flags;

		return True;
	} else
		return False;
}

// Called to handle an event

void TOutlineViewer::handleEvent(TEvent & event)
{
	const int mouseAutoToSkip = 3;

	TPoint mouse;
	TNode *cur;
	int newFocus = 0;
	int count;
	char *graph;
	uchar dragged;

	TScroller::handleEvent(event);
	switch (event.what) {
	case evMouseDown:
		count = 0;
		dragged = 0;
		do {
			if (dragged < 2)
				dragged++;
			mouse = makeLocal(event.mouse.where);
			if (mouseInView(event.mouse.where))
				newFocus = delta.y + mouse.y;
			else {
				if (event.what == evMouseAuto)
					count++;
				if (count == mouseAutoToSkip) {
					count = 0;
					if (mouse.y < 0)
						newFocus--;
					if (mouse.y >= size.y)
						newFocus++;
				}
			}
			if (foc != newFocus) {
				adjustFocus(newFocus);
				drawView();
			}
#ifndef __UNPATCHED
		} while (!(event.mouse.eventFlags & meDoubleClick) &&
			 mouseEvent(event, evMouseMove + evMouseAuto));
#else
		} while (mouseEvent(event, evMouseMove + evMouseAuto));
#endif

		if (event.mouse.eventFlags & meDoubleClick)
			selected(foc);
		else {
			if (dragged < 2) {
				cur = firstThat(isFocused);
				graph = getGraph(focLevel, focLines, focFlags);
				if (mouse.x < (int)strlen(graph)) {
					adjust(cur,
					       !isExpanded(cur) ? True : False);
					update();
					drawView();
				}
				delete graph;
			}
		}

		break;

	case evKeyboard:

		newFocus = foc;
		switch (ctrlToArrow(event.keyDown.keyCode)) {
		case kbUp:
		case kbLeft:
			newFocus--;
			break;
		case kbDown:
		case kbRight:
			newFocus++;
			break;
		case kbPgDn:
			newFocus += size.y - 1;
			break;
		case kbPgUp:
			newFocus -= size.y - 1;
			break;
		case kbHome:
			newFocus = delta.y;
			break;
#ifndef __UNPATCHED
		case kbEnd:
#else
		      casekbEnd:
#endif
			newFocus = delta.y + size.y - 1;
			break;
		case kbCtrlPgUp:
			newFocus = 0;
			break;
		case kbCtrlPgDn:
			newFocus = limit.y - 1;
			break;
		case kbCtrlEnter:
		case kbEnter:
			selected(newFocus);
			break;
		default:
			uchar code = event.keyDown.charScan.charCode;
			switch (code) {
			case '-':
			case '+':
				adjust(getNode(newFocus),
				       code == '+' ? True : False);
				break;
			case '*':
				expandAll(getNode(newFocus));
				break;
			default:
				return;
			}
			update();
		}
		clearEvent(event);
		adjustFocus(newFocus);
		drawView();
		break;
	default:
		;
	}
}

/*
  Called whenever Node is selected by the user either via keyboard
  control or by the mouse. }
*/
void TOutlineViewer::selected(int)
{
}

// Redraws the outline if the outliner sfFocus state changes

void TOutlineViewer::setState(ushort aState, Boolean enable)
{
	TScroller::setState(aState, enable);
	if ((aState & sfFocused) != 0)
		drawView();
}

static int updateCount;
static int updateMaxX;

static Boolean countNode(TOutlineViewer *beingCounted, TNode *p, int level,
			 int, long lines, ushort flags)
{
	int len;
	char *graph;

	updateCount++;
	graph = beingCounted->getGraph(level, lines, flags);
	len = strlen(beingCounted->getText(p)) + strlen(graph);
	if (updateMaxX < len)
		updateMaxX = len;
	delete graph;
	return False;
}

/*
  Updates the limits of the outline viewer.     Should be called whenever
  the data of the outline viewer changes.  This includes during
  the initalization of base classes.  TOutlineViewer assumes that
  the outline is empty.     If the outline becomes non-empty during the
  initialization, Update must be called. Also, if during the operation
  of the TOutlineViewer the data being displayed changes, Update
  and DrawView must be called. }
*/
void TOutlineViewer::update()
{
	updateCount = 0;
	updateMaxX = 0;
	firstThat(countNode);
	setLimit(updateMaxX, updateCount);
	adjustFocus(foc);
}

void TOutlineViewer::disposeNode(TNode *node)
{
	if (node) {
		if (node->childList)
			disposeNode(node->childList);
		if (node->next)
			disposeNode(node->next);
		delete node;
	}
}

#if !defined(NO_STREAMABLE)

void *TOutlineViewer::read(ipstream & ip)
{
	TScroller::read(ip);
	ip >> foc;
	return this;
}

void TOutlineViewer::write(opstream & op)
{
	TScroller::write(op);
	op << foc;
}

#endif

// TOutline

TOutline::TOutline(const TRect & bounds, TScrollBar *aHScrollBar,
		   TScrollBar *aVScrollBar, TNode *aRoot):TOutlineViewer(bounds,
									 aHScrollBar,
									 aVScrollBar)
{
	root = aRoot;
	update();
}

TOutline::~TOutline()
{
	disposeNode(root);
}

void TOutline::adjust(TNode *node, Boolean expand)
{
	node->expanded = expand;
}

TNode *TOutline::getRoot()
{
	return root;
}

int TOutline::getNumChildren(TNode *node)
{
	int i;
	TNode *p;

	p = node->childList;
	i = 0;
	while (p) {
		i++;
		p = p->next;
	}
	return i;
}

TNode *TOutline::getChild(TNode *node, int i)
{
	TNode *p;

	p = node->childList;
	while ((i != 0) && (p != nullptr)) {
		i--;
		p = p->next;
	}
	return p;
}

char *TOutline::getText(TNode *node)
{
	return node->text;
}

Boolean TOutline::isExpanded(TNode *node)
{
	return node->expanded;
}

Boolean TOutline::hasChildren(TNode *node)
{
	return Boolean(node->childList != nullptr);
}

#if !defined(NO_STREAMABLE)

TNode *TOutline::readNode(ipstream & ip)
{
	int nChildren;
	uchar more;
	uchar expand;

	TNode *node = new TNode(nullptr);

	ip >> more;
	ip >> expand;
	ip >> nChildren;
	node->text = ip.readString();
	node->expanded = Boolean(expand);

	if (nChildren)
		node->childList = readNode(ip);
	else
		node->childList = nullptr;

	if (more)
		node->next = readNode(ip);
	else
		node->next = nullptr;

	return node;
}

void TOutline::writeNode(TNode *node, opstream & op)
{
	uchar more = (node->next != nullptr) ? 1 : 0;
	uchar expand = (node->expanded) ? 1 : 0;

	op << more;
	op << expand;
	op << getNumChildren(node);
	op.writeString(node->text);

	if (node->childList != nullptr)
		writeNode(node->childList, op);

	if (node->next != nullptr)
		writeNode(node->next, op);
}

void *TOutline::read(ipstream & ip)
{
	TOutlineViewer::read(ip);

	root = readNode(ip);

	return this;
}

void TOutline::write(opstream & op)
{
	TOutlineViewer::write(op);

	writeNode(root, op);
}

TStreamable *TOutline::build()
{
	return new TOutline(streamableInit);
}

#endif
