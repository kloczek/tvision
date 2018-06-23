/*
 * TEditor.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 * Modified again by Dmitrij Korovkin <tkf@glasnet.ru>
 */

#define Uses_TKeys
#define Uses_TEditor
#define Uses_TIndicator
#define Uses_TEvent
#define Uses_TScrollBar
#define Uses_TFindDialogRec
#define Uses_TReplaceDialogRec
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <ctype.h>
#include <string.h>

/*
 * SS: declarations fixed.  See `asm.cc'.
 */
int countLines( void *buf, size_t count );
size_t scan( const void *block, size_t size, const char *str );
size_t iScan( const void *block, size_t size, const char *str );

inline int isWordChar( int ch )
{
#ifndef __UNPATCHED
    return isalnum((uchar)ch) || ch == '_';
//                  ^^^^^- correction for extended ASCII.
#else
    return isalnum(ch) || ch == '_';
#endif
}

const ushort firstKeys[] =
{
    37,
    kbCtrlA, cmWordLeft,
    kbCtrlC, cmPageDown,
    kbCtrlD, cmCharRight,
    kbCtrlE, cmLineUp,
    kbCtrlF, cmWordRight,
    kbCtrlG, cmDelChar,
    kbCtrlH, cmBackSpace,
    kbCtrlK, 0xFF02,
    kbCtrlL, cmSearchAgain,
    kbCtrlM, cmNewLine,
    kbCtrlO, cmIndentMode,
    kbCtrlQ, 0xFF01,
    kbCtrlR, cmPageUp,
    kbCtrlS, cmCharLeft,
    kbCtrlT, cmDelWord,
    kbCtrlU, cmUndo,
    kbCtrlV, cmInsMode,
    kbCtrlX, cmLineDown,
    kbCtrlY, cmDelLine,
    kbLeft, cmCharLeft,
    kbRight, cmCharRight,
    kbCtrlLeft, cmWordLeft,
    kbCtrlRight, cmWordRight,
    kbHome, cmLineStart,
    kbEnd, cmLineEnd,
    kbUp, cmLineUp,
    kbDown, cmLineDown,
    kbPgUp, cmPageUp,
    kbPgDn, cmPageDown,
    kbCtrlPgUp, cmTextStart,
    kbCtrlPgDn, cmTextEnd,
    kbIns, cmInsMode,
    kbDel, cmDelChar,
    kbShiftIns, cmPaste,
    kbShiftDel, cmCut,
    kbCtrlIns, cmCopy,
    kbCtrlDel, cmClear
};

const ushort quickKeys[] =
{   8,
    'A', cmReplace,
    'C', cmTextEnd,
    'D', cmLineEnd,
    'F', cmFind,
    'H', cmDelStart,
    'R', cmTextStart,
    'S', cmLineStart,
    'Y', cmDelEnd
};

const ushort blockKeys[] =
{   5,
    'B', cmStartSelect,
    'C', cmPaste,
    'H', cmHideSelect,
    'K', cmCopy,
    'Y', cmCut
};

const ushort *keyMap[] = { firstKeys, quickKeys, blockKeys };

ushort defEditorDialog( int, ... );

ushort scanKeyMap( const void *keyMap, ushort keyCode )
{
    ushort *kM = (ushort *)keyMap;
    uchar codeLow = keyCode & 0xff;
    uchar codeHi  = keyCode >> 8;

    int n;

    for (n = *kM++; n--; kM++)
    {
        uchar  mapLow  = *kM & 0xff;
        uchar  mapHi   = *kM >> 8;
        kM++;
        ushort command = *kM;

        if ((mapLow == codeLow) && ((mapHi == 0) || (mapHi == codeHi)))
            return command;
    };
    return 0;
}

#define cpEditor    "\x06\x07"

