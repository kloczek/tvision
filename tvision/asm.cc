/*
 * asm.cc
 *
 * Turbo Vision - Version 2.0
 *
 * All the assembly functions are converted in C/C++ and placed here.
 * This code was originally written by:
 * - Tommy Andreasen
 * - J”rn Sierwald
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 *
 * WARNING: very dirty code here... but it works :-)
 */

#define Uses_TEditor
#define Uses_TEvent
#define Uses_TFrame
#define Uses_TGroup
#define Uses_TPoint
#define Uses_TRect
#define Uses_TScreen
#define Uses_TTerminal
#define Uses_TView
#include <tv.h>

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <config.h>		/* configuration file */

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#include <curses.h>
#endif

/*
 * SS: this code is used to refresh the screen only if strictly necessary.
 * Date: Mon Jun 23 10:26:09 MET DST 1997
 */
static int lockRefresh = 0;

inline void doRefresh(TView *p)
{
	if (lockRefresh != 0) return;	/* we can't do any refresh */
	if (p->owner != NULL &&
		p->owner->lockFlag) return;	/* the owner is locked */
	refresh();
}

struct StaticVars1 {
  ushort *buf;
};

struct StaticVars2 {
  TView*        target;
  short         offset;
  short         y;
};

static StaticVars2 staticVars2;

int countLines( void *buf, size_t count )
{
  int anzahl=0;
  char *str=(char*) buf;
  for (unsigned int i=0; i<count; i++) {
    if (*(str++)== '\n') anzahl++;
  };
  return anzahl;
}

// These Routines are taken from Rogue Wave Tools++
size_t scan( const void *block, size_t size, const char *str )
{
  const	long   q	= 33554393L;
  const	long   q32	= q<<5;

  int	testLength	= size;
  int	patternLength	= strlen(str);

  if (patternLength <= 0) return UINT_MAX; //SS: nothing to search
  if( testLength < patternLength) return UINT_MAX;

  long	patternHash	= 0;
  long	testHash	= 0;

  const char*  testP= (const char*)block;
  const char*  patP = str;
  long   x = 1;
  int             i = patternLength-1;
  while(i--) x =  (x<<5)%q;

  for (i=0; i<patternLength; i++) {
    patternHash = ( (patternHash<<5) + *patP++  ) % q;
    testHash    = ( (testHash   <<5) + *testP++ ) % q;
  }

  testP = (const char*)block;
  const char* end = testP + testLength - patternLength;

  while (1) {

     if(testHash == patternHash)
	 return testP-(const char*)block;

     if (testP >= end) break;

     // Advance & calculate the new hash value:
     testHash = ( testHash + q32 - *testP * x                 ) % q;
     testHash = ( (testHash<<5)  + *(patternLength + testP++) ) % q;
  }
  return UINT_MAX;		// Not found.

};

size_t iScan( const void *block, size_t size, const char *str )
{
  const	long   q	= 33554393L;
  const	long   q32	= q<<5;

  int	testLength	= size;
  int	patternLength	= strlen(str);

  if (patternLength <= 0) return UINT_MAX; //SS: nothing to search
  if( testLength < patternLength) return UINT_MAX;

  long	patternHash	= 0;
  long	testHash	= 0;

  const char*  testP= (const char*)block;
  const char*  patP = str;
  long   x = 1;
  int             i = patternLength-1;
  while(i--) x =  (x<<5)%q;

  for (i=0; i<patternLength; i++) {
    patternHash = ( (patternHash<<5) + toupper(*patP++)  ) % q;
    testHash    = ( (testHash   <<5) + toupper(*testP++) ) % q;
  }

  testP = (const char*)block;
  const char* end = testP + testLength - patternLength;

  while (1) {

     if(testHash == patternHash)
	 return testP-(const char*)block;

     if (testP >= end) break;

     // Advance & calculate the new hash value:
     testHash = ( testHash + q32 - toupper(*testP) * x                 ) % q;
     testHash = ( (testHash<<5)  + toupper(*(patternLength + testP++)) ) % q;
  }
  return UINT_MAX;		// Not found.
};

// edits.cpp defines functions previously found in edits.asm
//
// This file written by J”rn Sierwald based on a file
// written by Tommy Andreasen

