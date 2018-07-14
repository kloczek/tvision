/*-------------------------------------------------------*/
/*                                                       */
/*   Calendar.h: Header file for Calendar.cpp            */
/*                                                       */
/*-------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( TV_INC_CALENDAR_H )
#define TV_INC_CALENDAR_H

int dayOfWeek(int day, int month, int year);

class TCalendarView:public TView {
      public:
	TCalendarView(TRect & r);
	TCalendarView(StreamableInit):TView(streamableInit) {
	}
	virtual void handleEvent(TEvent & event);
	virtual void draw();

      private:
	unsigned days, month, year;
	unsigned curDay, curMonth, curYear;

	virtual const char *streamableName() const {
		return name;
      } protected:
	virtual void write(opstream &);
	virtual void *read(ipstream &);

      public:
	static const char *const name;
	static TStreamable *build();
};

inline ipstream & operator >>(ipstream & is, TCalendarView & cl)
{
	return is >> (TStreamable &) cl;
}

inline ipstream & operator >>(ipstream & is, TCalendarView * &cl)
{
	return is >> (void *&)cl;
}

inline opstream & operator <<(opstream & os, TCalendarView & cl)
{
	return os << (TStreamable &) cl;
}

inline opstream & operator <<(opstream & os, TCalendarView * cl)
{
	return os << (TStreamable *) cl;
}

class TCalendarWindow:public TWindow {
      public:
	TCalendarWindow();
	TCalendarWindow(StreamableInit):TWindowInit(&TCalendarWindow::
						    initFrame),
	    TWindow(streamableInit) {
	}

      private:
	virtual const char *streamableName() const {
		return name;
      } protected:
	virtual void write(opstream &);
	virtual void *read(ipstream &);

      public:
	static const char *const name;
	static TStreamable *build();
};

inline ipstream & operator >>(ipstream & is, TCalendarWindow & cl)
{
	return is >> (TStreamable &) cl;
}

inline ipstream & operator >>(ipstream & is, TCalendarWindow * &cl)
{
	return is >> (void *&)cl;
}

inline opstream & operator <<(opstream & os, TCalendarWindow & cl)
{
	return os << (TStreamable &) cl;
}

inline opstream & operator <<(opstream & os, TCalendarWindow * cl)
{
	return os << (TStreamable *) cl;
}

#endif // TV_INC_CALENDAR_H