TEditor::TEditor( const TRect& bounds,
                  TScrollBar *aHScrollBar,
                  TScrollBar *aVScrollBar,
                  TIndicator *aIndicator,
		  uint aBufSize ) :
    TView( bounds ),
    hScrollBar( aHScrollBar ),
    vScrollBar( aVScrollBar ),
    indicator( aIndicator ),
    bufSize( aBufSize ),
    canUndo( True ),
    selecting( False ),
    overwrite( False ),
    autoIndent( False ) ,
    lockCount( 0 ),
    keyState( 0 )
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofSelectable;
    eventMask = evMouseDown | evKeyDown | evCommand | evBroadcast;
    showCursor();
    initBuffer();
    if( buffer != 0 )
        isValid = True;
    else
    {
        editorDialog( edOutOfMemory );
        bufSize = 0;
        isValid = False;
    }
    setBufLen(0);
}

TEditor::~TEditor()
{
}

void TEditor::shutDown()
{
    doneBuffer();
    TView::shutDown();
}

void TEditor::changeBounds( const TRect& bounds )
{
    setBounds(bounds);
    delta.x = max(0, min(delta.x, limit.x - size.x));
    delta.y = max(0, min(delta.y, limit.y - size.y));
    update(ufView);
}

uint TEditor::charPos( uint p, uint target )
{
    uint pos = 0;
    while( p < target )
    {
        if( bufChar(p) == '\x9' )
            pos |= 7;
        pos++;
        p++;
    }
    return pos;
}

uint TEditor::charPtr( uint p, uint target )
{
    uint pos = 0;
  /* SS: changed */
  while( (pos < target) && (p < bufLen) && (bufChar(p) != '\n') )
    {
    if( bufChar(p) == '\x09' )
        pos |= 7;
    pos++;
    p++;
    }
  if( pos > target )
    p--;
  return p;
}

Boolean TEditor::clipCopy()
{
    Boolean res = False;
    if( (clipboard != 0) && (clipboard != this) )
        {
        res = clipboard->insertFrom(this);
        selecting = False;
        update(ufUpdate);
        }
    return res;
}

void TEditor::clipCut()
{
    if( clipCopy() == True )
        deleteSelect();
}

void TEditor::clipPaste()
{
    if( (clipboard != 0) && (clipboard != this) )
        insertFrom(clipboard);
}

void TEditor::convertEvent( TEvent& event )
{
    if( event.what == evKeyDown )
        {
        if( (event.keyDown.controlKeyState & kbShift) != 0 &&
            event.keyDown.charScan.scanCode >= 0x47 &&
            event.keyDown.charScan.scanCode <= 0x51
          )
            event.keyDown.charScan.charCode = 0;

        ushort key = event.keyDown.keyCode;
        if( keyState != 0 )
            {
            if( (key & 0xFF) >= 0x01 && (key & 0xFF) <= 0x1A )
                key += 0x40;
            if( (key & 0xFF) >= 0x61 && (key & 0xFF) <= 0x7A )
                key -= 0x20;
            }
        key = scanKeyMap(keyMap[keyState], key);
        keyState = 0;
        if( key != 0 )
            {
            if( (key & 0xFF00) == 0xFF00 )
                {
                keyState = (key & 0xFF);
                clearEvent(event);
                }
            else
                {
                event.what = evCommand;
                event.message.command = key;
                }
            }
        }
}

Boolean TEditor::cursorVisible()
{
  return Boolean((curPos.y >= delta.y) && (curPos.y < delta.y + size.y));
}

void TEditor::deleteRange( uint startPtr,
                           uint endPtr,
                           Boolean delSelect
                         )
{
    if( hasSelection() == True && delSelect == True )
        deleteSelect();
    else
        {
        setSelect(curPtr, endPtr, True);
        deleteSelect();
        setSelect(startPtr, curPtr, False);
        deleteSelect();
        }
}

void TEditor::deleteSelect()
{
    insertText( 0, 0, False );
}

void TEditor::doneBuffer()
{
    delete buffer;
}

