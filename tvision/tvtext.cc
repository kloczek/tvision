/*
 * tvtext.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TScreen
#define Uses_TRadioButtons
#define Uses_TMenuBox
#define Uses_TFrame
#define Uses_TIndicator
#define Uses_THistory
#define Uses_TColorSelector
#define Uses_TMonoSelector
#define Uses_TColorDialog
#define Uses_TInputLine
#define Uses_TStatusLine
#define Uses_TCheckBoxes
#define Uses_TScrollBar
#define Uses_TButton
#define Uses_TDirListBox
#define Uses_TFileEditor
#define Uses_TEditWindow
#define Uses_TFileList
#define Uses_TProgram
#define Uses_MsgBox
#define Uses_TChDirDialog
#define Uses_TFileDialog
#define Uses_TFileInfoPane
#define Uses_TDeskTop
#define Uses_TPXPictureValidator
#define Uses_TFilterValidator
#define Uses_TRangeValidator
#define Uses_TStringLookupValidator
#define Uses_TListViewer
#define Uses_TOutlineViewer
#include <tv.h>
#include <help.h>

#include <ctype.h>

const uchar specialChars[] =
{
    175, 174, 26, 27, ' ', ' '
};

static const char altCodes1[] =
    "QWERTYUIOP\0\0\0\0ASDFGHJKL\0\0\0\0\0ZXCVBNM";
static const char altCodes2[] = "1234567890-=";

char getAltChar(ushort keyCode)
{
    if ((keyCode & 0xff) == 0)
        {
        ushort tmp = (keyCode >> 8);

        if( tmp == 2 )
            return '\xF0';      // special case to handle alt-Space

        else if( tmp >= 0x10 && tmp <= 0x32 )
            return altCodes1[tmp-0x10];     // alt-letter

        else if( tmp >= 0x78 && tmp <= 0x83 )
            return altCodes2[tmp - 0x78];   // alt-number

        }
    return 0;
}

ushort getAltCode(char c)
{
    if( c == 0 )
        return 0;

    c = toupper(c);

    /* SS: this makes g++ happy */

    if( (unsigned char) c == 0xf0)
        return 0x200;       // special case to handle alt-Space

    int i;
    for( i = 0; i < (int)sizeof( altCodes1 ); i++)
       if( altCodes1[i] == c )
        return (i+0x10) << 8;

    for( i = 0; i < (int)sizeof( altCodes2); i++)
        if (altCodes2[i] == c)
            return (i+0x78) << 8;

    return 0;
}

inline uchar lo(ushort w) { return w & 0xff; }
inline uchar hi(ushort w) { return w >> 8; }

char getCtrlChar(ushort keyCode)
{
	if ( (lo(keyCode)!= 0) && (lo(keyCode) <= ('Z'-'A'+1)))
		return lo(keyCode) + 'A' - 1;
	else
		return 0;
}

ushort getCtrlCode(uchar ch)
{
	return getAltCode(ch)|(((('a'<=ch)&&(ch<='z'))?(ch&~0x20):ch)-'A'+1);
}

const char * TRadioButtons::button = " ( ) ";

const char * TMenuBox::frameChars = " \332\304\277  \300\304\331  \263 \263  \303\304\264 ";

const char TFrame::initFrame[19] =
  "\x06\x0A\x0C\x05\x00\x05\x03\x0A\x09\x16\x1A\x1C\x15\x00\x15\x13\x1A\x19";

char TFrame::frameChars[33] =
    "   À ³ÚÃ ÙÄÁ¿´ÂÅ   È ºÉÇ ¼ÍÏ»¶Ñ "; // for UnitedStates code page

const char * TFrame::closeIcon = "[~\xFE~]";
const char * TFrame::zoomIcon = "[~\x18~]";
const char * TFrame::unZoomIcon = "[~\x12~]";
const char * TFrame::dragIcon = "~ÄÙ~";

char TIndicator::dragFrame = '\xCD';
char TIndicator::normalFrame = '\xC4';

const char * THistory::icon = "\xDE~\x19~\xDD";

char TColorSelector::icon = '\xDB';

const char * TMonoSelector::button = " ( ) ";
const char * TMonoSelector::normal = "Normal";
const char * TMonoSelector::highlight = "Highlight";
const char * TMonoSelector::underline = "Underline";
const char * TMonoSelector::inverse = "Inverse";

