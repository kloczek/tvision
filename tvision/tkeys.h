/*
 * tkeys.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if !defined( TV_TKEYS_H )
#define TV_TKEYS_H

/** \file tkeys.h
 * tkeys.h
 */


//  Control keys
//
//  NOTE: these Control key definitions are intended only to provide
//  mnemonic names for the ASCII control codes.  They cannot be used
//  to define menu hotkeys, etc., which require scan codes.

    /** \var kbCtrlA
     * Undocumented.
     * \var kbCtrlB
     * Undocumented.
     * \var kbCtrlC
     * Undocumented.
     */
const ushort
    kbCtrlA     = 0x0001,   kbCtrlB     = 0x0002,   kbCtrlC     = 0x0003,

    /** \var kbCtrlD
     * Undocumented.
     * \var kbCtrlE
     * Undocumented.
     * \var kbCtrlF
     * Undocumented.
     */
    kbCtrlD     = 0x0004,   kbCtrlE     = 0x0005,   kbCtrlF     = 0x0006,

    /** \var kbCtrlG
     * Undocumented.
     * \var kbCtrlH
     * Undocumented.
     * \var kbCtrlI
     * Undocumented.
     */
    kbCtrlG     = 0x0007,   kbCtrlH     = 0x0008,   kbCtrlI     = 0x0009,

    /** \var kbCtrlJ
     * Undocumented.
     * \var kbCtrlK
     * Undocumented.
     * \var kbCtrlL
     * Undocumented.
     */
    kbCtrlJ     = 0x000a,   kbCtrlK     = 0x000b,   kbCtrlL     = 0x000c,

    /** \var kbCtrlM
     * Undocumented.
     * \var kbCtrlN
     * Undocumented.
     * \var kbCtrlO
     * Undocumented.
     */
    kbCtrlM     = 0x000d,   kbCtrlN     = 0x000e,   kbCtrlO     = 0x000f,

    /** \var kbCtrlP
     * Undocumented.
     * \var kbCtrlQ
     * Undocumented.
     * \var kbCtrlR
     * Undocumented.
     */
    kbCtrlP     = 0x0010,   kbCtrlQ     = 0x0011,   kbCtrlR     = 0x0012,

    /** \var kbCtrlS
     * Undocumented.
     * \var kbCtrlT
     * Undocumented.
     * \var kbCtrlU
     * Undocumented.
     */
    kbCtrlS     = 0x0013,   kbCtrlT     = 0x0014,   kbCtrlU     = 0x0015,

    /** \var kbCtrlV
     * Undocumented.
     * \var kbCtrlW
     * Undocumented.
     * \var kbCtrlX
     * Undocumented.
     */
    kbCtrlV     = 0x0016,   kbCtrlW     = 0x0017,   kbCtrlX     = 0x0018,

    /** \var kbCtrlY
     * Undocumented.
     * \var kbCtrlZ
     * Undocumented.
     */
    kbCtrlY     = 0x0019,   kbCtrlZ     = 0x001a,