char TEditor::bufChar( uint p )
{
    return buffer[p + ((p >= curPtr) ? gapLen : 0)];
}

uint TEditor::bufPtr( uint p )
{
    return (p >= curPtr) ? p + gapLen : p;
}

#if 0
void TEditor::formatLine( void *DrawBuf, uint LinePtr,
                          int Width, ushort Color )
{
    uint p = LinePtr;
    while ((p < curPtr) && (buffer[p] != 0x0D) && (p - LinePtr <= Width))
    {
        ((ushort *) DrawBuf) [p - LinePtr] = buffer[p] + ((Color & 0xFF) << 8);
        p++;
    }

    if (p >= curPtr)
    {
    p += gapLen;

    while ((p < bufSize) && (buffer[p] != 0x0D) &&
           (p - gapLen - LinePtr <= Width))
    {
        ((ushort *) DrawBuf) [p - gapLen - LinePtr] =
                                        buffer[p] + ((Color & 0xFF) << 8);
        p++;
    }
    } else
        p += gapLen;

    while (p - gapLen - LinePtr <= Width)
    {
        ((ushort *) DrawBuf) [p - gapLen - LinePtr] =
                                        ' ' + ((Color & 0xFF) << 8);
        p++;
    }
}
#endif

void TEditor::formatLine( void *DrawBuf, uint LinePtr,
                          int Width, ushort Color )
{
  ushort i = 0;       // index in the DrawBuf
  size_t p = LinePtr; // index in the Buffer
  ushort curColor;

  /* draw the first part of the buffer */

  while ((p < curPtr) && (buffer[p] != '\n') && (i <= Width)) {
    curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) :
	((Color & 0xFF) << 8);
    if (buffer[p] == 0x9) {
      do {
        ((ushort *) DrawBuf) [i] = ' ' + curColor;
        i++;
      } while ((i % 8) && (i <= Width));
      p++;
    } else {
      ((ushort *) DrawBuf) [i] = curColor | (uchar)buffer[p];
      p++; i++;
    }
  }

  /* draw the second part of the buffer */

  if (p >= curPtr)
  {
    p += gapLen;

    while ((p < bufSize) && (buffer[p] != '\n') && (i <= Width))
    {
      curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) :
	((Color & 0xFF) << 8);
      if (buffer[p] == 0x9) {
        do {
          ((ushort *) DrawBuf) [i] = ' ' + curColor;
          i++;
        } while ((i % 8) && (i <= Width));
        p++;
      } else {
        ((ushort *) DrawBuf) [i] = curColor | (uchar)buffer[p];
        p++; i++;
      }
    }
  }
//  } else
//    p += gapLen; /* XXX */

  /* add some trailing spaces to fill the last part of the line */

  while (i < Width)	//buffer overflow problem fixed
  {
    curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) :
	((Color & 0xFF) << 8);
    ((ushort *) DrawBuf) [i] = ' ' + curColor;
//    p++; i++; /* XXX */
    i++;
  }
}

uint TEditor::lineEnd( uint p )
{
/*
    while (p < curPtr)
        if (buffer[p] == 0x0D)
            return p;
        else
            p++;

    if (curPtr == bufLen)
        return curPtr;

    while (p + gapLen < bufLen)
        if (buffer[p + gapLen] == 0x0D)
            return p;
        else
            p++;

    return p;
*/
    if (p < curPtr)
    {
	/* SS: changed */

        while (p < curPtr)
            if (buffer[p] == '\n')
                return p;
            else
                p++;

        if (curPtr == bufLen)
            return bufLen;


    }
    else
    {
        if (p == bufLen)
            return bufLen;
    }

/* SS: changed */

    while (p + gapLen < bufSize)
        if (buffer[p + gapLen] == '\n')
            return p;
        else
            p++;

    return p;

}

uint TEditor::lineStart( uint p )
{
/*
    while (p - gapLen > curPtr)
        if (buffer[--p + gapLen] == 0x0D)
            return p + 2;

    if (curPtr == 0)
        return 0;

    while (p > 0)
        if (buffer[--p] == 0x0D)
            return p + 2;

    return 0;
*/
/* SS: changed */

    while (p > curPtr)
        if (buffer[--p + gapLen] == '\n')
                return p + 1;

    if (curPtr == 0)
        return 0;

/* SS: changed */

    while (p > 0)
        if (buffer[--p] == '\n')
                return p + 1;

    return 0;
}

