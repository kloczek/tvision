/*
 * TVision example: how to use range validators in input lines
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TApplication
#define Uses_TButton
#define Uses_TCombo
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TKeys
#define Uses_TLabel
#define Uses_TMemo
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TPXPictureValidator
#define Uses_TRangeValidator
#define Uses_TScrollBar
#define Uses_TSubMenu
#define Uses_TStaticText

#include <tv.h>

const int cmOpenDialog = 100;

class Demo:public TApplication {
      public:
	Demo();
	void handleEvent(TEvent & event);
	static TMenuBar *initMenuBar(TRect r);
};

class MyDialog:public TDialog {
      public:
	MyDialog();
};

Demo::Demo():TProgInit(&Demo::initStatusLine, &Demo::initMenuBar,
	  &Demo::initDeskTop)
{
}

void Demo::handleEvent(TEvent & event)
{
	TApplication::handleEvent(event);

	if (event.what == evCommand) {
		switch (event.message.command) {
		case cmOpenDialog:
			{
				MyDialog *d = new MyDialog;
				executeDialog(d);
			}
			break;
		default:
			return;
		}
		clearEvent(event);
	}
}

TMenuBar *Demo::initMenuBar(TRect r)
{
	r.b.y = r.a.y + 1;

	return new TMenuBar(r,
			    *new TSubMenu("~F~ile", kbAltF, hcNoContext) +
			    *new TMenuItem("~D~ialog...", cmOpenDialog, kbNoKey,
					   hcNoContext) + newLine() +
			    *new TMenuItem("E~x~it...", cmQuit, kbAltX,
					   hcNoContext)
	    );
}

MyDialog::MyDialog():
TWindowInit(&TDialog::initFrame),
TDialog(TRect(0, 0, 42, 16), "TValidator example")
{
	TInputLine *line;
	TView *obj;

	options |= ofCentered;

	insert(obj = new TInputLine(TRect(23, 1, 40, 2), 40));
	insert(new TLabel(TRect(1, 1, 22, 2), "No validator", obj));

	//first approach: create the validator and next attach it to the
	//input line

	insert(obj = new TInputLine(TRect(23, 3, 27, 4), 3,
				    new TRangeValidator(1, 31)));
	insert(new TLabel(TRect(1, 3, 22, 4), "Date style", obj));
	insert(new TStaticText(TRect(27, 3, 28, 4), "/"));

	//second approach: create the input line and then add the
	//validator

	insert(line = new TInputLine(TRect(28, 3, 32, 4), 3));
	line->setValidator(new TRangeValidator(1, 12));

	insert(new TStaticText(TRect(32, 3, 33, 4), "/"));
	insert(new TInputLine(TRect(33, 3, 39, 4), 5,
			      new TRangeValidator(1950, 2050)));

	insert(obj = new TInputLine(TRect(23, 5, 27, 6), 10,
				    new TPXPictureValidator("&&", False)));
	insert(new TLabel(TRect(1, 5, 22, 6), "Two letters", obj));

	insert(obj = new TInputLine(TRect(23, 7, 40, 8), 20,
				    new TPXPictureValidator("#####-###",
							    True)));
	insert(new TLabel(TRect(1, 7, 22, 8), "Fixed-length code", obj));

	insert(obj = new TInputLine(TRect(23, 9, 40, 10), 10,
				    new TPXPictureValidator("*#", False)));
	insert(new TLabel(TRect(1, 9, 22, 10), "Variable-length code", obj));

	insert(obj = new TInputLine(TRect(23, 11, 40, 12), 20,
				    new TPXPictureValidator("##/##/####",
							    True)));
	insert(new TLabel(TRect(1, 11, 22, 12), "Another date style", obj));

	insert(new TButton(TRect(1, 13, 11, 15), "O~K~", cmOK, bfDefault));
	insert(new TButton(TRect(12, 13, 24, 15), "~C~ancel", cmCancel,
			   bfNormal));
	selectNext(False);
}

int main()
{
	Demo a;
	a.run();
}
