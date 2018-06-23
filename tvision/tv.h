/*
 * tv.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1991, 1994 by Borland International
 * All Rights Reserved.
 *

DISCLAIMER AND LIMITATION OF LIABILITY: Borland does not make or give any 
representation or warranty with respect to the usefulness or the efficiency 
of this software, it being understood that the degree of success with which 
equipment, software, modifications, and other materials can be applied to 
data processing is dependent upon many factors, many of which are not under 
Borland's control.  ACCORDINGLY, THIS SOFTWARE IS PROVIDED 'AS IS' WITHOUT 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING NO WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE, OR NONINFRINGEMENT.  THIS SOFTWARE IS 
PROVIDED GRATUITOUSLY AND, ACCORDINGLY, BORLAND SHALL NOT BE LIABLE UNDER 
ANY THEORY FOR ANY DAMAGES SUFFERED BY YOU OR ANY USER OF THE SOFTWARE.  
BORLAND WILL NOT SUPPORT THIS SOFTWARE AND IS UNDER NO OBLIGATION TO ISSUE 
UPDATES TO THIS SOFTWARE.  

WITHOUT LIMITING THE GENERALITY OF THE FOREGOING, NEITHER BORLAND NOR ITS 
SUPPLIERS SHALL BE LIABLE FOR (a) INCIDENTAL, CONSEQUENTIAL, SPECIAL OR 
INDIRECT DAMAGES OF ANY SORT, WHETHER ARISING IN TORT, CONTRACT OR OTHERWISE, 
EVEN IF BORLAND HAS BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES, OR (b) 
FOR ANY CLAIM BY ANY OTHER PARTY.  SOME STATES DO NOT ALLOW THE EXCLUSION OR 
LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THIS LIMITATION AND 
EXCLUSION MAY NOT APPLY TO YOU.  Use, duplication or disclosure by the 
Government is subject to restrictions set forth in subparagraphs (a) through 
(d) of the Commercial Computer-Restricted Rights clause at FAR 52.227-19 when 
applicable, or in subparagraph (c) (1) (ii) of the Rights in Technical Data 
and Computer Software clause at DFARS 252.227-7013, and in similar clauses in 
the NASA AR Supplement.  Contractor/ manufacturer is Borland International, 
Inc., 100 Borland Way, Scotts Valley, CA 95066.

 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

/** \file tv.h
 * tv.h
 */

#define TV_VERSION 0x0200

#define Uses_EventCodes
#define Uses_ViewCommands
#define Uses_ipstream
#define Uses_opstream
#define TV_INC_STDDLG_H

#if defined( Uses_TApplication )
#define Uses_TProgram
#define Uses_TScreen
#define Uses_TEventQueue
#define TV_INC_APP_H
#endif

#if defined( Uses_TProgram )
#define Uses_TEvent
#define Uses_TGroup
#define TV_INC_APP_H
#endif

#if defined( Uses_TDeskTop )
#define Uses_TGroup
#define TV_INC_APP_H
#endif

#if defined( Uses_TBackground )
#define Uses_TView
#define TV_INC_APP_H
#endif

#if defined( Uses_TReplaceDialogRec )
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TFindDialogRec )
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TEditWindow )
#define Uses_TWindow
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TFileEditor )
#define Uses_TEditor
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TMemo )
#define Uses_TEditor
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TEditor )
#define Uses_TView
#define Uses_TPoint
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TIndicator )
#define Uses_TView
#define Uses_TPoint
#define TV_INC_EDITORS_H
#endif

#if defined( Uses_TTerminal )
#define Uses_TTextDevice
#define TV_INC_TEXTVIEW_H
#endif

#if defined( Uses_TOutline )
#define Uses_TOutlineViewer
#endif

#if defined( Uses_TOutlineViewer )
#define Uses_TScroller
#define Uses_TScrollBar
#define TV_INC_OUTLINE_H
#endif

#if defined( Uses_TTextDevice )
#define Uses_TScroller
#define TV_INC_TEXTVIEW_H
#endif

#if defined( Uses_TStatusLine )
#define Uses_TView
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TStatusDef )
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TStatusItem )
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenuPopup )
#define Uses_TMenuBox
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenuBox )
#define Uses_TMenuView
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenuBar )
#define Uses_TMenuView
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenuView )
#define Uses_TView
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenu )
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TSubMenu )
#define Uses_TMenuItem
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TMenuItem )
#define TV_INC_MENUS_H
#endif

