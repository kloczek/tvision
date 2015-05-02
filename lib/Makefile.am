AUTOMAKE_OPTIONS = foreign

# Hack to create a symbolic link `tvision' pointing to this directory.
# This is needed because the sources contain directives like:
#     #include <tvision/tv.h>
# so the `tvision' directory must exist.

Makefile: tvision

tvision:
	ln -s $(srcdir)/. tvision

CLEANFILES = tvision

pkginclude_HEADERS = \
app.h buffers.h colorsel.h dialogs.h drawbuf.h editors.h \
help.h helpbase.h menus.h msgbox.h objects.h outline.h resource.h \
stddlg.h system.h textview.h tkeys.h tobjstrm.h ttypes.h tv.h \
tvconfig.h tvobjs.h util.h validate.h views.h

lib_LIBRARIES = libtvision.a
lib_LTLIBRARIES = libtvision.la
libtvision_la_LDFLAGS = -version-info 0:7:0

INCLUDES = -I..

libtvision_a_SOURCES = TApplication.cc TBackground.cc TButton.cc \
TChDirDialog.cc TCheckBoxes.cc \
TCluster.cc TCollection.cc TCommandSet.cc TDeskTop.cc TDialog.cc \
TDirCollection.cc TDirListBox.cc TEditWindow.cc TEditor.cc \
TFileCollection.cc TFileDialog.cc TFileEditor.cc TFileList.cc TFrame.cc \
TGroup.cc THistory.cc THistoryViewer.cc THistoryWindow.cc TIndicator.cc \
TInputLine.cc TLabel.cc TListBox.cc TListViewer.cc TMemo.cc TMenuBar.cc \
TMenuBox.cc TMenuPopup.cc TMenuView.cc TMultiCheckBoxes.cc TObject.cc \
TOutline.cc TParamText.cc TPoint.cc TProgram.cc TRadioButtons.cc \
TResourceCollection.cc TResourceFile.cc TScrollBar.cc TScroller.cc \
TSortedCollection.cc TStaticText.cc TStatusLine.cc TStrListMaker.cc \
TStringCollection.cc TValidator.cc TView.cc TWindow.cc allnames.cc asm.cc \
colorsel.cc drivers.cc editstat.cc help.cc helpbase.cc histlist.cc menu.cc \
misc.cc msgbox.cc new.cc palette.cc sall.cc stddlg.cc system.cc textview.cc \
tobjstrm.cc tvtext.cc

libtvision_la_SOURCES = tvision TApplication.cc TBackground.cc TButton.cc \
TChDirDialog.cc TCheckBoxes.cc \
TCluster.cc TCollection.cc TCommandSet.cc TDeskTop.cc TDialog.cc \
TDirCollection.cc TDirListBox.cc TEditWindow.cc TEditor.cc \
TFileCollection.cc TFileDialog.cc TFileEditor.cc TFileList.cc TFrame.cc \
TGroup.cc THistory.cc THistoryViewer.cc THistoryWindow.cc TIndicator.cc \
TInputLine.cc TLabel.cc TListBox.cc TListViewer.cc TMemo.cc TMenuBar.cc \
TMenuBox.cc TMenuPopup.cc TMenuView.cc TMultiCheckBoxes.cc TObject.cc \
TOutline.cc TParamText.cc TPoint.cc TProgram.cc TRadioButtons.cc \
TResourceCollection.cc TResourceFile.cc TScrollBar.cc TScroller.cc \
TSortedCollection.cc TStaticText.cc TStatusLine.cc TStrListMaker.cc \
TStringCollection.cc TValidator.cc TView.cc TWindow.cc allnames.cc asm.cc \
colorsel.cc drivers.cc editstat.cc help.cc helpbase.cc histlist.cc menu.cc \
misc.cc msgbox.cc new.cc palette.cc sall.cc stddlg.cc system.cc textview.cc \
tobjstrm.cc tvtext.cc
