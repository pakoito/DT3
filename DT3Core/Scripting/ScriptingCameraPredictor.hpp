#ifndef DT3_SCRIPTINGCAMERAPREDICTOR
#define DT3_SCRIPTINGCAMERAPREDICTOR
//==============================================================================
///	
///	File: ScriptingCameraPredictor.hpp
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

//==============================================================================
/// Helpful node to redict where a camera will be given position and velocity.
//==============================================================================

class ScriptingCameraPredictor: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCameraPredictor,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCameraPredictor	(void);	
									ScriptingCameraPredictor	(const ScriptingCameraPredictor &rhs);
        ScriptingCameraPredictor &	operator =					(const ScriptingCameraPredictor &rhs);	
        virtual                     ~ScriptingCameraPredictor	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world           (void);
		
	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                        (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize					(void);
				
	private:		
		Plug<Vector3>				_object_translation;
		Plug<Vector3>				_object_velocity;
		Plug<Vector3>				_offset;
		Plug<Vector3>				_out_translation;
		Plug<Vector3>				_out_velocity;
		
		DTfloat						_prediction_strength;
		DTfloat						_prediction_smooth;
		
		Vector3						_prediction;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
