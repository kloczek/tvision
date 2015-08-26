/*
 * TVision example: how to show a dialog box at startup
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TApplication
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TSubMenu
#define Uses_TStaticText

#include <tvision/tv.h>

const int cmAboutCmd    = 100;

class Demo: public TApplication
{
public:
	Demo();
	void aboutDlgBox();
	void handleEvent(TEvent &event);
	static TMenuBar *initMenuBar(TRect r);
};

Demo::Demo(): TProgInit(&Demo::initStatusLine, &Demo::initMenuBar,
	&Demo::initDeskTop)
{
	aboutDlgBox();
}

void Demo::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);

    if (event.what == evCommand)
    {
        switch (event.message.command)
            {
            case cmAboutCmd:            //  About Dialog Box
                aboutDlgBox();
            default:                    //  Unknown command
                return;
            }
        clearEvent (event);
    }
}

TMenuBar *Demo::initMenuBar(TRect r)
{
    r.b.y =  r.a.y + 1;

    return new TMenuBar(r,
	*new TSubMenu( "~F~ile", kbAltF, hcNoContext ) +
	    *new TMenuItem( "~A~bout...", cmAboutCmd, kbNoKey, hcNoContext) +
	    newLine() +
	    *new TMenuItem( "E~x~it...", cmQuit, kbAltX, hcNoContext)
    );
}

void Demo::aboutDlgBox()
{
    TDialog *aboutBox = new TDialog(TRect(0, 0, 39, 13), "About");

    aboutBox->insert(
      new TStaticText(TRect(9, 2, 30, 9),
        "\003Turbo Vision Demo\n\n"       // These strings will be
        "\003C++ Version\n\n"             // concatenated by the compiler.
        "\003Copyright (c) 1994\n\n"      // The \003 centers the line.
        "\003by me"
        )
      );

    aboutBox->insert(
      new TButton(TRect(14, 10, 26, 12), " OK", cmOK, bfDefault)
      );

    aboutBox->options |= ofCentered;

    executeDialog(aboutBox);
}

int main()
{
	Demo a;
	a.run();
}
