/*
 * TDirListBox.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TDirListBox
#define Uses_TEvent
#define Uses_TDirCollection
#define Uses_TChDirDialog
#define Uses_TDirEntry
#define Uses_TButton
#include <tv.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

TDirListBox::TDirListBox(const TRect & bounds,
			 TScrollBar *aScrollBar):TListBox(bounds, 1,
							  aScrollBar), cur(0)
{
	*dir = EOS;
}

TDirListBox::~TDirListBox()
{
	if (list())
		destroy(list());
}

void TDirListBox::getText(char *text, short item, short maxChars)
{
	strncpy(text, list()->at(item)->text(), maxChars);
	text[maxChars] = '\0';
}

void TDirListBox::selectItem(short item)
{
	message(owner, evCommand, cmChangeDir, list()->at(item));
}

/*
void TDirListBox::handleEvent( TEvent& event )
{
    if( event.what == evMouseDown && (event.mouse.eventFlags & meDoubleClick) )
        {
        event.what = evCommand;
        event.message.command = cmChangeDir;
        putEvent( event );
        clearEvent( event );
        }
    else
       TListBox::handleEvent( event );
}
*/

Boolean TDirListBox::isSelected(short item)
{
	return Boolean(item == cur);
}

void TDirListBox::showDrives(TDirCollection *)
{
	/* SS: do nothing */
}

void TDirListBox::showDirs(TDirCollection *dirs)
{
	/* SS: changed */

	char buf[PATH_MAX * 2];
	char *curDir = dir;
	char *end;
	char *name = buf + sizeof(buf) / 2;
	const int indentSize = 2;
	int indent = 0, len;

	/* extract directories from path string */

	memset(buf, ' ', sizeof(buf));
	strcpy(name, pathDir);
	len = strlen(pathDir);
	while ((end = strchr(curDir, '/')) != nullptr) {
		/* special case: root directory */

		if (end == dir)
			dirs->insert(new TDirEntry("/", ""));
		else {
			memcpy(name + len, curDir, end - curDir);
			name[len + end - curDir] = EOS;
			*end = EOS;
			dirs->insert(new TDirEntry(name - indent, dir));
			*end = '/';
			indent += indentSize;
		}
		curDir = end + 1;
	}
	cur = dirs->getCount() - 1;

	/* read subdirectories in the current directory */

	Boolean isFirst = True;
	DIR *dp;
	char path[PATH_MAX];
	dirent *de;
	struct stat s;

	sprintf(path, "%s.", dir);
	if ((dp = opendir(path)) != nullptr) {
		while ((de = readdir(dp)) != nullptr) {
			/* we don't want these directories */

			if (strcmp(de->d_name, ".") == 0 ||
			    strcmp(de->d_name, "..") == 0)
				continue;

			/* is it a directory ? */

			sprintf(path, "%s%s", dir, de->d_name);
			if (stat(path, &s) == 0 && S_ISDIR(s.st_mode)) {
				if (isFirst) {
					isFirst = False;
					strcpy(name, firstDir);
					len = strlen(firstDir);
				} else {
					strcpy(name, middleDir);
					len = strlen(middleDir);
				}
				strcpy(name + len, de->d_name);
				dirs->insert(new TDirEntry(name - indent,
							   path));
			}
		}
		closedir(dp);
	}

	/* old code */

	char *p = dirs->at(dirs->getCount() - 1)->text();
	char *i = strchr(p, graphics[0]);
	if (i == nullptr) {
		i = strchr(p, graphics[1]);
		if (i != nullptr)
			*i = graphics[0];
	} else {
		*(i + 1) = graphics[2];
		*(i + 2) = graphics[2];
	}
}

void TDirListBox::newDirectory(const char *str)
{
	/* SS: changed */

	strcpy(dir, str);
	TDirCollection *dirs = new TDirCollection(5, 5);
	showDirs(dirs);
	newList(dirs);
	focusItem(cur);
}

void TDirListBox::setState(ushort nState, Boolean enable)
{
	TListBox::setState(nState, enable);
	if ((nState & sfFocused) != 0)
#ifndef __UNPATCHED
		message(owner, evCommand, cmDirSelection, (void *)enable);	//!!
#else
		((TChDirDialog *) owner)->chDirButton->makeDefault(enable);
#endif
}

#if !defined(NO_STREAMABLE)

TStreamable *TDirListBox::build()
{
	return new TDirListBox(streamableInit);
}

#endif