void TEditor::doSearchReplace()
{
    int i;
    do  {
        i = cmCancel;
        if( search(findStr, editorFlags) == False )
            {
            if( (editorFlags & (efReplaceAll | efDoReplace)) !=
                (efReplaceAll | efDoReplace) )
                    editorDialog( edSearchFailed );
            }
        else
            if( (editorFlags & efDoReplace) != 0 )
                {
                i = cmYes;
                if( (editorFlags & efPromptOnReplace) != 0 )
                    {
                    TPoint c = makeGlobal( cursor );
                    i = editorDialog( edReplacePrompt, &c );
                    }
                if( i == cmYes )
                    {
                    lock();
                    insertText( replaceStr, strlen(replaceStr), False);
                    trackCursor(False);
                    unlock();
                    }
                }
        } while( i != cmCancel && (editorFlags & efReplaceAll) != 0 );
}

void TEditor::doUpdate()
{
    if( updateFlags != 0 )
        {
        setCursor(curPos.x - delta.x, curPos.y - delta.y);
        if( (updateFlags & ufView) != 0 )
            drawView();
        else
            if( (updateFlags & ufLine) != 0 )
                drawLines( curPos.y-delta.y, 1, lineStart(curPtr) );
        if( hScrollBar != 0 )
            hScrollBar->setParams(delta.x, 0, limit.x - size.x, size.x / 2, 1);
        if( vScrollBar != 0 )
            vScrollBar->setParams(delta.y, 0, limit.y - size.y, size.y - 1, 1);
        if( indicator != 0 )
            indicator->setValue(curPos, modified);
        if( (state & sfActive) != 0 )
            updateCommands();
        updateFlags = 0;
        }
}

void TEditor::draw()
{
    if( drawLine != delta.y )
        {
        drawPtr = lineMove( drawPtr, delta.y - drawLine );
        drawLine = delta.y;
        }
    drawLines( 0, size.y, drawPtr );
}

/*Added again as a such a plug for backward compatibility with an old
application*/
#ifdef __RUSSIAN_LETTERS
static unsigned KoiTable[128] =
{
        192, 193, 194, 195, 196, 197, 198, 199, /* 0x80 - 0x87 */
        200, 201, 202, 203, 204, 205 ,206, 207, /* 0x88 - 0x8f */
        208, 209, 210, 211, 212, 213, 214, 215, /* 0x90 - 0x97 */
        216, 217, 218, 219, 220, 221, 222, 223, /* 0x98 - 0x9f */
        240, 241, 242, 243, 244, 245, 246, 247, /* 0xa0 - 0xa7 */
        248, 249, 250, 251, 252, 253, 254, 255, /* 0xa8 - 0xaf */
        176, 177, 178, 179, 180, 181, 182, 183, /* 0xb0 - 0xb7 */
        184, 185, 186, 187, 188, 189, 190, 191, /* 0xb8 - 0xbf */
        238, 160, 161, 230, 164, 165, 228, 163, /* 0xc0 - 0xc7 */
        229, 168, 169, 170, 171, 172, 173, 174, /* 0xc8 - 0xcf */
        175, 239, 224, 225, 226, 227, 166, 162, /* 0xd0 - 0xd7 */
        236, 235, 167, 232, 237, 233, 231, 234, /* 0xd8 - 0xdf */
        158, 128, 129, 150, 132, 133, 148, 131, /* 0xe0 - 0xe7 */
        149, 136, 137, 138, 139, 140, 141, 142, /* 0xe8 - 0xef */
        143, 159, 144, 145, 146, 147, 134, 130, /* 0xf0 - 0xf7 */
        156, 155, 135, 152, 157, 153, 151, 154  /* 0xf8 - 0xff */
};
#endif

