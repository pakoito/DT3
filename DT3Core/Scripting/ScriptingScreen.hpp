#ifndef DT3_SCRIPTINGSCREEN
#define DT3_SCRIPTINGSCREEN
//==============================================================================
///	
///	File: ScriptingScreen.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TouchEvent;
class camera;

//==============================================================================
/// Class
//==============================================================================

class ScriptingScreen: public ScriptingBase {
		/// Checks for a key down event.

    public:
        DEFINE_TYPE(ScriptingScreen,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingScreen         (void);	
									ScriptingScreen         (const ScriptingScreen &rhs);
        ScriptingScreen &			operator =				(const ScriptingScreen &rhs);	
        virtual                     ~ScriptingScreen        (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
        virtual void                archive_done            (const std::shared_ptr<Archive> &archive);
		
	public:

		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Screen changed callback.
        void                        screen_opened           (DTuint width, DTuint height);

		/// Screen changed callback.
        void                        screen_changed          (DTuint width, DTuint height);

	private:				
		Plug<DTfloat>				_width;
		Plug<DTfloat>				_height;
		Plug<DTfloat>				_aspect;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
