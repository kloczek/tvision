/*
 * menu.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TKeys
#define Uses_TSubMenu
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TMenu
#include <tv.h>

#include <string.h>

TSubMenu::TSubMenu(const char *nm, ushort key, ushort helpCtx):TMenuItem(nm, 0,
									 key,
									 helpCtx)
{
}

TSubMenu & operator +(TSubMenu & s, TMenuItem & i)
{
	TSubMenu *sub = &s;
	while (sub->next != nullptr)
		sub = (TSubMenu *) (sub->next);

	if (sub->subMenu == nullptr)
		sub->subMenu = new TMenu(i);
	else {
		TMenuItem *cur = sub->subMenu->items;
		while (cur->next != nullptr)
			cur = cur->next;
		cur->next = &i;
	}
	return s;
}

TSubMenu & operator +(TSubMenu & s1, TSubMenu & s2)
{
	TMenuItem *cur = &s1;
	while (cur->next != nullptr)
		cur = cur->next;
	cur->next = &s2;
	return s1;
}

TStatusDef & operator +(TStatusDef & s1, TStatusItem & s2)
{
	TStatusDef *def = &s1;
	while (def->next != nullptr)
		def = def->next;
	if (def->items == nullptr)
		def->items = &s2;
	else {
		TStatusItem *cur = def->items;
		while (cur->next != nullptr)
			cur = cur->next;
		cur->next = &s2;
	}
	return s1;
}

TStatusDef & operator +(TStatusDef & s1, TStatusDef & s2)
{
	TStatusDef *cur = &s1;
	while (cur->next != nullptr)
		cur = cur->next;
	cur->next = &s2;
	return s1;
}
