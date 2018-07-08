/*
 * TFileDialog.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TFileDialog
#define Uses_MsgBox
#define Uses_TRect
#define Uses_TFileInputLine
#define Uses_TButton
#define Uses_TLabel
#define Uses_TFileList
#define Uses_THistory
#define Uses_TScrollBar
#define Uses_TEvent
#define Uses_TFileInfoPane
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

TFileDialog::TFileDialog( const char *aWildCard,
                          const char *aTitle,
                          const char *inputName,
                          ushort aOptions,
                          uchar histId
                        ) :
    TWindowInit( &TFileDialog::initFrame ),
    TDialog( TRect( 15, 1, 64, 20 ), aTitle ),
    directory( newStr("") )
{
    options |= ofCentered;
    strcpy( wildCard, aWildCard );

    fileName = new TFileInputLine( TRect( 3, 3, 31, 4 ), 79 );
    strcpy( fileName->data, wildCard );
    insert( fileName );

    insert( new TLabel( TRect( 2, 2, 3+cstrlen(inputName), 3 ),
                        inputName,
                        fileName
                      ) );
    insert( new THistory( TRect( 31, 3, 34, 4 ), fileName, histId ) );
    TScrollBar *sb = new TScrollBar( TRect( 3, 14, 34, 15 ) );
    insert( sb );
    insert( fileList = new TFileList( TRect( 3, 6, 34, 14 ), sb ) );
    insert( new TLabel( TRect( 2, 5, 8, 6 ), filesText, fileList ) );

    ushort opt = bfDefault;
    TRect r( 35, 3, 46, 5 );

    if( (aOptions & fdOpenButton) != 0 )
        {
        insert( new TButton( r, openText, cmFileOpen, opt ) );
        opt = bfNormal;
        r.a.y += 3;
        r.b.y += 3;
        }

    if( (aOptions & fdOKButton) != 0 )
        {
        insert( new TButton( r, okText, cmFileOpen, opt ) );
        opt = bfNormal;
        r.a.y += 3;
        r.b.y += 3;
        }

    if( (aOptions & fdReplaceButton) != 0 )
        {
        insert( new TButton( r, replaceText, cmFileReplace, opt ) );
        opt = bfNormal;
        r.a.y += 3;
        r.b.y += 3;
        }

    if( (aOptions & fdClearButton) != 0 )
        {
        insert( new TButton( r, clearText, cmFileClear, opt ) );
#ifndef __UNPATCHED
	// opt = bfNormal;
#else
        opt = bfNormal;
#endif
        r.a.y += 3;
        r.b.y += 3;
        }

    insert( new TButton( r, cancelText, cmCancel, bfNormal ) );
    r.a.y += 3;
    r.b.y += 3;

    if( (aOptions & fdHelpButton) != 0 )
        {
        insert( new TButton( r, helpText, cmHelp, bfNormal ) );
#ifndef __UNPATCHED
	// opt = bfNormal;
#else
        opt = bfNormal;
#endif
        r.a.y += 3;
        r.b.y += 3;
        }

    insert( new TFileInfoPane( TRect( 1, 16, 48, 18 ) ) );

    selectNext( False );
    if( (aOptions & fdNoLoadDir) == 0 )
        readDirectory();
}

TFileDialog::~TFileDialog()
{
    delete (char *)directory;
}

void TFileDialog::shutDown()
{
    fileName = nullptr;
    fileList = nullptr;
    TDialog::shutDown();
}

static Boolean relativePath( const char *path )
{
	/* SS: changed */

	if (path[0] == '/') return False;
	else return True;
}

/* Function defined but not used
static void noWildChars( char *dest, const char *src )
{
    while( *src != EOS )
        {
        if( *src != '?' && *src != '*' )
            *dest++ = *src;
        src++;
        }
    *dest = EOS;
}
*/

/* 'src' is cast to unsigned char * so that isspace sign extends it
   correctly. */
static void trim( char *dest, const char *src )
{
    while( *src != EOS && isspace( * (const unsigned char *) src ) )
        src++;
    while( *src != EOS && !isspace( * (const unsigned char *) src ) )
        *dest++ = *src++;
    *dest = EOS;
}

void TFileDialog::getFileName( char *s )
{
    /* SS: changed */

    char buf[PATH_MAX];

    trim( buf, fileName->data );
    if( relativePath( buf ) == True )
        {
        strcpy( buf, directory );
        trim( buf + strlen(buf), fileName->data );
        }
    fexpand( buf );
    strcpy( s, buf );
}

void TFileDialog::handleEvent(TEvent& event)
{
    TDialog::handleEvent(event);
    if( event.what == evCommand )
        {
        switch( event.message.command )
            {
            case cmFileOpen:
            case cmFileReplace:
            case cmFileClear:
                endModal(event.message.command);
                clearEvent(event);
                break;
            default:
                break;
            }
        }
    else if( event.what == evBroadcast && event.message.command == cmFileDoubleClicked )
        {
        event.what = evCommand;
        event.message.command = cmOK;
        putEvent( event );
        clearEvent( event );
        }
}

void TFileDialog::readDirectory()
{
    char curDir[PATH_MAX];
    getCurDir( curDir );
    if( directory )
        delete (char *)directory;
    directory = newStr( curDir );
    fileList->readDirectory( wildCard );
}

void TFileDialog::setData( void *rec )
{
    TDialog::setData( rec );
    if( *(char *)rec != EOS && isWild( (char *)rec ) )
        {
        valid( cmFileInit );
        fileName->select();
        }
}

void TFileDialog::getData( void *rec )
{
    getFileName( (char *)rec );
}

Boolean TFileDialog::checkDirectory( const char *str )
{
    if( pathValid( str ) )
        return True;
    else
        {
        messageBox( invalidDriveText, mfError | mfOKButton );
        fileName->select();
        return False;
        }
}

Boolean TFileDialog::valid(ushort command)
{
    /* SS: changed */

    char fName[PATH_MAX];
    char name[PATH_MAX];

    if( command == 0 )
        return True;

    if( TDialog::valid( command ) )
        {
        if( command != cmCancel && command != cmFileClear )
            {
            getFileName( fName );

            if( isWild( fName ) )
                {
		/* SS: changed */

		char path[PATH_MAX];
		expandPath(fName, path, name);
                if( checkDirectory( path ) )
                    {
                    delete (char *)directory;
                    directory = newStr( path );
                    strcpy( wildCard, name );
                    if( command != cmFileInit )
                        fileList->select();
                    fileList->readDirectory( directory, wildCard );
                    }
                }
            else if( isDir( fName ) )
                {
                if( checkDirectory( fName ) )
                    {
                    delete (char *)directory;
                    strcat( fName, "/" );
                    directory = newStr( fName );
                    if( command != cmFileInit )
                        fileList->select();
                    fileList->readDirectory( directory, wildCard );
                    }
                }
            else if( validFileName( fName ) )
                return True;
            else
                {
                messageBox( invalidFileText, mfError | mfOKButton );
                return False;
                }
            }
        else
            return True;
        }
    return False;
}

#if !defined(NO_STREAMABLE)

void TFileDialog::write( opstream& os )
{
    TDialog::write( os );
    os.writeString( wildCard );
    os << fileName << fileList;
}

void *TFileDialog::read( ipstream& is )
{
    TDialog::read( is );
    is.readString( wildCard, sizeof(wildCard) );
    is >> fileName >> fileList;
    readDirectory();
    return this;
}

TStreamable *TFileDialog::build()
{
    return new TFileDialog( streamableInit );
}


#endif
