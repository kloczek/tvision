/*
 * sall.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * This file written by J”rn Sierwald
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TBackground
#define Uses_TButton
#define Uses_TChDirDialog
#define Uses_TCheckBoxes
#define Uses_TCluster
#define Uses_TColorDialog
#define Uses_TColorDisplay
#define Uses_TColorGroupList
#define Uses_TColorItemList
#define Uses_TColorSelector
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TDirCollection
#define Uses_TDirEntry
#define Uses_TDirListBox
#define Uses_TEditor
#define Uses_TEditWindow
#define Uses_TFileCollection
#define Uses_TFileDialog
#define Uses_TFileEditor
#define Uses_TFileInfoPane
#define Uses_TFileInputLine
#define Uses_TFileList
#define Uses_TFilterValidator
#define Uses_TFrame
#define Uses_TGroup
#define Uses_THistory
#define Uses_TIndicator
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_TListBox
#define Uses_TListViewer
#define Uses_TLookupValidator
#define Uses_TMemo
#define Uses_TMenuBar
#define Uses_TMenuBox
#define Uses_TMenuPopup
#define Uses_TMenuView
#define Uses_TMonoSelector
#define Uses_TMultiCheckBoxes
#define Uses_TOutline
#define Uses_TPXPictureValidator
#define Uses_TParamText
#define Uses_TRadioButtons
#define Uses_TRangeValidator
#define Uses_TResourceCollection
#define Uses_TScrollBar
#define Uses_TScroller
#define Uses_TStaticText
#define Uses_TStatusLine
#define Uses_TStreamableClass
#define Uses_TStringCollection
#define Uses_TStringList
#define Uses_TStringLookupValidator
#define Uses_TValidator
#define Uses_TView
#define Uses_TWindow
#include <tv.h>

__link(RButton)
    __link(RCluster)
    __link(RDialog)
    __link(RDirListBox)
    __link(RFileInputLine)
    __link(RFileList)
    __link(RFrame)
    __link(RGroup)
    __link(RInputLine)
    __link(RLabel)
    __link(RListBox)
    __link(RListViewer)
    __link(RSortedListBox)
    __link(RScrollBar)
    __link(RStaticText)
    __link(RStringCollection)
    __link(RView)
    __link(RWindow)
TStreamableClass RBackground(TBackground::name,
			     TBackground::build, __DELTA(TBackground)
    );

TStreamableClass RButton(TButton::name, TButton::build, __DELTA(TButton)
    );

TStreamableClass RChDirDialog(TChDirDialog::name,
			      TChDirDialog::build, __DELTA(TChDirDialog)
    );

TStreamableClass RCheckBoxes(TCheckBoxes::name,
			     TCheckBoxes::build, __DELTA(TCheckBoxes)
    );

TStreamableClass RColorSelector(TColorSelector::name,
				TColorSelector::build, __DELTA(TColorSelector)
    );

TStreamableClass RMonoSelector(TMonoSelector::name,
			       TMonoSelector::build, __DELTA(TMonoSelector)
    );

TStreamableClass RColorDisplay(TColorDisplay::name,
			       TColorDisplay::build, __DELTA(TColorDisplay)
    );

TStreamableClass RColorGroupList(TColorGroupList::name,
				 TColorGroupList::build,
				 __DELTA(TColorGroupList)
    );

TStreamableClass RColorItemList(TColorItemList::name,
				TColorItemList::build, __DELTA(TColorItemList)
    );

TStreamableClass RColorDialog(TColorDialog::name,
			      TColorDialog::build, __DELTA(TColorDialog)
    );

TStreamableClass RCluster(TCluster::name, TCluster::build, __DELTA(TCluster)
    );

TStreamableClass RDeskTop(TDeskTop::name, TDeskTop::build, __DELTA(TDeskTop)
    );

TStreamableClass RDialog(TDialog::name, TDialog::build, __DELTA(TDialog)
    );

TStreamableClass RDirCollection(TDirCollection::name,
				TDirCollection::build, __DELTA(TDirCollection)
    );

TStreamableClass RDirListBox(TDirListBox::name,
			     TDirListBox::build, __DELTA(TDirListBox)
    );

TStreamableClass RIndicator(TIndicator::name,
			    TIndicator::build, __DELTA(TIndicator)
    );

TStreamableClass REditor(TEditor::name, TEditor::build, __DELTA(TEditor)
    );

TStreamableClass RMemo(TMemo::name, TMemo::build, __DELTA(TMemo)
    );

TStreamableClass RFileEditor(TFileEditor::name,
			     TFileEditor::build, __DELTA(TFileEditor)
    );

TStreamableClass REditWindow(TEditWindow::name,
			     TEditWindow::build, __DELTA(TEditWindow)
    );

TStreamableClass RFileCollection(TFileCollection::name,
				 TFileCollection::build,
				 __DELTA(TFileCollection)
    );

TStreamableClass RFileDialog(TFileDialog::name,
			     TFileDialog::build, __DELTA(TFileDialog)
    );

TStreamableClass RFileList(TFileList::name, TFileList::build, __DELTA(TFileList)
    );

TStreamableClass RSortedListBox(TSortedListBox::name,
				TSortedListBox::build, __DELTA(TSortedListBox)
    );

TStreamableClass RFileInfoPane(TFileInfoPane::name,
			       TFileInfoPane::build, __DELTA(TFileInfoPane)
    );

TFileInputLine::TFileInputLine(StreamableInit):
TInputLine(streamableInit)
{
}

TStreamable *TFileInputLine::build()
{
	return new TFileInputLine(streamableInit);
}

TStreamableClass RFileInputLine(TFileInputLine::name,
				TFileInputLine::build, __DELTA(TFileInputLine)
    );

TStreamableClass RFrame(TFrame::name, TFrame::build, __DELTA(TFrame)
    );

TStreamableClass RGroup(TGroup::name, TGroup::build, __DELTA(TGroup)
    );

TStreamableClass RHistory(THistory::name, THistory::build, __DELTA(THistory)
    );

TStreamableClass RInputLine(TInputLine::name,
			    TInputLine::build, __DELTA(TInputLine)
    );

TStreamableClass RLabel(TLabel::name, TLabel::build, __DELTA(TLabel)
    );

TStreamableClass RListBox(TListBox::name, TListBox::build, __DELTA(TListBox)
    );

TStreamableClass RListViewer(TListViewer::name,
			     TListViewer::build, __DELTA(TListViewer)
    );

TStreamableClass RMenuBar(TMenuBar::name, TMenuBar::build, __DELTA(TMenuBar)
    );

TStreamableClass RMenuBox(TMenuBox::name, TMenuBox::build, __DELTA(TMenuBox)
    );

TStreamableClass RMenuPopup(TMenuPopup::name,
			    TMenuPopup::build, __DELTA(TMenuPopup)
    );

TStreamableClass RMenuView(TMenuView::name, TMenuView::build, __DELTA(TMenuView)
    );

TStreamableClass RMultiCheckBoxes(TMultiCheckBoxes::name,
				  TMultiCheckBoxes::build,
				  __DELTA(TMultiCheckBoxes)
    );

TStreamableClass ROutline(TOutline::name, TOutline::build, __DELTA(TOutline)
    );

TStreamableClass RParamText(TParamText::name,
			    TParamText::build, __DELTA(TParamText)
    );

TStreamableClass RRadioButtons(TRadioButtons::name,
			       TRadioButtons::build, __DELTA(TRadioButtons)
    );

TStreamableClass RResourceCollection(TResourceCollection::name,
				     TResourceCollection::build,
				     __DELTA(TResourceCollection)
    );

TStreamableClass RScrollBar(TScrollBar::name,
			    TScrollBar::build, __DELTA(TScrollBar)
    );

TStreamableClass RScroller(TScroller::name, TScroller::build, __DELTA(TScroller)
    );

TStreamableClass RStaticText(TStaticText::name,
			     TStaticText::build, __DELTA(TStaticText)
    );

TStreamableClass RStatusLine(TStatusLine::name,
			     TStatusLine::build, __DELTA(TStatusLine)
    );

TStreamableClass RStringCollection(TStringCollection::name,
				   TStringCollection::build,
				   __DELTA(TStringCollection)
    );

TStreamableClass RStringList(TStringList::name,
			     TStringList::build, __DELTA(TStringList)
    );

TStreamableClass RValidator(TValidator::name,
			    TValidator::build, __DELTA(TValidator)
    );

TStreamableClass RFilterValidator(TFilterValidator::name,
				  TFilterValidator::build,
				  __DELTA(TFilterValidator)
    );

TStreamableClass RRangeValidator(TRangeValidator::name,
				 TRangeValidator::build,
				 __DELTA(TRangeValidator)
    );

TStreamableClass RPXPictureValidator(TPXPictureValidator::name,
				     TPXPictureValidator::build,
				     __DELTA(TPXPictureValidator)
    );

TStreamableClass RLookupValidator(TLookupValidator::name,
				  TLookupValidator::build,
				  __DELTA(TLookupValidator)
    );

TStreamableClass RStringLookupValidator(TStringLookupValidator::name,
					TStringLookupValidator::build,
					__DELTA(TStringLookupValidator)
    );

TStreamableClass RView(TView::name, TView::build, __DELTA(TView)
    );

TStreamableClass RWindow(TWindow::name, TWindow::build, __DELTA(TWindow)
    );
