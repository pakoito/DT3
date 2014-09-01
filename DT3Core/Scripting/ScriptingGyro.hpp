#ifndef DT3_SCRIPTINGGYRO
#define DT3_SCRIPTINGGYRO
//==============================================================================
///	
///	File: ScriptingGyro.hpp
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
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class ScriptingGyro: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingGyro,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingGyro           (void);	
									ScriptingGyro           (const ScriptingGyro &rhs);
        ScriptingGyro &             operator =              (const ScriptingGyro &rhs);	
        virtual                     ~ScriptingGyro          (void);
    
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
        void                        accelerate              (const Vector3 &a);
        
	private:		
		Plug<Vector3>               _out;
		Plug<Vector3>				_out_low_freq;
		Plug<Vector3>				_out_high_freq;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
