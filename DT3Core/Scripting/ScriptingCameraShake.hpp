#ifndef DT3_SCRIPTINGVECTOR3RANDOM
#define DT3_SCRIPTINGVECTOR3RANDOM
//==============================================================================
///	
///	File: ScriptingCameraShake.hpp
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
/// Simple camera shake.
//==============================================================================

class ScriptingCameraShake: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCameraShake,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingCameraShake	(void);	
									ScriptingCameraShake	(const ScriptingCameraShake &rhs);
        ScriptingCameraShake &		operator =				(const ScriptingCameraShake &rhs);	
        virtual						~ScriptingCameraShake	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);
        
	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize              (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<DTfloat>				_magnitude;
		Plug<DTboolean>				_on;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