void TEditor::drawLines( int y, uint count, uint linePtr )
{
    ushort color = getColor(0x0201);
    while( count-- > 0 )
        {
        ushort b[maxLineLength * 4];
        formatLine( b, linePtr, delta.x+size.x, color );

/*Added again as a such a plug for backward compatibility with an old
application*/
#ifdef __RUSSIAN_LETTERS
       ushort *p = &b[delta.x];
       for (int i = delta.x; i < size.x; i++, p++ )
       {
               int code = *p & 0xff;   /* character is in the low byte */

               if (code > 0x7f) *p = (*p & 0xff00) | KoiTable[code - 0x80];
       }
#endif
        writeBuf(0, y, size.x, 1, &b[delta.x]);
        linePtr = nextLine(linePtr);
        y++;
        }
}

void TEditor::find()
{
    TFindDialogRec findRec( findStr, editorFlags );
    if( editorDialog( edFind, &findRec ) != cmCancel )
        {
        strcpy( findStr, findRec.find );
        editorFlags = findRec.options & ~efDoReplace;
        doSearchReplace();
        }
}

uint TEditor::getMousePtr( TPoint m )
{
    TPoint mouse = makeLocal( m );
    mouse.x = max(0, min(mouse.x, size.x - 1));
    mouse.y = max(0, min(mouse.y, size.y - 1));
    return charPtr(lineMove(drawPtr, mouse.y + delta.y - drawLine),
        mouse.x + delta.x);
}

TPalette& TEditor::getPalette() const
{
    static TPalette palette( cpEditor, sizeof( cpEditor )-1 );
    return palette;
}

void TEditor::checkScrollBar( const TEvent& event,
                              TScrollBar *p,
                              int& d
                            )
{
    if( (event.message.infoPtr == p) && (p->value != d) )
        {
        d = p->value;
        update( ufView );
        }
}

