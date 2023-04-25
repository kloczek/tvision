/*
 * TGroup.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TScreen
#define Uses_TGroup
#define Uses_TView
#define Uses_TRect
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#define Uses_TVMemMgr
#include <tv.h>

TView *TheTopView = nullptr;
TGroup *ownerGroup = nullptr;

TGroup::TGroup(const TRect & bounds):TView(bounds),
current(nullptr),
last(nullptr), phase(phFocused), buffer(nullptr), lockFlag(0), endState(0)
{
	options |= ofSelectable | ofBuffered;
	clip = getExtent();
	eventMask = 0xFFFF;
}

TGroup::~TGroup()
{
}

void TGroup::shutDown()
{
	TView *p = last;
	if (p != nullptr) {
		do {
			p->hide();
			p = p->prev();
		} while (p != last);

		do {
			TView *T = p->prev();
			destroy(p);
			p = T;
		} while (last != nullptr);
	}
	freeBuffer();
	current = nullptr;
	TView::shutDown();
}

void doCalcChange(TView *p, void *d)
{
	TRect r;
	static_cast < TGroup * >(p)->calcBounds(r,
						*static_cast < TPoint * >(d));
	static_cast < TGroup * >(p)->changeBounds(r);
}

static void doAwaken(TView *v, void *)
{
	v->awaken();
}

void TGroup::awaken()
{
	forEach(doAwaken, nullptr);
}

void TGroup::changeBounds(const TRect & bounds)
{
	TPoint d;

	d.x = (bounds.b.x - bounds.a.x) - size.x;
	d.y = (bounds.b.y - bounds.a.y) - size.y;
	if (d.x == 0 && d.y == 0) {
		setBounds(bounds);
		drawView();
	} else {
		freeBuffer();
		setBounds(bounds);
		clip = getExtent();
		getBuffer();
		lock();
		forEach(doCalcChange, &d);
		unlock();
	}
}

void addSubviewDataSize(TView *p, void *T)
{
	*static_cast < ushort * >(T) += static_cast < TGroup * >(p)->dataSize();
}

ushort TGroup::dataSize()
{
	ushort T = 0;
	forEach(addSubviewDataSize, &T);
	return T;
}

void TGroup::remove(TView *p)
{
	if (p) {
		ushort saveState;
		saveState = p->state;
		p->hide();
		removeView(p);
		p->owner = nullptr;
		p->next = nullptr;
		if ((saveState & sfVisible) != 0)
			p->show();
	}
}

void TGroup::draw()
{
	if (buffer == nullptr) {
		getBuffer();
		if (buffer != nullptr) {
			lockFlag++;
			redraw();
			lockFlag--;
		}
	}
	if (buffer != nullptr)
		writeBuf(0, 0, size.x, size.y, buffer);
	else {
		clip = getClipRect();
		redraw();
		clip = getExtent();
	}
}

void TGroup::drawSubViews(TView *p, TView *bottom)
{
	while (p != bottom) {
		p->drawView();
		p = p->nextView();
	}
}

void TGroup::endModal(ushort command)
{
	if ((state & sfModal) != 0)
		endState = command;
	else
		TView::endModal(command);
}

void TGroup::eventError(TEvent & event)
{
	if (owner != nullptr)
		owner->eventError(event);
}

ushort TGroup::execute()
{
	do {
		endState = 0;
		do {
			TEvent e;
			getEvent(e);
			handleEvent(e);
			if (e.what != evNothing)
				eventError(e);
		} while (endState == 0);
	} while (!valid(endState));
	return endState;
}

ushort TGroup::execView(TView *p)
{
	if (p == nullptr)
		return cmCancel;

	ushort saveOptions = p->options;
	TGroup *saveOwner = p->owner;
	TView *saveTopView = TheTopView;
	TView *saveCurrent = current;
	TCommandSet saveCommands;
	getCommands(saveCommands);
	TheTopView = p;
	p->options = p->options & ~ofSelectable;
	p->setState(sfModal, True);
	setCurrent(p, enterSelect);
	if (saveOwner == nullptr)
		insert(p);
	ushort retval = p->execute();
	if (saveOwner == nullptr)
		remove(p);
	setCurrent(saveCurrent, leaveSelect);
	p->setState(sfModal, False);
	p->options = saveOptions;
	TheTopView = saveTopView;
	setCommands(saveCommands);
	return retval;
}

TView *TGroup::first()
{
	if (last == nullptr)
		return nullptr;
	else
		return last->next;
}

TView *TGroup::findNext(Boolean forwards)
{
	TView *p, *result;

	result = nullptr;
	if (current) {
		p = current;
		do {
			if (forwards)
				p = p->next;
			else
				p = p->prev();

		} while (!
			 ((((p->state & (sfVisible | sfDisabled)) == sfVisible)
			   && (p->options & ofSelectable)) || (p == current)));

		if (p != current)
			result = p;
	}
	return result;
}

Boolean TGroup::focusNext(Boolean forwards)
{
	TView *p;

	p = findNext(forwards);
	if (p)
		return p->focus();
	else
		return True;
}

TView *TGroup::firstMatch(ushort aState, ushort aOptions)
{
	if (last == nullptr)
		return nullptr;

	TView *temp = last;
	while (1) {
		if (((temp->state & aState) == aState) &&
		    ((temp->options & aOptions) == aOptions))
			return temp;

		temp = temp->next;
		if (temp == last)
			return nullptr;
	}
}

void TGroup::freeBuffer()
{
	if ((options & ofBuffered) != 0 && buffer != nullptr) {
		delete buffer;
		buffer = nullptr;
	}
}

void TGroup::getBuffer()
{
	if ((state & sfExposed) != 0)
		if ((options & ofBuffered) != 0 && (buffer == nullptr))
			buffer = new ushort[size.x * size.y];
}

void TGroup::getData(void *rec)
{
	ushort i = 0;
	if (last != nullptr) {
		TView *v = last;
		do {
			v->getData(static_cast < char *>(rec) + i);
			i += v->dataSize();
			v = v->prev();
		} while (v != last);
	}
}

struct handleStruct {
	handleStruct(TEvent & e, TGroup & g):event(e), grp(g) {
	} TEvent & event;
	TGroup & grp;
};

static void doHandleEvent(TView *p, void *s)
{
	handleStruct *ptr = static_cast < handleStruct * >(s);

	if (p == nullptr ||
	    ((p->state & sfDisabled) != 0 &&
	     (ptr->event.what & (positionalEvents | focusedEvents)) != 0)
	    )
		return;

	switch (ptr->grp.phase) {
	case TView::phPreProcess:
		if ((p->options & ofPreProcess) == 0)
			return;
		break;
	case TView::phPostProcess:
		if ((p->options & ofPostProcess) == 0)
			return;
		break;
	default:
		break;
	}
	if ((ptr->event.what & p->eventMask) != 0)
		p->handleEvent(ptr->event);
}

static Boolean hasMouse(TView *p, void *s)
{
	return p->containsMouse(*static_cast < TEvent * >(s));
}

void TGroup::handleEvent(TEvent & event)
{
	TView::handleEvent(event);

	handleStruct hs(event, *this);

	if ((event.what & focusedEvents) != 0) {
		phase = phPreProcess;
		forEach(doHandleEvent, &hs);

		phase = phFocused;
		doHandleEvent(current, &hs);

		phase = phPostProcess;
		forEach(doHandleEvent, &hs);
	} else {
		phase = phFocused;
		if ((event.what & positionalEvents) != 0) {
#ifndef __UNPATCHED
			// get pointer to topmost view holding mouse
			TView *p = firstThat(hasMouse, &event);
			if (p)
				// we have a view; send event to it
				doHandleEvent(p, &hs);
			else if (event.what == evMouseDown)
				TScreen::makeBeep();
#else
			doHandleEvent(firstThat(hasMouse, &event), &hs);
#endif
		} else
			forEach(doHandleEvent, &hs);
	}
}

void TGroup::insert(TView *p)
{
	insertBefore(p, first());
}

void TGroup::insertBefore(TView *p, TView *Target)
{
	if (p != nullptr && p->owner == nullptr
	    && (Target == nullptr || Target->owner == this)) {
		if ((p->options & ofCenterX) != 0)
			p->origin.x = (size.x - p->size.x) / 2;
		if ((p->options & ofCenterY) != 0)
			p->origin.y = (size.y - p->size.y) / 2;
		ushort saveState = p->state;
		p->hide();
		insertView(p, Target);
		if ((saveState & sfVisible) != 0)
			p->show();
		if ((saveState & sfActive) != 0)
			p->setState(sfActive, True);
	}
}

void TGroup::insertView(TView *p, TView *Target)
{
	p->owner = this;
	if (Target != nullptr) {
		Target = Target->prev();
		p->next = Target->next;
		Target->next = p;
	} else {
		if (last == nullptr)
			p->next = p;
		else {
			p->next = last->next;
			last->next = p;
		}
		last = p;
	}
}

void TGroup::lock()
{
	if (buffer != nullptr || lockFlag != 0)
		lockFlag++;
}

void TGroup::redraw()
{
	drawSubViews(first(), nullptr);
}

void TGroup::resetCurrent()
{
	setCurrent(firstMatch(sfVisible, ofSelectable), normalSelect);
}

void TGroup::resetCursor()
{
	if (current != nullptr)
		current->resetCursor();
}

void TGroup::selectNext(Boolean forwards)
{
	if (current != nullptr) {
		TView *p = findNext(forwards);
		if (p)
			p->select();
	}
}

void TGroup::selectView(TView *p, Boolean enable)
{
	if (p != nullptr)
		p->setState(sfSelected, enable);
}

void TGroup::focusView(TView *p, Boolean enable)
{
	if ((state & sfFocused) != 0 && p != nullptr)
		p->setState(sfFocused, enable);
}

void TGroup::setCurrent(TView *p, selectMode mode)
{
	if (current != p) {
		lock();
		focusView(current, False);
		if (mode != enterSelect)
			if (current != nullptr)
				current->setState(sfSelected, False);
		if (mode != leaveSelect)
			if (p != nullptr)
				p->setState(sfSelected, True);
		if ((state & sfFocused) != 0 && p != nullptr)
			p->setState(sfFocused, True);
		current = p;
		unlock();
	}
}

void TGroup::setData(void *rec)
{
	ushort i = 0;
	if (last != nullptr) {
		TView *v = last;
		do {
			v->setData(static_cast < char *>(rec) + i);
			i += v->dataSize();
			v = v->prev();
		} while (v != last);
	}
}

static void doExpose(TView *p, void *enable)
{
	if ((p->state & sfVisible) != 0)
		p->setState(sfExposed, *static_cast < Boolean * >(enable));
}

struct setBlock {
	ushort st;
	Boolean en;
};

static void doSetState(TView *p, void *b)
{
	p->setState(((setBlock *) b)->st, ((setBlock *) b)->en);
}

void TGroup::setState(ushort aState, Boolean enable)
{
	setBlock sb;
	sb.st = aState;
	sb.en = enable;

	TView::setState(aState, enable);

	if ((aState & (sfActive | sfDragging)) != 0) {
		lock();
		forEach(doSetState, &sb);
		unlock();
	}

	if ((aState & sfFocused) != 0) {
		if (current != nullptr)
			current->setState(sfFocused, enable);
	}

	if ((aState & sfExposed) != 0) {
		forEach(doExpose, &enable);
		if (enable == False)
			freeBuffer();
	}
}

void TGroup::unlock()
{
	if (lockFlag != 0 && --lockFlag == 0)
		drawView();
}

Boolean isInvalid(TView *p, void *command)
{
	return Boolean(!p->valid(*(ushort *) command));
}

Boolean TGroup::valid(ushort command)
{
	if (command == cmReleasedFocus) {
		if (current && (current->options & ofValidate))
			return current->valid(command);
		else
			return True;
	}

	return Boolean(firstThat(isInvalid, &command) == nullptr);
}

ushort TGroup::getHelpCtx()
{
	ushort h = hcNoContext;
	if (current != nullptr)
		h = current->getHelpCtx();
	if (h == hcNoContext)
		h = TView::getHelpCtx();
	return h;
}

#if !defined(NO_STREAMABLE)

static void doPut(TView *p, void *osp)
{
	*static_cast < opstream * >(osp) << p;
}

void TGroup::write(opstream & os)
{
	ushort index;

	TView::write(os);
	TGroup *ownerSave = owner;
	owner = this;
	int count = indexOf(last);
	os << count;
	forEach(doPut, &os);
	if (current == nullptr)
		index = 0;
	else
		index = indexOf(current);
	os << index;
	owner = ownerSave;
}

void *TGroup::read(ipstream & is)
{
	ushort index;

	TView::read(is);
	clip = getExtent();
	TGroup *ownerSave = owner;
	owner = this;
	last = nullptr;
	phase = TView::phFocused;
	current = nullptr;
	buffer = nullptr;
	lockFlag = 0;
	endState = 0;
	int count;
	is >> count;
	TView *tv;
	for (int i = 0; i < count; i++) {
		is >> tv;
		if (tv != nullptr)
			insertView(tv, nullptr);
	}
	owner = ownerSave;
	TView *current;
	is >> index;
	current = at(index);
	setCurrent(current, TView::normalSelect);
	if (ownerGroup == nullptr)
		awaken();
	return this;
}

TStreamable *TGroup::build()
{
	return new TGroup(streamableInit);
}

TGroup::TGroup(StreamableInit):TView(streamableInit)
{
}

#endif