uint TEditor::nextChar( uint p )
{
    if (p == bufLen)   return p;

    /* SS: changed */

    return ++p;
}

uint TEditor::prevChar( uint p )
{
    if (p == 0)   return p;

    /* SS: changed */

    return --p;
}

/*------------------------------------------------------------*/
/* filename -       exposed.cpp                               */
/*                                                            */
/* function(s)                                                */
/*                  Tview exposed member function             */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*    This file Copyright (c) 1993 by J”rn Sierwald           */
/*                                                            */
/*                                                            */
/*------------------------------------------------------------*/

int TView::exposedRec1(short x1, short x2, TView* p ) {
  while (1) {
/*20*/
    p=p->next;
    if (p==staticVars2.target) { // alle durch
      return exposedRec2( x1, x2, p->owner );
    };
    if ( !(p->state & sfVisible) || staticVars2.y<p->origin.y) continue; // keine Verdeckung

    if ( staticVars2.y<p->origin.y+p->size.y ) {
      // šberdeckung m”glich.
      if (x1<p->origin.x) { // f„ngt links vom Object an.
        if (x2<=p->origin.x) continue; // links vorbei
        if (x2>p->origin.x+p->size.x) {
          if (exposedRec1( x1, p->origin.x, p )) return 1;
          x1=p->origin.x+p->size.x;
        }
        else
          x2=p->origin.x;
      } else {
        if ( x1<p->origin.x+p->size.x ) x1=p->origin.x+p->size.x;
        if ( x1>=x2 ) return 0; // komplett verdeckt.
      };
    };

  }; // while

}

int TView::exposedRec2( short x1, short x2, TView* p ) {

  if (!(p->state & sfVisible)) return 0;
  if ( !p->owner || p->owner->buffer ) return 1;

  StaticVars2 savedStatics = staticVars2;

  staticVars2.y += p->origin.y;
  x1 += p->origin.x;
  x2 += p->origin.x;
  staticVars2.target=p;

  TGroup* g=p->owner;
  if (staticVars2.y<g->clip.a.y || staticVars2.y >= g->clip.b.y) {
    staticVars2 = savedStatics;
    return 0;
  };
  if (x1<g->clip.a.x) x1 = g->clip.a.x;
  if (x2>g->clip.b.x) x2 = g->clip.b.x;
  if (x1>=x2) {
    staticVars2 = savedStatics;
    return 0;
  };

  int retValue = exposedRec1( x1, x2, g->last );
  staticVars2 = savedStatics;
  return retValue;
}

Boolean TView::exposed() {
  if ( !(state & sfExposed) || size.x <= 0 || size.y <= 0 ) return Boolean(0);
  for (short y=0; y<size.y; y++) {
    staticVars2.y=y;
    if (exposedRec2( 0, size.x, this )) return Boolean(1);
  };
  return Boolean(0);
}

/*------------------------------------------------------------*/
/* filename -       framelin.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TFrame frameLine member function          */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*    This file Copyright (c) 1993 by J”rn Sierwald           */
/*                                                            */
/*                                                            */
/*------------------------------------------------------------*/

/* Erkl„rung der Mask:

       Bit 0
         |
  Bit 3 - - Bit 1
         |
       Bit 2

Wenn z.B. sichergestellt werden soll, dass eine linke obere Ecke im
Muster vorhanden ist, nimmt man :
  mask |= 0x06 .
*/