void TEditor::handleEvent( TEvent& event )
{
    TView::handleEvent( event );

    Boolean centerCursor = Boolean(!cursorVisible());
    uchar selectMode = 0;

    if( selecting == True ||
        (event.what & evMouse && (event.mouse.controlKeyState & kbShift) != 0) ||
        (event.what & evKeyboard && (event.keyDown.controlKeyState & kbShift ) != 0)
      )
        selectMode = smExtend;

    convertEvent( event );

    switch( event.what )
        {

        case evMouseDown:
            if( event.mouse.eventFlags & meDoubleClick )
                selectMode |= smDouble;

            do  {
                lock();
                if( event.what == evMouseAuto )
                    {
                    TPoint mouse = makeLocal( event.mouse.where );
                    TPoint d = delta;
                    if( mouse.x < 0 )
                        d.x--;
                    if( mouse.x >= size.x )
                        d.x++;
                    if( mouse.y < 0 )
                        d.y--;
                    if( mouse.y >= size.y )
                        d.y++;
                    scrollTo(d.x, d.y);
                    }
                setCurPtr(getMousePtr(event.mouse.where), selectMode);
                selectMode |= smExtend;
                unlock();
                } while( mouseEvent(event, evMouseMove + evMouseAuto) );
            break;

        case evKeyDown:
            if( event.keyDown.charScan.charCode == 9 ||
                ( event.keyDown.charScan.charCode >= 32 && event.keyDown.charScan.charCode < 255 ) )
                    {
                    lock();
                    if( overwrite == True && hasSelection() == False )
                        if( curPtr != lineEnd(curPtr) )
                            selEnd = nextChar(curPtr);
                    insertText( &event.keyDown.charScan.charCode, 1, False);
                    trackCursor(centerCursor);
                    unlock();
                    }
            else
                return;
            break;

        case evCommand:
            switch( event.message.command )
                {
                case cmFind:
                    find();
                    break;
                case cmReplace:
                    replace();
                    break;
                case cmSearchAgain:
                    doSearchReplace();
                    break;
                default:
                    lock();
                    switch( event.message.command )
                        {
                        case cmCut:
                            clipCut();
                            break;
                        case cmCopy:
                            clipCopy();
                            break;
                        case cmPaste:
                            clipPaste();
                            break;
                        case cmUndo:
                            undo();
                            break;
                        case cmClear:
                            deleteSelect();
                            break;
                        case cmCharLeft:
                            setCurPtr(prevChar(curPtr), selectMode);
                            break;
                        case cmCharRight:
                            setCurPtr(nextChar(curPtr), selectMode);
                            break;
                        case cmWordLeft:
                            setCurPtr(prevWord(curPtr), selectMode);
                            break;
                        case cmWordRight:
                            setCurPtr(nextWord(curPtr), selectMode);
                            break;
                        case cmLineStart:
                            setCurPtr(lineStart(curPtr), selectMode);
                            break;
                        case cmLineEnd:
                            setCurPtr(lineEnd(curPtr), selectMode);
                            break;
                        case cmLineUp:
                            setCurPtr(lineMove(curPtr, -1), selectMode);
                            break;
                        case cmLineDown:
                            setCurPtr(lineMove(curPtr, 1), selectMode);
                            break;
                        case cmPageUp:
                            setCurPtr(lineMove(curPtr, -(size.y-1)), selectMode);
                            break;
                        case cmPageDown:
                            setCurPtr(lineMove(curPtr, size.y-1), selectMode);
                            break;
                        case cmTextStart:
                            setCurPtr(0, selectMode);
                            break;
                        case cmTextEnd:
                            setCurPtr(bufLen, selectMode);
                            break;
                        case cmNewLine:
                            newLine();
                            break;
                        case cmBackSpace:
                            deleteRange(prevChar(curPtr), curPtr, True);
                            break;
                        case cmDelChar:
                            deleteRange(curPtr, nextChar(curPtr), True);
                            break;
                        case cmDelWord:
                            deleteRange(curPtr, nextWord(curPtr), False);
                            break;
                        case cmDelStart:
                            deleteRange(lineStart(curPtr), curPtr, False);
                            break;
                        case cmDelEnd:
                            deleteRange(curPtr, lineEnd(curPtr), False);
                            break;
                        case cmDelLine:
                            deleteRange(lineStart(curPtr), nextLine(curPtr), False);
                            break;
                        case cmInsMode:
                            toggleInsMode();
                            break;
                        case cmStartSelect:
                            startSelect();
                            break;
                        case cmHideSelect:
                            hideSelect();
                            break;
                        case cmIndentMode:
                            autoIndent = Boolean(!autoIndent);
                            break;
                        default:
                            unlock();
                            return;
                        }
                    trackCursor(centerCursor);
                    unlock();
                    break;
                }
#ifndef __UNPATCHED
                break;
#endif
        case evBroadcast:
            switch( event.message.command )
                {
                case cmScrollBarChanged:
            if ((event.message.infoPtr == hScrollBar) ||
                (event.message.infoPtr == vScrollBar))
            {
                        checkScrollBar( event, hScrollBar, delta.x );
                        checkScrollBar( event, vScrollBar, delta.y );
            }
            else
                return;
                    break;
                default:
                    return;
                }
        }
    clearEvent(event);
}

Boolean TEditor::hasSelection()
{
    return Boolean(selStart != selEnd);
}

void TEditor::hideSelect()
{
    selecting = False;
    setSelect(curPtr, curPtr, False);
}

void TEditor::initBuffer()
{
    buffer = new char[bufSize];
}

