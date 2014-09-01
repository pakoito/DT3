#ifndef DT3_DEVICEINPUT
#define DT3_DEVICEINPUT
//==============================================================================
///	
///	File: DeviceInput.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TouchEvent;
class Vector3;

//==============================================================================
/// Class
//==============================================================================

class DeviceInput: public DeviceBase {
		/// Base class and default implementation for Input Manager.

    public:
        DEFINE_TYPE(DeviceInput,DeviceBase)
		DEFINE_CREATE
         
									DeviceInput			(void);	
	private:
									DeviceInput			(const DeviceInput &rhs);
        DeviceInput &				operator =				(const DeviceInput &rhs);	
    public:
        virtual						~DeviceInput			(void);
    
	public:
	
		// Keys
		enum {
			// The keyboard syms have been cleverly chosen to map to ASCII 
			INPUT_UNKNOWN       = 0,
			INPUT_FIRST         = 0,
			INPUT_BACKSPACE     = 8,
			INPUT_TAB           = 9,
			INPUT_CLEAR         = 12,
			INPUT_RETURN        = 13,
			INPUT_PAUSE         = 19,
			INPUT_ESCAPE        = 27,
			INPUT_SPACE         = 32,
			INPUT_EXCLAIM       = 33,
			INPUT_QUOTEDBL      = 34,
			INPUT_HASH          = 35,
			INPUT_DOLLAR        = 36,
			INPUT_AMPERSAND     = 38,
			INPUT_QUOTE         = 39,
			INPUT_LEFTPAREN     = 40,
			INPUT_RIGHTPAREN    = 41,
			INPUT_ASTERISK      = 42,
			INPUT_PLUS          = 43,
			INPUT_COMMA         = 44,
			INPUT_MINUS         = 45,
			INPUT_PERIOD        = 46,
			INPUT_SLASH         = 47,
			INPUT_0             = 48,
			INPUT_1             = 49,
			INPUT_2             = 50,
			INPUT_3             = 51,
			INPUT_4             = 52,
			INPUT_5             = 53,
			INPUT_6             = 54,
			INPUT_7             = 55,
			INPUT_8             = 56,
			INPUT_9             = 57,
			INPUT_COLON         = 58,
			INPUT_SEMICOLON     = 59,
			INPUT_LESS          = 60,
			INPUT_EQUALS        = 61,
			INPUT_GREATER       = 62,
			INPUT_QUESTION      = 63,
			INPUT_AT            = 64,
			// Skip uppercase letters
			 
			INPUT_LEFTBRACKET	= 91,
			INPUT_BACKSLASH		= 92,
			INPUT_RIGHTBRACKET	= 93,
			INPUT_CARET         = 94,
			INPUT_UNDERSCORE	= 95,
			INPUT_BACKQUOTE		= 96,
			INPUT_A             = 97,
			INPUT_B             = 98,
			INPUT_C             = 99,
			INPUT_D             = 100,
			INPUT_E             = 101,
			INPUT_F             = 102,
			INPUT_G             = 103,
			INPUT_H             = 104,
			INPUT_I             = 105,
			INPUT_J             = 106,
			INPUT_K             = 107,
			INPUT_L             = 108,
			INPUT_M             = 109,
			INPUT_N             = 110,
			INPUT_O             = 111,
			INPUT_P             = 112,
			INPUT_Q             = 113,
			INPUT_R             = 114,
			INPUT_S             = 115,
			INPUT_T             = 116,
			INPUT_U             = 117,
			INPUT_V             = 118,
			INPUT_W             = 119,
			INPUT_X             = 120,
			INPUT_Y             = 121,
			INPUT_Z             = 122,
			INPUT_DELETE		= 127,
			// end of ASCII mapped keysyms 