void TFrame::frameLine( TDrawBuffer& frameBuf, short y, short n, uchar color )
{
  unsigned char frameMask[maxViewWidth];
  short int i;
  frameMask[0]=initFrame[n];
  for (i=1; i+1<size.x; i++) {
    frameMask[i]=initFrame[n+1];
  };
  frameMask[size.x-1]=initFrame[n+2];

  TView* p;
  p=owner->last;
  while (1) {
    p=p->next;
    if (p==this) break;
    if ((p->options & ofFramed) && (p->state & sfVisible)) {
      unsigned char mask1, mask2;
      if (y+1<p->origin.y) continue;
      else if (y+1==p->origin.y) { mask1=0x0A; mask2=0x06;}
      else if (y==p->origin.y+p->size.y) { mask1=0x0A; mask2=0x03;}
      else if (y<p->origin.y+p->size.y) { mask1=0; mask2=0x05;}
      else continue;
      unsigned short xMin=p->origin.x;
      unsigned short xMax=p->origin.x+p->size.x;
      if (xMin<1) xMin=1;
      if (xMax>size.x-1) xMax=size.x-1;
      if (xMax>xMin) {
        if (mask1==0) {
          frameMask[xMin-1] |= mask2;
          frameMask[xMax]   |= mask2;
        } else {
          frameMask[xMin-1] |= mask2;
          frameMask[xMax]   |= (mask2 ^ mask1);
          for (i=xMin; i< xMax; i++) {
            frameMask[i] |= mask1;
          }
        };
      };
    };
  }; // while
  unsigned short* dest=frameBuf.data;
  i=size.x;
  short int i1=0;
  while (i--) {
    *dest++= ( ((unsigned short)color) << 8 ) + (unsigned char) frameChars[frameMask[i1]];
    i1++;
  } /* endwhile */
};

/*------------------------------------------------------------*/
/* filename -       tgrmv.cpp                                 */
/*                                                            */
/* function(s)                                                */
/*                  TGroup removeView member function         */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*    This file Copyright (c) 1993 by J”rn Sierwald           */
/*                                                            */
/*                                                            */
/*------------------------------------------------------------*/

void TGroup::removeView( TView *p ) {
  if (last) {
    TView *cur=last;
    while (1) {
      if (p==cur->next) {
        cur->next=p->next;
        if (last==p) {
          if (cur->next==p) last=0;
          else last=cur;
          break;
        };
      };
      if (cur->next==last) break;
      cur=cur->next;
    };
  }; // endif
};

/*------------------------------------------------------------*/
/* filename -       tvcursor.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  Tview resetCursor member function         */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*    This file Copyright (c) 1993 by J”rn Sierwald           */
/*                                                            */
/*                                                            */
/*------------------------------------------------------------*/

void TView::resetCursor() {
  TView *p,*p2;
  TGroup *g;
  TPoint cur;
  if  ((state & (sfVisible | sfCursorVis | sfFocused))
             == (sfVisible | sfCursorVis | sfFocused) )
  {
    p=this;
    cur=cursor;
    while (1) {
      if (!(cur.x>=0 && cur.x<p->size.x
          && cur.y>=0 && cur.y<p->size.y)) {
        break;
      };
      cur.x += p->origin.x;
      cur.y += p->origin.y;
      p2 =p;
      g=p->owner;
      if (g==0) {
        //cursor setzen

	/*
	 * SS: we should change the cursor size according to the sfCursorIns
	 * flag in the state variable:
	 *
	 * if (state & sfCursorIns) {
         *     setBigCursor
         * } else {
         *     setSmallCursor
         * }
	 *
	 * Is there a way to do it under linux ?
	 */
	TScreen::moveCursor(cur.x, cur.y);
	TScreen::drawCursor(1);
        return;
      };
      if (!(g->state & sfVisible)) break;
      p=g->last;
      {
        label1:
        p=p->next;
        if (p==p2) { // alle durchgesucht.
          p=p->owner;
          continue;
        };
        if ((p->state & sfVisible)
            && cur.x>=p->origin.x
            && cur.x<p->size.x+p->origin.x
            && cur.y>=p->origin.y
            && cur.y<p->size.y+p->origin.y) {
          break; // Cursor wird verdeckt.
        };
        goto label1;
      };
    };
  }
  // no cursor, please.
  TScreen::drawCursor(0);
}

// TTPRVLNS.CPP
// Copyright 1994 by J”rn Sierwald
//
// C++ Version of ttprvlns.asm
// implementation of TTerminal::prevLines(...)
//
// "You don't need assembler to write obfuscated code."