Boolean TEditor::insertBuffer( char *p,
                               uint offset,
                               uint length,
                               Boolean allowUndo,
                               Boolean selectText
                             )
{
    selecting = False;
    uint selLen = selEnd - selStart;
    if( selLen == 0 && length == 0 )
        return True;

    uint delLen = 0;
    if( allowUndo == True )
        {
        if( curPtr == selStart )
            delLen = selLen;
        else
            if( selLen > insCount )
                delLen = selLen - insCount;
        }

    uint newSize = uint(bufLen + delCount - selLen + delLen) + length;

    if( newSize > bufLen + delCount && setBufSize(newSize) == False )
            {
            editorDialog( edOutOfMemory );
	    selEnd = selStart;
            return False;
            }

    uint selLines = countLines( &buffer[bufPtr(selStart)], selLen );
    if( curPtr == selEnd )
        {
        if( allowUndo == True )
            {
            if( delLen > 0 )
                memmove(
                         &buffer[curPtr + gapLen - delCount - delLen],
                         &buffer[selStart],
                         delLen
                       );
            insCount -= selLen - delLen;
            }
        curPtr = selStart;
        curPos.y -= selLines;
        }
    if( delta.y > curPos.y )
        {
        delta.y -= selLines;
        if( delta.y < curPos.y )
            delta.y = curPos.y;
        }

    if( length > 0 )
        memmove(
                &buffer[curPtr],
                &p[offset],
                length
               );

    uint lines = countLines( &buffer[curPtr], length );
    curPtr += length;
    curPos.y += lines;
    drawLine = curPos.y;
    drawPtr = lineStart(curPtr);
    curPos.x = charPos(drawPtr, curPtr);
    if( selectText == False )
        selStart = curPtr;
    selEnd = curPtr;
    bufLen += length - selLen;
    gapLen -= length - selLen;
    if( allowUndo == True )
        {
        delCount += delLen;
        insCount += length;
        }
    limit.y += lines - selLines;
    delta.y = max(0, min(delta.y, limit.y - size.y));
    if( isClipboard() == False )
        modified = True;
    setBufSize(bufLen + delCount);
    if( selLines == 0 && lines == 0 )
        update(ufLine);
    else
        update(ufView);
    return True;
}

Boolean TEditor::insertFrom( TEditor *editor )
{
    uint pt = editor->bufPtr(editor->selStart);
    return insertBuffer( editor->buffer,
			 pt,
                         editor->selEnd - editor->selStart,
                         canUndo,
                         isClipboard()
                        );
}

Boolean TEditor::insertText( const void *text, uint length,
	Boolean selectText )
{
  return insertBuffer( (char *)text, 0, length, canUndo, selectText);
}

Boolean TEditor::isClipboard()
{
    return Boolean(clipboard == this);
}

uint TEditor::lineMove( uint p, int count )
{
    uint i = p;
    p = lineStart(p);
    uint pos = charPos(p, i);
    while( count != 0 )
        {
        i = p;
        if( count < 0 )
            {
            p = prevLine(p);
            count++;
            }
        else
            {
            p = nextLine(p);
            count--;
            }
        }
    if( p != i )
        p = charPtr(p, pos);
    return p;
}

void TEditor::lock()
{
    lockCount++;
}

void TEditor::newLine()
{
    /* SS: changed */
    const char nl[] = "\n";
    uint p = lineStart(curPtr);
    uint i = p;
    while( i < curPtr &&
           ( (buffer[i] == ' ') || (buffer[i] == '\x9'))
         )
         i++;
    insertText(nl, 1, False);
    if( autoIndent == True )
        insertText( &buffer[p], i - p, False);
}

uint TEditor::nextLine( uint p )
{
    return nextChar(lineEnd(p));
}

uint TEditor::nextWord( uint p )
{
    while( p < bufLen && isWordChar(bufChar(p)) != 0 )
        p = nextChar(p);
    while( p < bufLen && isWordChar(bufChar(p)) == 0 )
        p = nextChar(p);
    return p;
}

uint TEditor::prevLine( uint p )
{
  return lineStart(prevChar(p));
}

uint TEditor::prevWord( uint p )
{
    while( p > 0 && isWordChar(bufChar(prevChar(p))) == 0 )
        p = prevChar(p);
    while( p > 0 && isWordChar(bufChar(prevChar(p))) != 0 )
        p = prevChar(p);
    return p;
}