#if defined( Uses_TColorDialog )
#define Uses_TColorGroup
#define Uses_TDialog
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorItemList )
#define Uses_TListViewer
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorGroupList )
#define Uses_TListViewer
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorDisplay )
#define Uses_TView
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TMonoSelector )
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TMonoSelector )
#define Uses_TCluster
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorSelector )
#define Uses_TView
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorGroup )
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TColorItem )
#define TV_INC_COLORSEL_H
#endif

#if defined( Uses_TChDirDialog )
#define Uses_TDialog
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TDirListBox )
#define Uses_TListBox
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TDirCollection )
#define Uses_TCollection
#define Uses_TDirEntry
#define TV_INC_STDDLG_H
#endif

#if defined ( Uses_TRangeValidator )
#define Uses_TFilterValidator
#endif

#if defined ( Uses_TFilterValidator )
#define Uses_TValidator
#endif

#if defined ( Uses_TStringLookupValidator )
#define Uses_TLookupValidator
#define Uses_TStringCollection
#endif

#if defined ( Uses_TLookupValidator )
#define Uses_TValidator
#endif

#if defined ( Uses_TPXPictureValidator )
#define Uses_TValidator
#endif

#if defined ( Uses_TValidator )
#define Uses_TObject
#define Uses_TStreamable
#define TV_INC_VALIDATOR_H
#endif

#if defined( Uses_TDirEntry )
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TFileDialog )
#define Uses_TDialog
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TFileInfoPane )
#define Uses_TView
#define Uses_TSearchRec
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TFileList )
#define Uses_TSortedListBox
#define Uses_TFileCollection
#define Uses_TSearchRec
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TSortedListBox )
#define Uses_TListBox
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TFileCollection )
#define Uses_TSortedCollection
#define Uses_TSearchRec
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TFileInputLine )
#define Uses_TInputLine
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_TSearchRec )
#define TV_INC_STDDLG_H
#endif

#if defined( Uses_THistory )
#define Uses_TView
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_THistoryWindow )
#define Uses_TWindow
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_THistoryViewer )
#define Uses_TListViewer
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TLabel )
#define Uses_TStaticText
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TParamText )
#define Uses_TStaticText
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TStaticText )
#define Uses_TView
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TListBox )
#define Uses_TListViewer
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TCheckBoxes )
#define Uses_TCluster
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TMultiCheckBoxes )
#define Uses_TCluster
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TRadioButtons )
#define Uses_TCluster
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TCluster )
#define Uses_TView
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TSItem )
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TButton )
#define Uses_TView
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TInputLine )
#define Uses_TView
#define Uses_TValidator
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TDialog )
#define Uses_TWindow
#define TV_INC_DIALOGS_H
#endif

#if defined( Uses_TVMemMgr )
#define TV_INC_BUFFERS_H
#endif

#if defined( Uses_TWindow )
#define Uses_TGroup
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TGroup )
#define Uses_TView
#define Uses_TRect
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TListViewer )
#define Uses_TView
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TScroller )
#define Uses_TView
#define Uses_TPoint
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TScrollBar )
#define Uses_TView
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TFrame )
#define Uses_TView
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TView )
#define Uses_TObject
#define Uses_TStreamable
#define Uses_TCommandSet
#define Uses_TPoint
#define Uses_TDrawBuffer
#define Uses_TPalette
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TPalette )
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TCommandSet )
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_ViewCommands )
#define TV_INC_VIEWS_H
#endif

#if defined( Uses_TStrListMaker )
#define Uses_TObject
#define Uses_TStreamable
#define Uses_TStrIndexRec
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TStringList )
#define Uses_TObject
#define Uses_TStreamable
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TStrIndexRec )
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TResourceFile )
#define Uses_TObject
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TResourceItem )
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TResourceCollection )
#define Uses_TStringCollection
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_TStringCollection )
#define Uses_TSortedCollection
#define TV_INC_RESOURCE_H
#endif

#if defined( Uses_MsgBox )
#define TV_INC_MSGBOX_H
#endif

#if defined( Uses_TSystemError )
#define TV_INC_SYSTEM_H
#endif

