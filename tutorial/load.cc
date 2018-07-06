/*
 * TVision example: how to create custom views
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TParamText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu
#define Uses_TWindow

#include <tv.h>

#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
	cmNewLoad = 100,
	cmUpdateLoad
};

class TProgressBar:public TView {
	static const int colLeft = 0x4f;
	static const int colRight = 0x2f;
	int max;
	int value;
      public:
	TProgressBar(const TRect & r, int aMax, int aValue = 0);
	void draw();
	int getValue() {
		return value;
	}
	void setValue(int aValue);
};

TProgressBar::TProgressBar(const TRect & r, int aMax, int aValue):TView(r),
max(aMax), value(aValue)
{
	growMode |= gfGrowHiX;
}

void TProgressBar::draw()
{
	TDrawBuffer b;
	char str[maxViewWidth];
	int mid = (max == 0 || value >= max) ? size.x : size.x * value / max;

	sprintf(str, "%d%%", max == 0 ? 100 : 100 * value / max);
	b.moveChar(0, ' ', colLeft, mid);
	b.moveChar(mid, ' ', colRight, size.x - mid);
	b.moveStr((size.x - std::strlen(str)) / 2, str, 0);
	writeBuf(0, 0, size.x, size.y, b);
}

void TProgressBar::setValue(int aValue)
{
	if (aValue != value) {
		value = aValue;
		drawView();
	}
}

class TLoadWindow:public TWindow {
	static const int count = 3;
	time_t oldTime;
	TParamText *pTime;
	TProgressBar *bar[count];
      public:
	static const int minW = 28;
	static const int minH = 9;
	TLoadWindow(int x = 0, int y = 0);
	void handleEvent(TEvent & event);
	void sizeLimits(TPoint & min, TPoint & max);
	void update();
};

TLoadWindow::TLoadWindow(int x, int y):TWindowInit(&TLoadWindow::initFrame),
TWindow(TRect(x, y, x + minW, y + minH), "System load", wnNoNumber), oldTime(-1)
{
	options |= ofTileable;
	for (int i = 0; i < count; i++) {
		insert(bar[i] = new TProgressBar(TRect(9, i * 2 + 1, minW - 2,
						       i * 2 + 2), 100));
	}
	insert(pTime = new TParamText(TRect(2, count * 2 + 1, minW - 2,
					    count * 2 + 2)));
	insert(new TStaticText(TRect(1, 1, 9, 6),
			       "\003 1 min\n"
			       "\n" "\003 5 min\n" "\n" "\00315 min"));
	update();
}

void TLoadWindow::handleEvent(TEvent & event)
{
	TWindow::handleEvent(event);
	if (event.what == evBroadcast && event.message.command == cmUpdateLoad)
		update();
}

void TLoadWindow::update()
{
	time_t newTime = time(NULL);
	if (pTime != 0 && oldTime != newTime) {
		oldTime = newTime;
		pTime->setText(ctime(&oldTime));

		std::ifstream f("/proc/loadavg");
		for (int i = 0; f && i < count; i++) {
			double v;

			f >> v;
			if (bar[i] != 0)
				bar[i]->setValue((int)(v * 100));
		}
	}
}

void TLoadWindow::sizeLimits(TPoint & min, TPoint & max)
{
	TView::sizeLimits(min, max);
	min.x = minW;
	min.y = max.y = minH;
}

class myApp:public TApplication {
      public:
	myApp();
	static TMenuBar *initMenuBar(TRect r);
	static TStatusLine *initStatusLine(TRect r);
	void cascade();
	void handleEvent(TEvent & Event);
	void idle();
	void newLoadWindow();
	void tile();
};

myApp::myApp():TProgInit(&myApp::initStatusLine, &myApp::initMenuBar,
	  &myApp::initDeskTop)
{
	srand(time(NULL));
}

void myApp::cascade()
{
	deskTop->cascade(deskTop->getExtent());
}

void myApp::handleEvent(TEvent & event)
{
	TApplication::handleEvent(event);
	if (event.what == evCommand) {
		switch (event.message.command) {
		case cmCascade:
			cascade();
			break;
		case cmNewLoad:
			newLoadWindow();
			break;
		case cmTile:
			tile();
			break;
		default:
			return;
		}
		clearEvent(event);
	}
}

void myApp::newLoadWindow()
{
	TView *w = validView(new TLoadWindow);
	if (w != 0) {
		w->moveTo((int)((double)(size.x - TLoadWindow::minW) *
				rand() / (RAND_MAX + 1.0)),
			  (int)((double)(size.y - TLoadWindow::minH) *
				rand() / (RAND_MAX + 1.0)));
		deskTop->insert(w);
	}
}

void myApp::tile()
{
	deskTop->tile(deskTop->getExtent());
}

static Boolean isTileable(TView * p, void *)
{
	if ((p->options & ofTileable) != 0)
		return True;
	else
		return False;
}

void myApp::idle()
{
	TApplication::idle();
	if (deskTop->firstThat(isTileable, 0) != 0) {
		enableCommand(cmTile);
		enableCommand(cmCascade);
	} else {
		disableCommand(cmTile);
		disableCommand(cmCascade);
	}
	message(deskTop, evBroadcast, cmUpdateLoad, 0);
}

TMenuBar *myApp::initMenuBar(TRect r)
{
	TSubMenu & sub1 = *new TSubMenu("~F~ile", 0) +
	    *new TMenuItem("System ~l~oad", cmNewLoad, kbAltL, hcNoContext,
			   "Alt-L") + newLine() + *new TMenuItem("E~x~it",
								 cmQuit, kbAltX,
								 hcNoContext,
								 "Alt-X");
	TSubMenu & sub2 =
	    *new TSubMenu("~W~indows", 0) + *new TMenuItem("~R~esize/move",
							   cmResize, kbCtrlF5,
							   hcNoContext,
							   "Ctrl-F5") +
	    *new TMenuItem("~Z~oom", cmZoom, kbF5, hcNoContext,
			   "F5") + *new TMenuItem("~N~ext", cmNext, kbF6,
						  hcNoContext,
						  "F6") +
	    *new TMenuItem("~C~lose", cmClose, kbAltF3, hcNoContext,
			   "Alt-F3") + *new TMenuItem("~T~ile", cmTile,
						      kbNoKey) +
	    *new TMenuItem("C~a~scade", cmCascade, kbNoKey);
	r.b.y = r.a.y + 1;
	return new TMenuBar(r, sub1 + sub2);
}

TStatusLine *myApp::initStatusLine(TRect r)
{
	r.a.y = r.b.y - 1;
	return new TStatusLine(r,
			       *new TStatusDef(0, 50) +
			       *new TStatusItem("~Alt-X~ Exit", kbAltX,
						cmQuit) +
			       *new TStatusItem("~Alt-L~ system load", kbAltL,
						cmNewLoad) + *new TStatusItem(0,
									      kbAltF3,
									      cmClose)
			       + *new TStatusItem(0, kbF10,
						  cmMenu) + *new TStatusItem(0,
									     kbF5,
									     cmZoom)
			       + *new TStatusItem(0, kbCtrlF5, cmResize));
}

int main()
{
	myApp d;

	d.run();
	return 0;
}