void TEditor::replace()
{
    TReplaceDialogRec replaceRec( findStr, replaceStr, editorFlags );
    if( editorDialog( edReplace, &replaceRec ) != cmCancel )
        {
        strcpy( findStr, replaceRec.find );
        strcpy( replaceStr, replaceRec.replace );
        editorFlags = replaceRec.options | efDoReplace;
        doSearchReplace();
        }

}

void TEditor::scrollTo( int x, int y )
{
    x = max(0, min(x, limit.x - size.x));
    y = max(0, min(y, limit.y - size.y));
    if( x != delta.x || y != delta.y )
        {
        delta.x = x;
        delta.y = y;
        update(ufView);
        }
}

Boolean TEditor::search( const char *findStr, ushort opts )
{
    ulong pos = curPtr;
    ulong i;
    do  {
        if( (opts & efCaseSensitive) != 0 )
            i = scan( &buffer[bufPtr(pos)], bufLen - pos, findStr);
        else
            i = iScan( &buffer[bufPtr(pos)], bufLen - pos, findStr);

        if( i != sfSearchFailed )
            {
            i += pos;
            if( (opts & efWholeWordsOnly) == 0 ||
                !(
                    ( i != 0 && isWordChar(bufChar(i - 1)) != 0 ) ||
                    ( i + strlen(findStr) != bufLen &&
                        isWordChar(bufChar(i + strlen(findStr)))
                    )
                 ))
                {
                lock();
                setSelect(i, i + strlen(findStr), False);
                trackCursor(Boolean(!cursorVisible()));
                unlock();
                return True;
                }
            else
                pos = i + 1;
            }
        } while( i != sfSearchFailed );
    return False;
}

void TEditor::setBufLen( uint length )
{
    bufLen = length;
    gapLen = bufSize - length;
    selStart = 0;
    selEnd = 0;
    curPtr = 0;
    delta.x = 0;
    delta.y = 0;
    curPos = delta;
    limit.x = maxLineLength;
    limit.y = countLines( &buffer[gapLen], bufLen ) + 1;
    drawLine = 0;
    drawPtr = 0;
    delCount = 0;
    insCount = 0;
    modified = False;
    update(ufView);
}

Boolean TEditor::setBufSize( uint newSize )
{
    return Boolean(newSize <= bufSize);
}

void TEditor::setCmdState( ushort command, Boolean enable )
{
    TCommandSet s;
    s += command;
    if( enable == True && (state & sfActive) != 0 )
        enableCommands(s);
    else
        disableCommands(s);
}

void TEditor::setCurPtr( uint p, uchar selectMode )
{
    uint anchor;
    if( (selectMode & smExtend) == 0 )
        anchor = p;
    else if( curPtr == selStart )
        anchor = selEnd;
    else
        anchor = selStart;

    if( p < anchor )
        {
        if( (selectMode & smDouble) != 0 )
            {
            p = prevLine(nextLine(p));
            anchor = nextLine(prevLine(anchor));
            }
        setSelect(p, anchor, True);
        }
    else
        {
        if( (selectMode & smDouble) != 0 )
            {
            p = nextLine(p);
            anchor = prevLine(nextLine(anchor));
            }
        setSelect(anchor, p, False);
        }
}

void TEditor::setSelect( uint newStart, uint newEnd, Boolean curStart )
{
    uint p;
    if( curStart != 0 )
        p = newStart;
    else
        p = newEnd;

    uchar flags = ufUpdate;

    if( newStart != selStart || newEnd != selEnd )
        if( newStart != newEnd || selStart != selEnd )
            flags = ufView;

    if( p != curPtr )
        {
        if( p > curPtr )
            {
            uint l = p - curPtr;
            memmove( &buffer[curPtr], &buffer[curPtr + gapLen], l);
            curPos.y += countLines(&buffer[curPtr], l);
            curPtr = p;
            }
        else
            {
            uint l = curPtr - p;
            curPtr = p;
            curPos.y -= countLines(&buffer[curPtr], l);
            memmove( &buffer[curPtr + gapLen], &buffer[curPtr], l);
            }
        drawLine = curPos.y;
        drawPtr = lineStart(p);
        curPos.x = charPos(drawPtr, p);
        delCount = 0;
        insCount = 0;
        setBufSize(bufLen);
    }
    selStart = newStart;
    selEnd = newEnd;
    update(flags);
}

