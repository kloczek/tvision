/*
 * TVision example: a simple text editor
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_MsgBox
#define Uses_TApplication
#define Uses_TBackground
#define Uses_TButton
#define Uses_TChDirDialog
#define Uses_TCheckBoxes
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TEditWindow
#define Uses_TEditor
#define Uses_TFileDialog
#define Uses_THistory
#define Uses_TInputLine
#define Uses_TKeys
#define Uses_TLabel
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TSItem
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu

#include <fstream>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <tv.h>

//new command codes; standard commands are defined in views.h

enum
{
    cmAbout = 100,
    cmShowClip
};

//main class of our application

class TVDemo: public TApplication
{
private:
    TEditWindow *clipWindow;
    static TDialog *createFindDialog();
    static TDialog *createReplaceDialog();
    static ushort doEditorDialog(int dialog, ...);
    static ushort doExecute(TDialog *p, void *data);
    static ushort doReplacePrompt(TPoint &cursor);
    void aboutBox();
    void cascade();
    void changeDir();
    void newFile();
    void openFile(const char *fileSpec);
    void shell();
    void showClipboard();
    void tile();
public:
    TVDemo();
    static TMenuBar *initMenuBar(TRect r);
    static TStatusLine *initStatusLine(TRect r);
    virtual void handleEvent(TEvent& Event);
    virtual void idle();
};

//constructor

TVDemo::TVDemo(): TProgInit(&TVDemo::initStatusLine, &TVDemo::initMenuBar,
    &TVDemo::initDeskTop)
{
    //Variable editorDialog is a function pointer used by TEditor objects to
    //display various dialog boxes. The default editorDialog, defEditorDialog
    //(implemented in editstat.cc), simply returns cmCancel. The following
    //line replaces it with our version.

    TEditor::editorDialog = doEditorDialog;

    //create a clipboard

    clipWindow = new TEditWindow(deskTop->getExtent(), 0, wnNoNumber);
    if (clipWindow != 0)
    {
	//remember who is the clipboard; all editor istances will
	//refer to this one when doing clipboard operations

	TEditor::clipboard = (TEditor *) clipWindow->editor;

	//put the clipboard in the background

	clipWindow->hide();
	deskTop->insert(clipWindow);
    }
}

//shows the about dialog

void TVDemo::aboutBox()
{
    TDialog *box = new TDialog(TRect(0, 0, 32, 11), "About");
    box->insert(new TStaticText(TRect(1, 2, 1+30, 2+5),
	"\003TVEDIT\n"
	"\003A simple text editor\n\n"
	"\003Ported from Turbo Pascal 6.0 by Sergio Sigala"));
    box->insert(new TButton(TRect(11, 8, 11+10, 10), "O~K~", cmOK,
	bfDefault));
    box->options |= ofCentered;
    executeDialog(box);
}

//moves all the windows in a cascade-like fashion

void TVDemo::cascade()
{
    deskTop->cascade(deskTop->getExtent());
}

//changes the current working directory

void TVDemo::changeDir()
{
    TView *d = validView(new TChDirDialog(0, cmChangeDir));

    if (d != 0)
    {
	deskTop->execView(d);
	destroy(d);
    }
}

//creates a find dialog and then returns his address

TDialog *TVDemo::createFindDialog()
{
    TInputLine *p;
    TDialog *d = new TDialog(TRect(0, 0, 38, 12), "Find");
    if (!d) return 0;

    d->options |= ofCentered;

    d->insert(p = new TInputLine(TRect(3, 3, 32, 4), 80));
    d->insert(new TLabel(TRect(2, 2, 15, 3), "~T~ext to find", p));
    d->insert(new THistory(TRect(32, 3, 35, 4), p, 10));

    d->insert(new TCheckBoxes(TRect(3, 5, 35, 7),
	new TSItem("~C~ase sensitive",
	new TSItem("~W~hole words only", 0))));

    d->insert(new TButton(TRect(14, 9, 24, 11), "O~K~", cmOK, bfDefault));
    d->insert(new TButton(TRect(14+12, 9, 24+12, 11), "Cancel", cmCancel,
	bfNormal));
    d->selectNext(False);
    return d;
}

//creates a replace dialog and then returns his address

TDialog *TVDemo::createReplaceDialog()
{
    TInputLine *p;
    TDialog *d = new TDialog(TRect(0, 0, 40, 16), "Replace");
    if (!d) return 0;

    d->options |= ofCentered;

    d->insert(p = new TInputLine(TRect(3, 3, 34, 4), 80));
    d->insert(new TLabel(TRect(2, 2, 15, 3), "~T~ext to find", p));
    d->insert(new THistory(TRect(34, 3, 37, 4), p, 10));

    d->insert(p = new TInputLine(TRect(3, 6, 34, 7), 80));
    d->insert(new TLabel(TRect(2, 5, 12, 6), "~N~ew text", p));
    d->insert(new THistory(TRect(34, 6, 37, 7), p, 11));

    d->insert(new TCheckBoxes(TRect(3, 8, 37, 12),
	new TSItem("~C~ase sensitive",
	new TSItem("~W~hole words only",
	new TSItem("~P~rompt on replace",
	new TSItem("~R~eplace all", 0))))));

    d->insert(new TButton(TRect(17, 13, 27, 15), "O~K~", cmOK, bfDefault));
    d->insert(new TButton(TRect(28, 13, 38, 15), "Cancel", cmCancel,
	bfNormal));
    d->selectNext(False);
    return d;
}

//This is a function used by TEditor objects to display various dialog boxes.
//Since dialog boxes are very application-dependent, a TEditor object does not
//display its own dialog boxes directly. Instead it controls them through this
//function.

ushort TVDemo::doEditorDialog(int dialog, ...)
{
    va_list ap;
    void *info = 0;

    va_start(ap, dialog);	//initializes the variable argument list
    info = va_arg(ap, void *);	//get first parameter (if any)
    va_end(ap);
    switch (dialog)
    {
    case edOutOfMemory:
	return messageBox("Not enough memory for this operation",
	mfError + mfOKButton);
    case edReadError:
	return messageBox(mfError + mfOKButton, "Error reading file %s", info);
    case edWriteError:
	return messageBox(mfError + mfOKButton, "Error writing file %s", info);
    case edCreateError:
	return messageBox(mfError + mfOKButton, "Error creating file %s",
	info);
    case edSaveModify:
	return messageBox(mfInformation + mfYesNoCancel,
	"%s has been modified. Save?", info);
    case edSaveUntitled:
	return messageBox("Save untitled file?",
	mfInformation + mfYesNoCancel);
    case edSaveAs:
	return doExecute(new TFileDialog("*.*", "Save file as", "~N~ame",
	fdOKButton, 101), info);
    case edFind:
	return doExecute(createFindDialog(), info);
    case edSearchFailed:
	return messageBox("Search string not found", mfError + mfOKButton);
    case edReplace:
	return doExecute(createReplaceDialog(), info);
    case edReplacePrompt:
    	va_start(ap, dialog);
	TPoint *cursor = va_arg(ap, TPoint *);
	va_end(ap);
	return doReplacePrompt(*cursor);
    }
}

//executes a dialog in modal state; similar to TProgram::execute(), but this
//version is fully static

ushort TVDemo::doExecute(TDialog *p, void *data)
{
    ushort result = cmCancel;
    if (1) //validView(p))
    {
	if (data) p->setData(data);
	result = deskTop->execView(p);
	if (result != cmCancel && data) p->getData(data);
	destroy(p);
    }
    return result;
}

ushort TVDemo::doReplacePrompt(TPoint &cursor)
{
    TRect r(0, 2, 40, 9);

    r.move((deskTop->size.x - r.b.x) / 2, 0);
    TPoint lower = deskTop->makeGlobal(r.b);
    if (cursor.y <= lower.y) r.move(0, (deskTop->size.y - r.b.y - 2));

    return messageBoxRect(r, "Replace this occurence?",
	mfYesNoCancel + mfInformation);
}

//handles application's events

void TVDemo::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand) switch (event.message.command)
    {
    case cmAbout:
	aboutBox();
	break;
    case cmCascade:
	cascade();
	break;
    case cmChDir:
	changeDir();
	break;
    case cmDosShell:
	shell();
	break;
    case cmNew:
	newFile();
	break;
    case cmOpen:
	openFile("*");
	break;
    case cmShowClip:
	showClipboard();
	break;
    case cmTile:
	tile();
	break;
    default:
	return;
    }
    clearEvent(event);
}

//returns true if and only if the view at address `p' is tileable and visible

static Boolean isTileable(TView *p, void *)
{
    if ((p->options & ofTileable) != 0 &&
	(p->state & sfVisible) != 0) return True;
    else return False;
}

//called when in idle state

void TVDemo::idle()
{
    TApplication::idle();
    if (deskTop->firstThat(isTileable, 0) != 0)
    {
	enableCommand(cmTile);
	enableCommand(cmCascade);
    }
    else
    {
	disableCommand(cmSave);
	disableCommand(cmSaveAs);
	disableCommand(cmUndo);
	disableCommand(cmCut);
	disableCommand(cmCopy);
	disableCommand(cmPaste);
	disableCommand(cmClear);
	disableCommand(cmFind);
	disableCommand(cmReplace);
	disableCommand(cmSearchAgain);
	disableCommand(cmTile);
	disableCommand(cmCascade);
    }
}

//creates a new menu bar

TMenuBar *TVDemo::initMenuBar(TRect r)
{
    TSubMenu& sub1 = *new TSubMenu("~\360~", 0, hcNoContext) +
    *new TMenuItem("~A~bout...", cmAbout, kbNoKey, hcNoContext);

    TSubMenu& sub2 = *new TSubMenu("~F~ile", 0, hcNoContext) +
    *new TMenuItem("~O~pen...", cmOpen, kbF3, hcNoContext, "F3") +
    *new TMenuItem("~N~ew", cmNew, kbNoKey, hcNoContext, "") +
    *new TMenuItem("~S~ave", cmSave, kbF2, hcNoContext, "F2") +
    *new TMenuItem("S~a~ve as...", cmSaveAs, kbNoKey, hcNoContext, "") +
    newLine() +
    *new TMenuItem("~C~hange dir...", cmChDir, kbNoKey, hcNoContext, "") +
    *new TMenuItem("~D~OS shell", cmDosShell, kbNoKey, hcNoContext, "") +
    *new TMenuItem("E~x~it", cmQuit, kbAltX, hcNoContext, "Alt-X");

    TSubMenu& sub3 = *new TSubMenu("~E~dit", 0, hcNoContext) +
    *new TMenuItem("~U~ndo", cmUndo, kbNoKey, hcNoContext, "") +
    newLine() +
    *new TMenuItem("Cu~t~", cmCut, kbShiftDel, hcNoContext, "Shift-Del") +
    *new TMenuItem("~C~opy", cmCopy, kbCtrlIns, hcNoContext, "Ctrl-Ins") +
    *new TMenuItem("~P~aste", cmPaste, kbShiftIns, hcNoContext, "Shift-Ins") +
    *new TMenuItem("~S~how clipboard", cmShowClip, kbNoKey, hcNoContext, "") +
    newLine() +
    *new TMenuItem("~C~lear", cmClear, kbCtrlDel, hcNoContext, "Ctrl-Del");

    TSubMenu& sub4 = *new TSubMenu("~S~earch", 0, hcNoContext) +
    *new TMenuItem("~F~ind...", cmFind, kbNoKey, hcNoContext) +
    *new TMenuItem("~R~eplace...", cmReplace, kbNoKey, hcNoContext) +
    *new TMenuItem("~S~earch again", cmSearchAgain, kbNoKey, hcNoContext);

    TSubMenu& sub5 = *new TSubMenu("~W~indows", 0, hcNoContext) +
    *new TMenuItem("~S~ize/move", cmResize, kbCtrlF5, hcNoContext, "Ctrl-F5") +
    *new TMenuItem("~Z~oom", cmZoom, kbF5, hcNoContext, "F5") +
    *new TMenuItem("~N~ext", cmNext, kbF6, hcNoContext, "F6") +
    *new TMenuItem("~P~revious", cmPrev, kbShiftF6, hcNoContext, "Shift-F6") +
    *new TMenuItem("~C~lose", cmClose, kbAltF3, hcNoContext, "Alt-F3") +
    *new TMenuItem("~T~ile", cmTile, kbNoKey, hcNoContext) +
    *new TMenuItem("C~a~scade", cmCascade, kbNoKey, hcNoContext);

    r.b.y =  r.a.y + 1;
    return new TMenuBar(r, sub1 + sub2 + sub3 + sub4 + sub5);
}

//creates a new status line

TStatusLine *TVDemo::initStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;
    return new TStatusLine(r,
    *new TStatusDef(0, 50) +
    *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
    *new TStatusItem("~F2~ Save", kbF2, cmSave) +
    *new TStatusItem("~F3~ Open", kbF3, cmOpen) +
    *new TStatusItem("~Alt-F3~ Close", kbAltF3, cmClose) +
    *new TStatusItem("~F5~ Zoom", kbF5, cmZoom) +
    *new TStatusItem("~F6~ Next", kbF6, cmNext) +
    *new TStatusItem("~F10~ Menu", kbF10, cmMenu) +
    *new TStatusItem("", kbCtrlF5, cmResize));
}

//Creates a new edit window, with no assigned file name.
//The window title will be `Untitled'.

void TVDemo::newFile()
{
    TView *w = validView(new TEditWindow(deskTop->getExtent(), 0,
	wnNoNumber));
    if (w != 0) deskTop->insert(w);
}

//opens an existing file, whose file name is passed as a parameter
//in `fileSpec'

void TVDemo::openFile(const char *fileSpec)
{
    TFileDialog *d = (TFileDialog *) validView(new TFileDialog(fileSpec,
	"Open a File", "~N~ame", fdOpenButton, 100));

    if (d != 0 && deskTop->execView(d) != cmCancel)
    {
	char fileName[PATH_MAX];

	d->getFileName(fileName);
	TView *w = validView(new TEditWindow(deskTop->getExtent(),
	    fileName, wnNoNumber));
	if (w != 0) deskTop->insert(w);
    }
    destroy(d);
}

void TVDemo::shell()
{
    raise(SIGTSTP); //this line simulates a Ctrl-Z
}

//Shows the clipboard. The user can't really destroy the clipboard: every
//cmClose command will simply move the clipboard window in the background,
//by hiding it.

void TVDemo::showClipboard()
{
    if (clipWindow != 0)
    {
	clipWindow->select();
	clipWindow->show();
    }
}

//moves all the windows in a tile-like fashion

void TVDemo::tile()
{
    deskTop->tile(deskTop->getExtent());
}

int main()
{
    TVDemo d;

    d.run();
    return 0;
}