#if defined( Uses_TScreen )
#define TV_INC_SYSTEM_H
#endif

#if defined( Uses_TEventQueue )
#define Uses_TEvent
#define TV_INC_SYSTEM_H
#endif

#if defined( Uses_TEvent )
#define Uses_TPoint
#define TV_INC_SYSTEM_H
#endif

#if defined( Uses_THardwareInfo )
#define TV_INC_HARDWARE_H
#endif

#if defined( Uses_EventCodes )
#define TV_INC_SYSTEM_H
#endif

#if defined( Uses_TSortedCollection )
#define Uses_TNSSortedCollection
#define Uses_TCollection
#define TV_INC_OBJECTS_H
#endif

#if defined( Uses_TCollection )
#define Uses_TNSCollection
#define Uses_TStreamable
#define TV_INC_OBJECTS_H
#endif

#if defined( Uses_TRect )
#define Uses_TPoint
#define Uses_ipstream
#define Uses_opstream
#define TV_INC_OBJECTS_H
#endif

#if defined( Uses_TPoint )
#define TV_INC_OBJECTS_H
#endif

#if defined( Uses_TDrawBuffer )
#define TV_INC_DRAWBUF_H
#endif

#if defined( Uses_fpstream )
#define Uses_fpbase
#define Uses_iopstream
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_ofpstream )
#define Uses_fpbase
#define Uses_opstream
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_ifpstream )
#define Uses_fpbase
#define Uses_ipstream
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_fpbase )
#define Uses_pstream
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_iopstream )
#define Uses_ipstream
#define Uses_opstream
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_opstream )
#define Uses_pstream
#define Uses_TPWrittenObjects
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_ipstream )
#define Uses_pstream
#define Uses_TPReadObjects
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_pstream )
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TPReadObjects )
#define Uses_TNSCollection
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TPWrittenObjects )
#define Uses_TNSSortedCollection
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TStreamableTypes )
#define Uses_TNSSortedCollection
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TStreamableClass )
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TStreamable )
#define TV_INC_TOBJSTRM_H
#endif

#if defined( Uses_TNSSortedCollection )
#define Uses_TNSCollection
#define TV_INC_TVOBJS_H
#endif

#if defined( Uses_TNSCollection )
#define Uses_TObject
#define TV_INC_TVOBJS_H
#endif

#if defined( Uses_TObject )
#define TV_INC_TVOBJS_H
#endif

#if defined( Uses_TKeys )
#define TV_INC_TKEYS_H
#endif

#include <tvconfig.h>
#include <ttypes.h>

//#if defined( TV_INC_HARDWARE_H )
//#include <hardware.h>
//#endif

#if defined( TV_INC_TKEYS_H )
#include <tkeys.h>
#endif

#include <util.h>

#if defined( TV_INC_TVOBJS_H )
#include <tvobjs.h>
#endif

#if defined( TV_INC_TOBJSTRM_H )
#include <tobjstrm.h>
#endif

#if defined( TV_INC_DRAWBUF_H )
#include <drawbuf.h>
#endif

#if defined( TV_INC_OBJECTS_H )
#include <objects.h>
#endif

#if defined( TV_INC_SYSTEM_H )
#include <system.h>
#endif

#if defined( TV_INC_MSGBOX_H )
#include <msgbox.h>
#endif

#if defined( TV_INC_RESOURCE_H )
#include <resource.h>
#endif

#if defined( TV_INC_VIEWS_H )
#include <views.h>
#endif

#if defined( TV_INC_BUFFERS_H )
#include <buffers.h>
#endif

#if defined( TV_INC_DIALOGS_H )
#include <dialogs.h>
#endif

#if defined( TV_INC_VALIDATOR_H )
#include <validate.h>
#endif

#if defined( TV_INC_STDDLG_H )
#include <stddlg.h>
#endif

#if defined( TV_INC_COLORSEL_H )
#include <colorsel.h>
#endif

#if defined( TV_INC_MENUS_H )
#include <menus.h>
#endif

#if defined( TV_INC_TEXTVIEW_H )
#include <textview.h>
#endif

#if defined( TV_INC_EDITORS_H )
#include <editors.h>
#endif

#if defined( TV_INC_OUTLINE_H )
#include <outline.h>
#endif

#if defined( TV_INC_APP_H )
#include <app.h>
#endif