// Extended key codes

    /** \var kbEsc
     * Undocumented.
     * \var kbAltSpace
     * Undocumented.
     * \var kbCtrlIns
     * Undocumented.
     */
    kbEsc       = 0x011b,   kbAltSpace  = 0x0200,   kbCtrlIns   = 0x0400,

    /** \var kbShiftIns
     * Undocumented.
     * \var kbCtrlDel
     * Undocumented.
     * \var kbShiftDel
     * Undocumented.
     */
    kbShiftIns  = 0x0500,   kbCtrlDel   = 0x0600,   kbShiftDel  = 0x0700,

    /** \var kbBack
     * Undocumented.
     * \var kbCtrlBack
     * Undocumented.
     * \var kbShiftTab
     * Undocumented.
     */
    kbBack      = 0x0e08,   kbCtrlBack  = 0x0e7f,   kbShiftTab  = 0x0f00,

    /** \var kbTab
     * Undocumented.
     * \var kbAltQ
     * Undocumented.
     * \var kbAltW
     * Undocumented.
     */
    kbTab       = 0x0f09,   kbAltQ      = 0x1000,   kbAltW      = 0x1100,

    /** \var kbAltE
     * Undocumented.
     * \var kbAltR
     * Undocumented.
     * \var kbAltT
     * Undocumented.
     */
    kbAltE      = 0x1200,   kbAltR      = 0x1300,   kbAltT      = 0x1400,

    /** \var kbAltY
     * Undocumented.
     * \var kbAltU
     * Undocumented.
     * \var kbAltI
     * Undocumented.
     */
    kbAltY      = 0x1500,   kbAltU      = 0x1600,   kbAltI      = 0x1700,

    /** \var kbAltO
     * Undocumented.
     * \var kbAltP
     * Undocumented.
     * \var kbCtrlEnter
     * Undocumented.
     */
    kbAltO      = 0x1800,   kbAltP      = 0x1900,   kbCtrlEnter = 0x1c0a,

    /** \var kbEnter
     * Undocumented.
     * \var kbAltA
     * Undocumented.
     * \var kbAltS
     * Undocumented.
     */
    kbEnter     = 0x1c0d,   kbAltA      = 0x1e00,   kbAltS      = 0x1f00,

    /** \var kbAltD
     * Undocumented.
     * \var kbAltF
     * Undocumented.
     * \var kbAltG
     * Undocumented.
     */
    kbAltD      = 0x2000,   kbAltF      = 0x2100,   kbAltG      = 0x2200,

    /** \var kbAltH
     * Undocumented.
     * \var kbAltJ
     * Undocumented.
     * \var kbAltK
     * Undocumented.
     */
    kbAltH      = 0x2300,   kbAltJ      = 0x2400,   kbAltK      = 0x2500,

    /** \var kbAltL
     * Undocumented.
     * \var kbAltZ
     * Undocumented.
     * \var kbAltX
     * Undocumented.
     */
    kbAltL      = 0x2600,   kbAltZ      = 0x2c00,   kbAltX      = 0x2d00,

    /** \var kbAltC
     * Undocumented.
     * \var kbAltV
     * Undocumented.
     * \var kbAltB
     * Undocumented.
     */
    kbAltC      = 0x2e00,   kbAltV      = 0x2f00,   kbAltB      = 0x3000,

    /** \var kbAltN
     * Undocumented.
     * \var kbAltM
     * Undocumented.
     * \var kbF1
     * Undocumented.
     */
    kbAltN      = 0x3100,   kbAltM      = 0x3200,   kbF1        = 0x3b00,

    /** \var kbF2
     * Undocumented.
     * \var kbF3
     * Undocumented.
     * \var kbF4
     * Undocumented.
     */
    kbF2        = 0x3c00,   kbF3        = 0x3d00,   kbF4        = 0x3e00,

    /** \var kbF5
     * Undocumented.
     * \var kbF6
     * Undocumented.
     * \var kbF7
     * Undocumented.
     */
    kbF5        = 0x3f00,   kbF6        = 0x4000,   kbF7        = 0x4100,

    /** \var kbF8
     * Undocumented.
     * \var kbF9
     * Undocumented.
     * \var kbF10
     * Undocumented.
     */
    kbF8        = 0x4200,   kbF9        = 0x4300,   kbF10       = 0x4400,

    /** \var kbHome
     * Undocumented.
     * \var kbUp
     * Undocumented.
     * \var kbPgUp
     * Undocumented.
     */
    kbHome      = 0x4700,   kbUp        = 0x4800,   kbPgUp      = 0x4900,

    /** \var kbGrayMinus
     * Undocumented.
     * \var kbLeft
     * Undocumented.
     * \var kbRight
     * Undocumented.
     */
    kbGrayMinus = 0x4a2d,   kbLeft      = 0x4b00,   kbRight     = 0x4d00,

    /** \var kbGrayPlus
     * Undocumented.
     * \var kbEnd
     * Undocumented.
     * \var kbDown
     * Undocumented.
     */
    kbGrayPlus  = 0x4e2b,   kbEnd       = 0x4f00,   kbDown      = 0x5000,

    /** \var kbPgDn
     * Undocumented.
     * \var kbIns
     * Undocumented.
     * \var kbDel
     * Undocumented.
     */
    kbPgDn      = 0x5100,   kbIns       = 0x5200,   kbDel       = 0x5300,

    /** \var kbShiftF1
     * Undocumented.
     * \var kbShiftF2
     * Undocumented.
     * \var kbShiftF3
     * Undocumented.
     */
    kbShiftF1   = 0x5400,   kbShiftF2   = 0x5500,   kbShiftF3   = 0x5600,

    /** \var kbShiftF4
     * Undocumented.
     * \var kbShiftF5
     * Undocumented.
     * \var kbShiftF6
     * Undocumented.
     */
    kbShiftF4   = 0x5700,   kbShiftF5   = 0x5800,   kbShiftF6   = 0x5900,

    /** \var kbShiftF7
     * Undocumented.
     * \var kbShiftF8
     * Undocumented.
     * \var kbShiftF9
     * Undocumented.
     */
    kbShiftF7   = 0x5a00,   kbShiftF8   = 0x5b00,   kbShiftF9   = 0x5c00,

    /** \var kbShiftF10
     * Undocumented.
     * \var kbCtrlF1
     * Undocumented.
     * \var kbCtrlF2
     * Undocumented.
     */
    kbShiftF10  = 0x5d00,   kbCtrlF1    = 0x5e00,   kbCtrlF2    = 0x5f00,

    /** \var kbCtrlF3
     * Undocumented.
     * \var kbCtrlF4
     * Undocumented.
     * \var kbCtrlF5
     * Undocumented.
     */
    kbCtrlF3    = 0x6000,   kbCtrlF4    = 0x6100,   kbCtrlF5    = 0x6200,

    /** \var kbCtrlF6
     * Undocumented.
     * \var kbCtrlF7
     * Undocumented.
     * \var kbCtrlF8
     * Undocumented.
     */
    kbCtrlF6    = 0x6300,   kbCtrlF7    = 0x6400,   kbCtrlF8    = 0x6500,

    /** \var kbCtrlF9
     * Undocumented.
     * \var kbCtrlF10
     * Undocumented.
     * \var kbAltF1
     * Undocumented.
     */
    kbCtrlF9    = 0x6600,   kbCtrlF10   = 0x6700,   kbAltF1     = 0x6800,

    /** \var kbAltF2
     * Undocumented.
     * \var kbAltF3
     * Undocumented.
     * \var kbAltF4
     * Undocumented.
     */
    kbAltF2     = 0x6900,   kbAltF3     = 0x6a00,   kbAltF4     = 0x6b00,

    /** \var kbAltF5
     * Undocumented.
     * \var kbAltF6
     * Undocumented.
     * \var kbAltF7
     * Undocumented.
     */
    kbAltF5     = 0x6c00,   kbAltF6     = 0x6d00,   kbAltF7     = 0x6e00,

    /** \var kbAltF8
     * Undocumented.
     * \var kbAltF9
     * Undocumented.
     * \var kbAltF10
     * Undocumented.
     */
    kbAltF8     = 0x6f00,   kbAltF9     = 0x7000,   kbAltF10    = 0x7100,

    /** \var kbCtrlPrtSc
     * Undocumented.
     * \var kbCtrlLeft
     * Undocumented.
     * \var kbCtrlRight
     * Undocumented.
     */
    kbCtrlPrtSc = 0x7200,   kbCtrlLeft  = 0x7300,   kbCtrlRight = 0x7400,

    /** \var kbCtrlEnd
     * Undocumented.
     * \var kbCtrlPgDn
     * Undocumented.
     * \var kbCtrlHome
     * Undocumented.
     */
    kbCtrlEnd   = 0x7500,   kbCtrlPgDn  = 0x7600,   kbCtrlHome  = 0x7700,

    /** \var kbAlt1
     * Undocumented.
     * \var kbAlt2
     * Undocumented.
     * \var kbAlt3
     * Undocumented.
     */
    kbAlt1      = 0x7800,   kbAlt2      = 0x7900,   kbAlt3      = 0x7a00,

    /** \var kbAlt4
     * Undocumented.
     * \var kbAlt5
     * Undocumented.
     * \var kbAlt6
     * Undocumented.
     */
    kbAlt4      = 0x7b00,   kbAlt5      = 0x7c00,   kbAlt6      = 0x7d00,

    /** \var kbAlt7
     * Undocumented.
     * \var kbAlt8
     * Undocumented.
     * \var kbAlt9
     * Undocumented.
     */
    kbAlt7      = 0x7e00,   kbAlt8      = 0x7f00,   kbAlt9      = 0x8000,

    /** \var kbAlt0
     * Undocumented.
     * \var kbAltMinus
     * Undocumented.
     * \var kbAltEqual
     * Undocumented.
     */
    kbAlt0      = 0x8100,   kbAltMinus  = 0x8200,   kbAltEqual  = 0x8300,

    /** \var kbCtrlPgUp
     * Undocumented.
     * \var kbAltBack
     * Undocumented.
     * \var kbNoKey
     * Undocumented.
     */
    kbCtrlPgUp  = 0x8400,   kbAltBack   = 0x0800,   kbNoKey     = 0x0000,

