#ifndef DT3_SCRIPTINGVECTOR3RANDOM
#define DT3_SCRIPTINGVECTOR3RANDOM
//==============================================================================
///	
///	File: ScriptingVector3Random.hpp
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
/// Generate random direction.
//==============================================================================

class ScriptingVector3Random: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Random,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Random	(void);	
									ScriptingVector3Random	(const ScriptingVector3Random &rhs);
        ScriptingVector3Random &    operator =				(const ScriptingVector3Random &rhs);
        virtual						~ScriptingVector3Random	(void);
    
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
		virtual void				initialize				(void);
				
	private:		
		Plug<DTfloat>				_length;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
