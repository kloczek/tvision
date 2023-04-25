/*
 * msgbox.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_MsgBox ) && !defined( TV_MsgBox )
#define TV_MsgBox

/** \file msgbox.h
 * msgbox.h
 */

#include <stdarg.h>

class TRect;

/** \fn messageBox( const char *msg, ushort aOptions )
 * Displays a message box with the given string in `msg'. `aOptions' is a
 * combination of one or more of the following message box constants:
 *
 * <pre>
 * Constant       Value  Meaning
 *
 * mfWarning      0x0000 Display a Warning box
 * mfError        0x0001 Display a Error box
 * mfInformation  0x0002 Display an Information Box
 * mfConfirmation 0x0003 Display a Confirmation Box
 * mfYesButton    0x0100 Put a Yes button into the dialog
 * mfNoButton     0x0200 Put a No button into the dialog
 * mfOKButton     0x0400 Put an OK button into the dialog
 * mfCancelButton 0x0800 Put a Cancel button into the dialog
 * </pre>
 *
 * The standard "Yes, No, Cancel" dialog box is defined as:
 *
 * <pre>
 * mfYesNoCancel = mfYesButton | mfNoButton | mfCancelButton;
 * </pre>
 *
 * The standard "OK, Cancel" dialog box is defined as:
 *
 * <pre>
 * mfOKCancel = mfOKButton | mfCancelButton;
 * </pre>
 */
ushort messageBox(const char *msg, ushort aOptions);

/** \fn messageBox( unsigned aOptions, const char *msg, ... )
 * Displays a message box. This form uses `msg' as a format string using the
 * additional parameters that follow it. `aOptions' is set to one of the
 * message box constants defined for @ref messageBox().
 */
ushort messageBox(unsigned aOptions, const char *msg, ...);

/** \fn messageBoxRect( const TRect &r, const char *msg, ushort aOptions )
 * Displays a message box with the given string in `msg' in the given
 * rectangle `r'. `aOptions' is set to one of the message box constants
 * defined for @ref messageBox().
 */
ushort messageBoxRect(const TRect & r, const char *msg, ushort aOptions);

/** \fn messageBoxRect( const TRect &r, ushort aOptions, const char *msg, ... )
 * Displays a message box in the given rectangle `r'. This form uses `msg' as
 * a format string using the additional parameters that follow it. `aOptions'
 * is set to one of the message box constants defined for @ref messageBox().
 */
ushort messageBoxRect(const TRect & r, ushort aOptions, const char *msg, ...);

/** \fn inputBox( const char *Title, const char *aLabel, char *s, uchar limit )
 * Displays an input box with the given title `title' and label `aLabel'.
 * Accepts input to string `s' with a maximum of `limit' characters.
 */
ushort inputBox(const char *Title, const char *aLabel, char *s, uchar limit);

/** \fn inputBoxRect( const TRect &bounds, const char *title, const char *aLabel, char *s, uchar limit )
 * Displays an input box with the given bounds `bounds', title `title' and
 * label `aLabel'. Accepts input to string `s' with a maximum of `limit'
 * characters.
 */
ushort inputBoxRect(const TRect & bounds, const char *title,
		    const char *aLabel, char *s, uchar limit);

const
//  Message box classes
 int mfWarning = 0x0000,	// Display a Warning box
    mfError = 0x0001,		// Display a Error box
    mfInformation = 0x0002,	// Display an Information Box
    mfConfirmation = 0x0003,	// Display a Confirmation Box
// Message box button flags
    mfYesButton = 0x0100,	// Put a Yes button into the dialog
    mfNoButton = 0x0200,	// Put a No button into the dialog
    mfOKButton = 0x0400,	// Put an OK button into the dialog
    mfCancelButton = 0x0800,	// Put a Cancel button into the dialog
    mfYesNoCancel = mfYesButton | mfNoButton | mfCancelButton,
    // Standard Yes, No, Cancel dialog
    mfOKCancel = mfOKButton | mfCancelButton;
				    // Standard OK, Cancel dialog
/**
 * This class stores a set of standard strings used in message boxes.
 *
 * If you need to change them, see file `tvtext.cc'.
 * @see messageBox
 * @short Contains a set of standard message strings
 */
class MsgBoxText {
      public:
    /**
     * Standard value is "~Y~es".
     */
	static const char *yesText;
    /**
     * Standard value is "~N~o".
     */
	static const char *noText;
    /**
     * Standard value is "O~K~".
     */
	static const char *okText;
    /**
     * Standard value is "Cancel".
     */
	static const char *cancelText;
    /**
     * Standard value is "Warning".
     */
	static const char *warningText;
    /**
     * Standard value is "Error".
     */
	static const char *errorText;
    /**
     * Standard value is "Information".
     */
	static const char *informationText;
    /**
     * Standard value is "Confirm".
     */
	static const char *confirmText;
};

#endif // Uses_MsgBox
