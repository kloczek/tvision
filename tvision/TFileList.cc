/*
 * TFileList.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TVMemMgr
#define Uses_MsgBox
#define Uses_TFileList
#define Uses_TRect
#define Uses_TSearchRec
#define Uses_TEvent
#define Uses_TGroup
#define Uses_TKeys
#include <tv.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <glob.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void fexpand( char * );

TFileList::TFileList( const TRect& bounds,
                      TScrollBar *aScrollBar) :
    TSortedListBox( bounds, 2, aScrollBar )
{
}

TFileList::~TFileList()
{
   if ( list() )
      destroy ( list() );
}

void TFileList::focusItem( short item )
{
    TSortedListBox::focusItem( item );
    message( owner, evBroadcast, cmFileFocused, list()->at(item) );
}

void TFileList::selectItem( short item )
{
    message( owner, evBroadcast, cmFileDoubleClicked, list()->at(item) );
}

void TFileList::getData( void * )
{
}

void TFileList::setData( void * )
{
}

ushort TFileList::dataSize()
{
    return 0;
}

void* TFileList::getKey( const char *s )
{
static TSearchRec sR;

    if( (shiftState & kbShift) != 0 || *s == '.' )
        sR.attr = FA_DIREC;
    else
        sR.attr = 0;
    strcpy( sR.name, s );

    /* SS: changed */

    for (char *p = sR.name; *p != '\0'; p++) *p = toupper(*p);
    return &sR;
}

void TFileList::getText( char *dest, short item, short maxChars )
{
    TSearchRec *f = (TSearchRec *)(list()->at(item));

    strncpy( dest, f->name, maxChars );
    dest[maxChars] = '\0';
    if( f->attr & FA_DIREC )
        strcat( dest, "/" );
}


void TFileList::readDirectory( const char *dir, const char *wildCard )
{
    char path[PATH_MAX];
    strcpy( path, dir );
    strcat( path, wildCard );
    readDirectory( path );
}

struct DirSearchRec : public TSearchRec
{
	/* SS: changed */

	void readFf_blk(char *filename, struct stat &s)
	{
		attr = FA_ARCH;
		if (S_ISDIR(s.st_mode)) attr |= FA_DIREC;
		strcpy(name, filename);
		size = s.st_size;

		ftime t;
		struct tm *broken = localtime(&s.st_mtime);
		t.ft_tsec = broken->tm_sec / 2;
		t.ft_min = broken->tm_min;
		t.ft_hour = broken->tm_hour;
		t.ft_day = broken->tm_mday;

		/*
		 * Month value should begin at 1.
		 * Date: Thu, 23 Jan 1997 11:34:50 +0100 (MET)
		 */
		t.ft_month = broken->tm_mon + 1;
		t.ft_year = broken->tm_year - 80;
		time = *(long *) &t;
	}

    void *operator new( size_t );

};

void *DirSearchRec::operator new( size_t sz )
{
    void *temp = ::operator new( sz );
    if( TVMemMgr::safetyPoolExhausted() )
        {
        delete temp;
        temp = 0;
        }
    return temp;
}

void TFileList::readDirectory( const char *aWildCard )
{
	/* SS: changed */

	DIR *dp;
	DirSearchRec *p = NULL;
	char dir[PATH_MAX];
	char file[PATH_MAX];
	char path[PATH_MAX];
	char *np;
	dirent *de;
	glob_t gl;
	struct stat s;

	strcpy( path, aWildCard );
	if (!isWild(path)) strcat(path, "*");
	fexpand( path );
	expandPath(path, dir, file);
	TFileCollection *fileList = new TFileCollection( 5, 5 );

	/* find all filenames that match our wildcards */

	/*
	 * The use of 'glob' function was proposed by:
	 * Rainer Keuchel <r_keuchel@smaug.netwave.de>
	 * Date: 18 Jan 1997 22:52:12 +0000
	 */
#ifdef GLOB_PERIOD
	if (glob(path, GLOB_PERIOD, NULL, &gl) == 0)
#else
	if (glob(path, 0, NULL, &gl) == 0)
#endif
	for (int i = 0; i < gl.gl_pathc; i++)
	{
		/* is this a regular file ? */

		if (stat(gl.gl_pathv[i], &s) == 0 && S_ISREG(s.st_mode))
		{
			if ((p = new DirSearchRec) == NULL) break;

			/* strip directory part */

			if ((np = strrchr(gl.gl_pathv[i], '/')) != NULL) np++;
			else np = gl.gl_pathv[i];
			p->readFf_blk(np, s);
			fileList->insert( p );
		}
	}
	globfree(&gl);

	/* now read all directory names */

	sprintf(path, "%s.", dir);
	if ((dp = opendir(path)) != NULL)
	{
		while ((de = readdir(dp)) != NULL)
		{
			/* we don't want these directories */

			if (strcmp(de->d_name, ".") == 0 ||
				strcmp(de->d_name, "..") == 0) continue;

			/* is it a directory ? */

			sprintf(path, "%s%s", dir, de->d_name);
			if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
			{
				if ((p = new DirSearchRec) == NULL) break;
				p->readFf_blk(de->d_name, s);
				fileList->insert( p );
			}
		}
		closedir(dp);
	}

    if( strlen( dir ) > 1 )
        {
        p = new DirSearchRec;
        if( p != 0 )
            {
		sprintf(path, "%s..", dir);
		if (stat(path, &s) == 0) p->readFf_blk("..", s);
		else
		{
			strcpy( p->name, ".." );
			p->size = 0;
			p->time = 0x210000uL;
			p->attr = FA_DIREC;
		}
		fileList->insert( p );
            }
        }

    if( p == 0 )
        messageBox( tooManyFiles, mfOKButton | mfWarning );
    newList(fileList);
    if( list()->getCount() > 0 )
        message( owner, evBroadcast, cmFileFocused, list()->at(0) );
    else
        {
        static DirSearchRec noFile;
        message( owner, evBroadcast, cmFileFocused, &noFile );
        }
}

#if !defined(NO_STREAMABLE)

TStreamable *TFileList::build()
{
    return new TFileList( streamableInit );
}

#endif
