/*
 * TVision example: how to use the TListBox class
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#include <stdio.h>
#include <stdlib.h>

#define Uses_TSortedListBox
#define Uses_TStringCollection
#define Uses_TListBox
#define Uses_TApplication
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TWindow
#define Uses_TScrollBar
#define Uses_TDialog
#define Uses_TButton
#define Uses_MsgBox

#include <tv.h>

const int cmNewDialog  = 200;

class TMyApp : public TApplication
{
public:
	TMyApp();
	static TStatusLine *initStatusLine( TRect r );
	static TMenuBar *initMenuBar( TRect r );
	virtual void handleEvent( TEvent& event);
	void newDialog();
};

TMyApp::TMyApp() :
    TProgInit( &TMyApp::initStatusLine,
               &TMyApp::initMenuBar,
               &TMyApp::initDeskTop
             )
{
}

struct TDemoDialogData
{
	TListBoxRec tlbr;
};

void TMyApp::newDialog()
{
	TDialog *pd = new TDialog( TRect( 20, 6, 60, 19), "Demo Dialog" );
	if( pd )
	{
		TScrollBar *sb = new TScrollBar( TRect( 21, 2, 22, 11 ) );
		pd->insert( sb );

		TListBox *lb = new TListBox( TRect( 2, 2, 20, 11 ), 2, sb );
//
// TSortedListBox has some more user-friendly stuff
//
//		TListBox *lb = new TSortedListBox( TRect( 2, 2, 20, 11 ), 2,
//			sb );
//
		pd->insert( lb );

		/* build a collection of strings */

		#define HOWMANY	20
		char *names[HOWMANY] =
		{
			"dog", "cat", "bird", "fish",
			"animal1", "animal2", "animal3", "animal4",
			"animal5", "animal6", "animal7", "animal8",
			"human1", "human2", "human3", "human4",
			"human5", "human6", "human7", "human8"
		};
		TStringCollection tsc(10, 5);
//
// TStringCollection sorts the strings by default
//
		for (int i = 0; i < HOWMANY; i++) tsc.insert(names[i]);

		/* add two buttons */

		pd->insert( new TButton( TRect( 28, 6, 38, 8 ), "~O~K", cmOK,
			bfDefault ));
		pd->insert( new TButton( TRect( 28, 10, 38, 12 ), "~C~ancel",
			cmCancel, bfNormal ));

		/* now run the dialog */

		TDemoDialogData d;

		d.tlbr.items = &tsc;	/* collection address */
		d.tlbr.selection = 2;	/* default selection */

		pd->setData( &d );

	        ushort control = deskTop->execView( pd );

		if( control != cmCancel ) pd->getData( &d );

		if (control == cmOK) messageBox(mfInformation | mfOKButton,
			"Your selection is %s", tsc.at(d.tlbr.selection));
	}
	destroy( pd );
}

void TMyApp::handleEvent(TEvent& event)
{
    TApplication::handleEvent(event);
    if( event.what == evCommand )
        {
        switch( event.message.command )
            {
            case cmNewDialog:
		newDialog();
                break;
            default:
                return;
            }
        clearEvent( event );            // clear event after handling
        }
}

TMenuBar *TMyApp::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;    // set bottom line 1 line below top line
    return new TMenuBar( r,
        *new TSubMenu( "~F~ile", kbAltF )+
            *new TMenuItem( "E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X" )+
        *new TSubMenu( "~W~indow", kbAltW )+
            *new TMenuItem( "~N~ext", cmNext,     kbF6, hcNoContext, "F6" )+
            *new TMenuItem( "~Z~oom", cmZoom,     kbF5, hcNoContext, "F5" )+
            *new TMenuItem( "~D~ialog", cmNewDialog, kbF2, hcNoContext, "F2" )
        );
}

TStatusLine *TMyApp::initStatusLine( TRect r )
{
    r.a.y = r.b.y - 1;     // move top to 1 line above bottom
    return new TStatusLine( r,
        *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( 0, kbF10, cmMenu ) +
            *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit )
        );
}

int main()
{
    TMyApp myApp;
    myApp.run();
    return 0;
}
