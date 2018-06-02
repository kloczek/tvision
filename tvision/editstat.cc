/*
 * editstat.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TEditor
#define Uses_TEditorDialog
#include <tv.h>

ushort defEditorDialog( int, ... )
{
    return cmCancel;
}

TEditorDialog TEditor::editorDialog = defEditorDialog;
ushort TEditor::editorFlags = efBackupFiles | efPromptOnReplace;
char TEditor::findStr[maxFindStrLen] = "";
char TEditor::replaceStr[maxReplaceStrLen] = "";
TEditor * TEditor::clipboard = 0;