ushort TTerminal::prevLines(ushort pos, ushort lines ) {
  if (lines==0) { bufInc(pos); bufInc(pos); return pos; };
  // I don't see the logic in the previous line. But that's what
  // the .asm file says.

  if (pos==queBack) return queBack; // Nothing to do

  bufDec(pos); // pos might be pointing to a '\n'

  if (pos<queBack) {
    while ( !( buffer[pos]=='\n' && !--lines ) && pos-- );
    if (lines) pos=bufSize-1;
  };

  if (lines)
/* SS: we should check if there is an available character before read it */
	while (pos > queBack && !( buffer[pos]=='\n' && !--lines ))
	{
		pos--;
	}

  if (lines)
    return queBack;
  else
    bufInc(pos);

  return pos;
};

// TVWRITE.CPP
// Copyright 1993,1994 by J”rn Sierwald
//
// C++ Version of tvwrite.asm
//
// It doesn't look beautiful, but what the heck..

extern TPoint shadowSize;
extern uchar shadowAttr;

static StaticVars1 staticVars1;

void TView::writeViewRec1(short x1, short x2, TView* p, int shadowCounter ) {
  while (1) {
/*20*/
    p=p->next;
    if (p==staticVars2.target) { // alle durch
      // printit!
      if (p->owner->buffer) {

/*
 * SS: now we should remove the mouse pointer from the screen.  This is
 * not necessary because we have a copy of the screen.
 *
 * if (p->owner->buffer == TScreen::screenBuffer) TScreen::drawMouse(0);
 */

	if (shadowCounter == 0)
	{
		/* SS: writes a row of data to the screen */

		if (p->owner->buffer == TScreen::screenBuffer)
			TScreen::writeRow(
			p->owner->size.x * staticVars2.y + x1,
			staticVars1.buf + (x1 - staticVars2.offset),
			x2 - x1);
		memmove(p->owner->buffer + p->owner->size.x * staticVars2.y +
			x1, staticVars1.buf + x1 - staticVars2.offset,
			(x2 - x1) * 2);
	} else { // paint with shadowAttr
		int l = x2 - x1;
		int dst1 = p->owner->size.x * staticVars2.y + x1;
		ushort *dst = p->owner->buffer + dst1;
		ushort *src= staticVars1.buf + (x1 - staticVars2.offset);
		while (l--)
		{
			ushort d = (*src++ & 0xff) | (shadowAttr << 8);

			/* SS: writes a character on the screen */

			if (p->owner->buffer == TScreen::screenBuffer)
				TScreen::writeRow(dst1++, &d, 1);
			*dst++ = d;
	        }
	}

/* SS: draws mouse pointer */

if (p->owner->buffer == TScreen::screenBuffer) TScreen::drawMouse(1);
      };
      if (p->owner->lockFlag==0) writeViewRec2( x1, x2, p->owner, shadowCounter );
      return ; // (p->owner->lockFlag==0);
    };
    if ( !(p->state & sfVisible) || staticVars2.y<p->origin.y) continue; // keine Verdeckung

    if ( staticVars2.y<p->origin.y+p->size.y ) {
      // šberdeckung m”glich.
      if (x1<p->origin.x) { // f„ngt links vom Object an.
        if (x2<=p->origin.x) continue; // links vorbei
        writeViewRec1( x1, p->origin.x, p, shadowCounter );
        x1=p->origin.x;
      };
                   //  if (x1>=p->origin.x) {
      if ( x2<=p->origin.x+p->size.x ) return; // komplett verdeckt.
      if ( x1<p->origin.x+p->size.x ) x1=p->origin.x+p->size.x;
                  // if ( x1>=p->origin.x+p->size.x ) { // k”nnte h”chstens im Schatten liegen
      if ( (p->state & sfShadow) && (staticVars2.y>=p->origin.y+shadowSize.y)) {
        if (x1>=p->origin.x+p->size.x+shadowSize.x) {
          continue; // rechts vorbei
        } else {
          shadowCounter++;
          if (x2<=p->origin.x+p->size.x+shadowSize.x) {
            continue; // alles im Schatten
          } else { // aufteilen Schattenteil, rechts daneben
            writeViewRec1( x1, p->origin.x+p->size.x+shadowSize.x, p, shadowCounter );
            x1=p->origin.x+p->size.x+shadowSize.x;
            shadowCounter--;
            continue;
          };
        };
      } else {
        continue; // rechts vorbei, 1.Zeile hat keinen Schatten
      };
    };
    if ( (p->state & sfShadow) && (staticVars2.y < p->origin.y+p->size.y+shadowSize.y) ) {
      // im y-Schatten von Object?
      if (x1<p->origin.x+shadowSize.x) {
        if (x2<= p->origin.x+shadowSize.x) continue; // links vorbei
        writeViewRec1( x1, p->origin.x+shadowSize.x, p, shadowCounter );
        x1 = p->origin.x+shadowSize.x;
      };
      if (x1>=p->origin.x+shadowSize.x+p->size.x) continue;
      shadowCounter++;
      if (x2<=p->origin.x+p->size.x+shadowSize.x) {
        continue; // alles im Schatten
      } else { // aufteilen Schattenteil, rechts daneben
        writeViewRec1( x1, p->origin.x+p->size.x+shadowSize.x, p, shadowCounter );
        x1=p->origin.x+p->size.x+shadowSize.x;
        shadowCounter--;
        continue;
      };

    } else { // zu weit unten
      continue;
    };

  }; // while

}

