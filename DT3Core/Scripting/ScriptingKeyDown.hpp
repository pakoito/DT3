#ifndef DT3_SCRIPTINGKEYDOWN
#define DT3_SCRIPTINGKEYDOWN
//==============================================================================
///	
///	File: ScriptingKeyDown.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Checks for a key down event.
//==============================================================================

class ScriptingKeyDown: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingKeyDown,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingKeyDown		(void);	
									ScriptingKeyDown		(const ScriptingKeyDown &rhs);
        ScriptingKeyDown &			operator =				(const ScriptingKeyDown &rhs);	
        virtual                     ~ScriptingKeyDown		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:

		/// Called to initialize the object
		virtual void				initialize				(void);

        /// Called in accelerometer callback
        void                        key                     (DTuint modifiers, DTushort key);
				
	private:		
		Plug<std::string>           _keys;
		
		Plug<DTboolean>				_left_shift;
		Plug<DTboolean>				_right_shift;
		Plug<DTboolean>				_left_ctrl;
		Plug<DTboolean>				_right_ctrl;
		Plug<DTboolean>				_left_alt;
		Plug<DTboolean>				_right_alt;
		Plug<DTboolean>				_left_meta;
		Plug<DTboolean>				_right_meta;
		Plug<DTboolean>				_caps_meta;
		
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
