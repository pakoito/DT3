#ifndef DT3_SCRIPTINGACCELEROMETER
#define DT3_SCRIPTINGACCELEROMETER
//==============================================================================
///	
///	File: ScriptingAccelerometer.hpp
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
/// Accelerometer input.
//==============================================================================

class ScriptingAccelerometer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingAccelerometer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingAccelerometer	(void);	
									ScriptingAccelerometer	(const ScriptingAccelerometer &rhs);
        ScriptingAccelerometer &	operator =				(const ScriptingAccelerometer &rhs);	
        virtual                     ~ScriptingAccelerometer (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
        virtual void                archive_done            (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:

		/// Called to initialize the object
		virtual void				initialize				(void);
				
        /// Called in accelerometer callback
        void                        accelerate              (const Vector3 &a);
        
        /// Registered with world to tick this node
		/// \param dt delta time
		virtual void                tick                    (const DTfloat dt);

#if DT3_OS == DT3_MACOSX || DT3_OS == DT3_WINDOWS
        /// Registered with system to get key presses
		/// \param modifiers modifiers
		/// \param key key code
        void                        key                     (DTuint modifiers, DTushort key);
#endif

	private:		
		Plug<DTboolean>             _enable;

		Plug<Vector3>               _out;
		Plug<Vector3>               _out_low_freq;
		Plug<Vector3>               _out_high_freq;
        
        DTfloat                     _warmup_time;
        DTfloat                     _current_warmup_time;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
