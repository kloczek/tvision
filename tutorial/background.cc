/*
 * TVision example: how to change the background pattern
 *
 * Written by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TApplication
#define Uses_TDeskTop

#include <tv.h>

class Demo: public TApplication
{
public:
	Demo();
};

Demo::Demo(): TProgInit(&Demo::initStatusLine, &Demo::initMenuBar,
	&Demo::initDeskTop)
{
}

int main()
{
	TDeskTop::defaultBkgrnd = '?';
	Demo a;
	a.run();
}