			// International keyboard syms 
			INPUT_WORLD_0		= 160,		// 0xA0 
			INPUT_WORLD_1		= 161,
			INPUT_WORLD_2		= 162,
			INPUT_WORLD_3		= 163,
			INPUT_WORLD_4		= 164,
			INPUT_WORLD_5		= 165,
			INPUT_WORLD_6		= 166,
			INPUT_WORLD_7		= 167,
			INPUT_WORLD_8		= 168,
			INPUT_WORLD_9		= 169,
			INPUT_WORLD_10		= 170,
			INPUT_WORLD_11		= 171,
			INPUT_WORLD_12		= 172,
			INPUT_WORLD_13		= 173,
			INPUT_WORLD_14		= 174,
			INPUT_WORLD_15		= 175,
			INPUT_WORLD_16		= 176,
			INPUT_WORLD_17		= 177,
			INPUT_WORLD_18		= 178,
			INPUT_WORLD_19		= 179,
			INPUT_WORLD_20		= 180,
			INPUT_WORLD_21		= 181,
			INPUT_WORLD_22		= 182,
			INPUT_WORLD_23		= 183,
			INPUT_WORLD_24		= 184,
			INPUT_WORLD_25		= 185,
			INPUT_WORLD_26		= 186,
			INPUT_WORLD_27		= 187,
			INPUT_WORLD_28		= 188,
			INPUT_WORLD_29		= 189,
			INPUT_WORLD_30		= 190,
			INPUT_WORLD_31		= 191,
			INPUT_WORLD_32		= 192,
			INPUT_WORLD_33		= 193,
			INPUT_WORLD_34		= 194,
			INPUT_WORLD_35		= 195,
			INPUT_WORLD_36		= 196,
			INPUT_WORLD_37		= 197,
			INPUT_WORLD_38		= 198,
			INPUT_WORLD_39		= 199,
			INPUT_WORLD_40		= 200,
			INPUT_WORLD_41		= 201,
			INPUT_WORLD_42		= 202,
			INPUT_WORLD_43		= 203,
			INPUT_WORLD_44		= 204,
			INPUT_WORLD_45		= 205,
			INPUT_WORLD_46		= 206,
			INPUT_WORLD_47		= 207,
			INPUT_WORLD_48		= 208,
			INPUT_WORLD_49		= 209,
			INPUT_WORLD_50		= 210,
			INPUT_WORLD_51		= 211,
			INPUT_WORLD_52		= 212,
			INPUT_WORLD_53		= 213,
			INPUT_WORLD_54		= 214,
			INPUT_WORLD_55		= 215,
			INPUT_WORLD_56		= 216,
			INPUT_WORLD_57		= 217,
			INPUT_WORLD_58		= 218,
			INPUT_WORLD_59		= 219,
			INPUT_WORLD_60		= 220,
			INPUT_WORLD_61		= 221,
			INPUT_WORLD_62		= 222,
			INPUT_WORLD_63		= 223,
			INPUT_WORLD_64		= 224,
			INPUT_WORLD_65		= 225,
			INPUT_WORLD_66		= 226,
			INPUT_WORLD_67		= 227,
			INPUT_WORLD_68		= 228,
			INPUT_WORLD_69		= 229,
			INPUT_WORLD_70		= 230,
			INPUT_WORLD_71		= 231,
			INPUT_WORLD_72		= 232,
			INPUT_WORLD_73		= 233,
			INPUT_WORLD_74		= 234,
			INPUT_WORLD_75		= 235,
			INPUT_WORLD_76		= 236,
			INPUT_WORLD_77		= 237,
			INPUT_WORLD_78		= 238,
			INPUT_WORLD_79		= 239,
			INPUT_WORLD_80		= 240,
			INPUT_WORLD_81		= 241,
			INPUT_WORLD_82		= 242,
			INPUT_WORLD_83		= 243,
			INPUT_WORLD_84		= 244,
			INPUT_WORLD_85		= 245,
			INPUT_WORLD_86		= 246,
			INPUT_WORLD_87		= 247,
			INPUT_WORLD_88		= 248,
			INPUT_WORLD_89		= 249,
			INPUT_WORLD_90		= 250,
			INPUT_WORLD_91		= 251,
			INPUT_WORLD_92		= 252,
			INPUT_WORLD_93		= 253,
			INPUT_WORLD_94		= 254,
			INPUT_WORLD_95		= 255,		// 0xFF 

			// Numeric keypad 
			INPUT_KP0           = 256,
			INPUT_KP1           = 257,
			INPUT_KP2           = 258,
			INPUT_KP3           = 259,
			INPUT_KP4           = 260,
			INPUT_KP5           = 261,
			INPUT_KP6           = 262,
			INPUT_KP7           = 263,
			INPUT_KP8           = 264,
			INPUT_KP9           = 265,
			INPUT_KP_PERIOD		= 266,
			INPUT_KP_DIVIDE		= 267,
			INPUT_KP_MULTIPLY	= 268,
			INPUT_KP_MINUS		= 269,
			INPUT_KP_PLUS		= 270,
			INPUT_KP_ENTER		= 271,
			INPUT_KP_EQUALS		= 272,

