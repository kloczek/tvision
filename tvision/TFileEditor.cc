/*
 * TFileEditor.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TProgram
#define Uses_TGroup
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#include <fstream>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline ushort min(ushort u1, ushort u2)
{
	return u1 < u2 ? u1 : u2;
}

inline uint min(uint u1, uint u2)
{
	return u1 < u2 ? u1 : u2;
}

TFileEditor::TFileEditor(const TRect & bounds,
			 TScrollBar *aHScrollBar,
			 TScrollBar *aVScrollBar,
			 TIndicator *aIndicator,
			 const char *aFileName):TEditor(bounds, aHScrollBar,
							aVScrollBar, aIndicator,
							0)
{
	if (aFileName == nullptr)
		fileName[0] = EOS;
	else {
		strcpy(fileName, aFileName);
		fexpand(fileName);
		if (isValid)
			isValid = loadFile();
	}
}

void TFileEditor::doneBuffer()
{
	delete buffer;
}

void TFileEditor::handleEvent(TEvent & event)
{
	TEditor::handleEvent(event);
	switch (event.what) {
	case evCommand:
		switch (event.message.command) {
		case cmSave:
			save();
			break;
		case cmSaveAs:
			saveAs();
			break;
		default:
			return;
		}
		break;
	default:
		return;
	}
	clearEvent(event);
}

void TFileEditor::initBuffer()
{
	buffer = new char[bufSize];
}

Boolean TFileEditor::loadFile()
{
	std::ifstream f(fileName, std::ios::in | std::ios::binary);
	if (!f) {
		setBufLen(0);
		return True;
	} else {
		f.seekg(0, std::ios::end);
		uint fSize = f.tellg();
		f.seekg(0, std::ios::beg);
		if (setBufSize(fSize) == False) {
			editorDialog(edOutOfMemory);
			return False;
		}
		f.read(&buffer[bufSize - fSize], fSize);
		if (!f) {
			editorDialog(edReadError, fileName);
			return False;
		} else {
			setBufLen(fSize);
			return True;
		}
	}
}

Boolean TFileEditor::save()
{
	if (*fileName == EOS)
		return saveAs();
	else
		return saveFile();
}

Boolean TFileEditor::saveAs()
{
	Boolean res = False;
	if (editorDialog(edSaveAs, fileName) != cmCancel) {
		fexpand(fileName);
		message(owner, evBroadcast, cmUpdateTitle, nullptr);
		res = saveFile();
		if (isClipboard() == True)
			*fileName = EOS;
	}
	return res;
}

static void writeBlock(std::ofstream & f, char *buf, uint len)
{
	while (len > 0) {
		int l = len < INT_MAX ? len : INT_MAX;
		f.write(buf, l);
		buf += l;
		len -= l;
	}
}

Boolean TFileEditor::saveFile()
{
	if ((editorFlags & efBackupFiles) != 0) {
		/* SS: little change */

		char backupName[PATH_MAX];
		sprintf(backupName, "%s%s", fileName, backupExt);
		rename(fileName, backupName);
	}

	std::ofstream f(fileName, std::ios::out | std::ios::binary);

	if (!f) {
		editorDialog(edCreateError, fileName);
		return False;
	} else {
		writeBlock(f, buffer, curPtr);
		writeBlock(f, buffer + curPtr + gapLen, bufLen - curPtr);

		if (!f) {
			editorDialog(edWriteError, fileName);
			return False;
		} else {
			modified = False;
			update(ufUpdate);
		}
	}
	return True;
}

Boolean TFileEditor::setBufSize(uint newSize)
{
	if (newSize == 0)
		newSize = 0x1000;
	else
		newSize = (newSize + 0x0FFF) & 0xFFFFF000;
	if (newSize != bufSize) {
		char *temp = buffer;
		/* Bypass safety pool to allocate buffer, but check for possible
		   nullptr return value. */
		if ((buffer = (char *)malloc(newSize)) == nullptr) {
			delete temp;
			return False;
		}
		ulong n = bufLen - curPtr + delCount;
		memcpy(buffer, temp, min(newSize, bufSize));
		memmove(&buffer[newSize - n], &temp[bufSize - n], n);
		delete temp;
		bufSize = newSize;
		gapLen = bufSize - bufLen;
	}
	return True;
}

void TFileEditor::shutDown()
{
	setCmdState(cmSave, False);
	setCmdState(cmSaveAs, False);
	TEditor::shutDown();
}

void TFileEditor::updateCommands()
{
	TEditor::updateCommands();
	setCmdState(cmSave, True);
	setCmdState(cmSaveAs, True);
}

Boolean TFileEditor::valid(ushort command)
{
	if (command == cmValid)
		return isValid;
	else {
		if (modified == True) {
			int d;
			if (*fileName == EOS)
				d = edSaveUntitled;
			else
				d = edSaveModify;

			switch (editorDialog(d, fileName)) {
			case cmYes:
				return save();
			case cmNo:
				modified = False;
				return True;
			case cmCancel:
				return False;
			}
		}
	}
	return True;
}

#if !defined(NO_STREAMABLE)

void TFileEditor::write(opstream & os)
{
	TEditor::write(os);
	os.writeString(fileName);
	os << selStart << selEnd << curPtr;
}

void *TFileEditor::read(ipstream & is)
{
	TEditor::read(is);
	bufSize = 0;
	is.readString(fileName, sizeof(fileName));
	if (isValid) {
		isValid = loadFile();
		ulong sStart, sEnd, curs;
		is >> sStart >> sEnd >> curs;
		if (isValid && sEnd <= bufLen) {
			setSelect(sStart, sEnd, Boolean(curs == sStart));
			trackCursor(True);
		}
	}
	return this;
}

TStreamable *TFileEditor::build()
{
	return new TFileEditor(streamableInit);
}

TFileEditor::TFileEditor(StreamableInit):TEditor(streamableInit)
{
}

#endif