const char * TColorDialog::colors = "Colors";
const char * TColorDialog::groupText = "~G~roup";
const char * TColorDialog::itemText = "~I~tem";
const char * TColorDialog::forText = "~F~oreground";
const char * TColorDialog::bakText = "~B~ackground";
const char * TColorDialog::textText = "Text ";
const char * TColorDialog::colorText = "Color";
const char * TColorDialog::okText = "O~K~";
const char * TColorDialog::cancelText = "Cancel";

const char TInputLine::rightArrow = '\x10';
const char TInputLine::leftArrow = '\x11';

const char * TStatusLine::hintSeparator = "\xB3 ";

const char * TCheckBoxes::button = " [ ] ";

TScrollChars TScrollBar::vChars = {'\x1E', '\x1F', '\xB1', '\xFE', '\xB2'};
TScrollChars TScrollBar::hChars = {'\x11', '\x10', '\xB1', '\xFE', '\xB2'};

const char * TButton::shadows = "\xDC\xDB\xDF";
const char * TButton::markers = "[]";

const char * TDirListBox::pathDir   = "ÀÄÂ";
const char * TDirListBox::firstDir  =   "ÀÂÄ";
const char * TDirListBox::middleDir =   " ÃÄ";
const char * TDirListBox::lastDir   =   " ÀÄ";
const char * TDirListBox::drives = "Drives";
const char * TDirListBox::graphics = "ÀÃÄ";

const char * TFileEditor::backupExt = "~";

const char * TPXPictureValidator::errorMsg = "Error in picture format.\n %s";
const char * TFilterValidator::errorMsg = "Invalid character in input";
const char * TRangeValidator::errorMsg = "Value not in the range %ld to %ld";
const char * TStringLookupValidator::errorMsg = "Input is not in list of valid strings";

const char * TRangeValidator::validUnsignedChars = "+0123456789";
const char * TRangeValidator::validSignedChars = "+-0123456789";

const char * TListViewer::emptyText = "<empty>";

const char * THelpWindow::helpWinTitle = "Help";
const char * THelpFile::invalidContext =
    "\n No help available in this context.";

const char * TEditWindow::clipboardTitle = "Clipboard";
const char * TEditWindow::untitled = "Untitled";

const char * TFileList::tooManyFiles = "Too many files.";

const char * TProgram::exitText = "~Alt-X~ Exit";

const char * MsgBoxText::yesText = "~Y~es";
const char * MsgBoxText::noText = "~N~o";
const char * MsgBoxText::okText = "O~K~";
const char * MsgBoxText::cancelText = "Cancel";
const char * MsgBoxText::warningText = "Warning";
const char * MsgBoxText::errorText = "Error";
const char * MsgBoxText::informationText = "Information";
const char * MsgBoxText::confirmText = "Confirm";

const char * TChDirDialog::changeDirTitle = "Change Directory";
const char * TChDirDialog::dirNameText = "Directory ~n~ame";
const char * TChDirDialog::dirTreeText = "Directory ~t~ree";
const char * TChDirDialog::okText = "O~K~";
const char * TChDirDialog::chdirText = "~C~hdir";
const char * TChDirDialog::revertText = "~R~evert";
const char * TChDirDialog::helpText = "Help";
const char * TChDirDialog::drivesText = "Drives";
const char * TChDirDialog::invalidText = "Invalid directory";

const char * TFileDialog::filesText = "~F~iles";
const char * TFileDialog::openText = "~O~pen";
const char * TFileDialog::okText = "O~K~";
const char * TFileDialog::replaceText = "~R~eplace";
const char * TFileDialog::clearText = "~C~lear";
const char * TFileDialog::cancelText = "Cancel";
const char * TFileDialog::helpText = "~H~elp";
const char * TFileDialog::invalidDriveText = "Invalid drive or directory";
const char * TFileDialog::invalidFileText = "Invalid file name.";

const char * TFileInfoPane::pmText = "p";
const char * TFileInfoPane::amText = "a";
const char * const TFileInfoPane::months[] =
    {
    "","Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
    };

char TDeskTop::defaultBkgrnd = '\xB0';

char TListViewer::separatorChar = '\xB3';
const char* TOutlineViewer::graphChars = "\x20\xB3\xC3\xC0\xC4\xC4+\xC4";