			// Arrows + Home/end pad 
			INPUT_UP			= 273,
			INPUT_DOWN          = 274,
			INPUT_RIGHT         = 275,
			INPUT_LEFT          = 276,
			INPUT_INSERT		= 277,
			INPUT_HOME          = 278,
			INPUT_END           = 279,
			INPUT_PAGEUP		= 280,
			INPUT_PAGEDOWN		= 281,

			// Function keys 
			INPUT_F1			= 282,
			INPUT_F2			= 283,
			INPUT_F3			= 284,
			INPUT_F4			= 285,
			INPUT_F5			= 286,
			INPUT_F6			= 287,
			INPUT_F7			= 288,
			INPUT_F8			= 289,
			INPUT_F9			= 290,
			INPUT_F10           = 291,
			INPUT_F11           = 292,
			INPUT_F12           = 293,
			INPUT_F13           = 294,
			INPUT_F14           = 295,
			INPUT_F15           = 296,

			// Key state modifier keys 
			INPUT_NUMLOCK		= 300,
			INPUT_CAPSLOCK		= 301,
			INPUT_SCROLLOCK		= 302,
			INPUT_RSHIFT		= 303,
			INPUT_LSHIFT		= 304,
			INPUT_RCTRL         = 305,
			INPUT_LCTRL         = 306,
			INPUT_RALT          = 307,
			INPUT_LALT          = 308,
			INPUT_RMETA         = 309,
			INPUT_LMETA         = 310,
			INPUT_LSUPER		= 311,		// Left "Windows" key 
			INPUT_RSUPER		= 312,		// Right "Windows" key 
			INPUT_MODE          = 313,		// "Alt Gr" key
			INPUT_COMPOSE		= 314,		// Multi-key compose key 

			// Miscellaneous function keys 
			INPUT_HELP          = 315,
			INPUT_PRINT         = 316,
			INPUT_SYSREQ		= 317,
			INPUT_BREAK         = 318,
			INPUT_MENU          = 319,
			INPUT_POWER         = 320,		// Power Macintosh power key
			INPUT_EURO          = 321,		// Some european keyboards
			INPUT_UNDO          = 322,		// Atari keyboard has Undo 

            INPUT_LAST
		};
		
		// Modifiers
		enum {
			MOD_NONE            = 0x0000,
			MOD_LSHIFT          = 0x0001,
			MOD_RSHIFT          = 0x0002,
			MOD_LCTRL           = 0x0040,
			MOD_RCTRL           = 0x0080,
			MOD_LALT            = 0x0100,
			MOD_RALT            = 0x0200,
			MOD_LMETA           = 0x0400,
			MOD_RMETA           = 0x0800,
			MOD_NUM             = 0x1000,
			MOD_CAPS            = 0x2000,
			MOD_MODE            = 0x4000,
			MOD_RESERVED        = 0x8000
		};

		/// Sets the current touch state
		/// \param event Touch state
		virtual void				set_touch_event             (TouchEvent *event);

		/// Sets the current keybaord state
		/// \param modifiers Bitmask of modifier keys
		/// \param key Key code
        virtual void                set_key_down_event          (DTuint modifiers, DTushort key);

		/// Sets the current keybaord state
		/// \param modifiers Bitmask of modifier keys
		/// \param key Key code
        virtual void                set_key_up_event            (DTuint modifiers, DTushort key);

		/// Fires a back button event (Android only)
		virtual void				do_back_button              (void);

		/// Fires a menu button event (Android only)
		virtual void				do_menu_button              (void);


		/// Sets the current accelerometer state
		/// \param a Acceleration
		virtual void				set_acceleration_event      (const Vector3 &a);

		/// Sets the current gyro state
		/// \param a Rotational acceleration
		virtual void				set_gyro_event              (const Vector3 &w);

		/// Sets the current magnetometer state
		/// \param a Rotational acceleration
		virtual void				set_magnetometer_event      (const Vector3 &m);

    
    
		/// Call to begin mouse capture
        virtual void                begin_capture_mouse         (void);

		/// Call to begin mouse capture
        virtual void                end_capture_mouse           (void);
};

//==============================================================================
//==============================================================================


} // DT3

#endif