void TView::writeViewRec2( short x1, short x2, TView* p, int shadowCounter ) {
  if (!(p->state & sfVisible) || p->owner==0 ) return;

  StaticVars2 savedStatics = staticVars2;

  staticVars2.y += p->origin.y;
  x1 += p->origin.x;
  x2 += p->origin.x;
  staticVars2.offset += p->origin.x;
  staticVars2.target=p;

  TGroup* g=p->owner;
  if (staticVars2.y<g->clip.a.y || staticVars2.y >= g->clip.b.y) {
    staticVars2 = savedStatics;
    return;
  };
  if (x1<g->clip.a.x) x1 = g->clip.a.x;
  if (x2>g->clip.b.x) x2 = g->clip.b.x;
  if (x1>=x2) {
    staticVars2 = savedStatics;
    return;
  };

  writeViewRec1( x1, x2, g->last, shadowCounter );
  staticVars2 = savedStatics;
}

void TView::writeView( short x1, short x2, short y, const void* buf ) {
//  cerr << "Output ";
  if (y<0 || y>=size.y) return;
  if (x1<0) x1=0;
  if (x2>size.x) x2=size.x;
  if (x1>=x2) return;
  staticVars2.offset=x1;
  staticVars1.buf= (ushort*) buf;
  staticVars2.y=y;
  writeViewRec2( x1, x2, this, 0 );
	doRefresh(this);
}

void TView::writeBuf( short x, short y, short w, short h, const void *buf) {
	lockRefresh++;		/* stop the refresh */
  for (int i=0; i<h; i++) {
    writeView( x,x+w,y+i,(ushort*) buf + w*i );
  } /* endfor */
	lockRefresh--;		/* allow the refresh */
	doRefresh(this);
}

void TView::writeChar( short x, short y, char c, uchar color, short count) {
  ushort b[maxViewWidth];
  ushort myChar= ( ((ushort)mapColor(color))<<8 ) + (unsigned char) c;
  short count2=count;
  if (x<0) x=0;
  if (x+count>maxViewWidth) return;
  ushort* p = b;
  while ( count-- ) *p++ = myChar;
  writeView( x, x+count2, y, b);
}

void TView::writeLine( short x, short y, short w, short h, const void *buf) {
  if (h==0) return;
	lockRefresh++;		/* stop the refresh */
  for (int i=0; i<h; i++) {
    writeView ( x, x+w, y+i, buf );
  };
	lockRefresh--;		/* allow the refresh */
	doRefresh(this);
}

void TView::writeStr( short x, short y, const char *str, uchar color) {
  if (!str) return;
  ushort l= strlen(str);
  if (l==0) return;
  if (l>maxViewWidth) l=maxViewWidth;
  ushort l2=l;
  ushort myColor=( (ushort)mapColor(color) ) << 8;
  ushort b[maxViewWidth];
  ushort* p = b;
  while ( *p++ = myColor+(*(const unsigned char*)str++), --l );
  writeView ( x, x+l2, y, b );
}