void TEditor::setState( ushort aState, Boolean enable )
{
    TView::setState(aState, enable);
    switch( aState )
        {
        case sfActive:
            if( hScrollBar != 0 )
                hScrollBar->setState(sfVisible, enable);
            if( vScrollBar != 0 )
                vScrollBar->setState(sfVisible, enable);
            if( indicator != 0 )
                indicator->setState(sfVisible, enable);
            updateCommands();
            break;

        case sfExposed:
            if( enable == True )
                unlock();
        }
}

void TEditor::startSelect()
{
    hideSelect();
    selecting = True;
}

void TEditor::toggleInsMode()
{
    overwrite = Boolean(!overwrite);
    setState(sfCursorIns, Boolean(!getState(sfCursorIns)));
}

void TEditor::trackCursor( Boolean center )
{
    if( center == True )
        scrollTo( curPos.x - size.x + 1, curPos.y - size.y / 2);
    else
        scrollTo( max(curPos.x - size.x + 1, min(delta.x, curPos.x)),
                  max(curPos.y - size.y + 1, min(delta.y, curPos.y)));
}

void TEditor::undo()
{
    if( delCount != 0 || insCount != 0 )
        {
        selStart = curPtr - insCount;
        selEnd = curPtr;
        ushort length = delCount;
        delCount = 0;
        insCount = 0;
        insertBuffer(buffer, curPtr + gapLen - length, length, False, True);
        }
}

void TEditor::unlock()
{
    if( lockCount > 0 )
        {
        lockCount--;
        if( lockCount == 0 )
            doUpdate();
        }
}

void TEditor::update( uchar aFlags )
{
    updateFlags |= aFlags;
    if( lockCount == 0 )
        doUpdate();
}

void TEditor::updateCommands()
{
    setCmdState( cmUndo, Boolean( delCount != 0 || insCount != 0 ) );
    if( isClipboard() == False )
        {
        setCmdState(cmCut, hasSelection());
        setCmdState(cmCopy, hasSelection());
        setCmdState(cmPaste,
                    Boolean(clipboard != 0 && (clipboard->hasSelection())) );
        }
    setCmdState(cmClear, hasSelection());
    setCmdState(cmFind, True);
    setCmdState(cmReplace, True);
    setCmdState(cmSearchAgain, True);
}

Boolean TEditor::valid( ushort )
{
  return isValid;
}

#if !defined(NO_STREAMABLE)

void TEditor::write( opstream& os )
{
    TView::write( os );
    os << hScrollBar << vScrollBar << indicator
       << bufSize << (int)canUndo;
}

void *TEditor::read( ipstream& is )
{
    TView::read( is );
    int temp;
    is >> hScrollBar >> vScrollBar >> indicator
       >> bufSize >> temp;
    canUndo = Boolean(temp);
    selecting = False;
    overwrite = False;
    autoIndent = False;
    lockCount = 0;
    keyState = 0;
    initBuffer();
    if( buffer != 0 )
        isValid = True;
    else
        {
        TEditor::editorDialog( edOutOfMemory, 0 );
        bufSize = 0;
        }
    lockCount = 0;
    lock();
    setBufLen( 0 );
    return this;
}

TStreamable *TEditor::build()
{
    return new TEditor( streamableInit );
}

TEditor::TEditor( StreamableInit ) : TView( streamableInit )
{
}

#endif