//  Keyboard state and shift masks

    /** \var kbLeftShift
     * Undocumented.
     */
    kbLeftShift   = 0x0001,

    /** \var kbRightShift
     * Undocumented.
     */
    kbRightShift  = 0x0002,

    /** \var kbShift
     * Undocumented.
     */
    kbShift       = kbLeftShift | kbRightShift,

    /** \var kbLeftCtrl
     * Undocumented.
     */
    kbLeftCtrl    = 0x0004,

    /** \var kbRightCtrl
     * Undocumented.
     */
    kbRightCtrl   = 0x0004,

    /** \var kbCtrlShift
     * Undocumented.
     */
    kbCtrlShift   = kbLeftCtrl | kbRightCtrl,

    /** \var kbLeftAlt
     * Undocumented.
     */
    kbLeftAlt     = 0x0008,

    /** \var kbRightAlt
     * Undocumented.
     */
    kbRightAlt    = 0x0008,

    /** \var kbAltShift
     * Undocumented.
     */
    kbAltShift    = kbLeftAlt | kbRightAlt,

    /** \var kbScrollState
     * Undocumented.
     */
    kbScrollState = 0x0010,

    /** \var kbNumState
     * Undocumented.
     */
    kbNumState    = 0x0020,

    /** \var kbCapsState
     * Undocumented.
     */
    kbCapsState   = 0x0040,

    /** \var kbInsState
     * Undocumented.
     */
    kbInsState    = 0x0080;

#endif  // TV_TKEYS_H
