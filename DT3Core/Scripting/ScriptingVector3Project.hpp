#ifndef DT3_SCRIPTINGVECTOR3PROJECT
#define DT3_SCRIPTINGVECTOR3PROJECT
//==============================================================================
///	
///	File: ScriptingVector3Project.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class WorldNode;

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class ScriptingVector3Project: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Project,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Project	(void);	
									ScriptingVector3Project	(const ScriptingVector3Project &rhs);
        ScriptingVector3Project &	operator =				(const ScriptingVector3Project &rhs);	
        virtual						~ScriptingVector3Project(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
    
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	private:		
        Plug<std::shared_ptr<WorldNode>>   _camera;
		Plug<Vector3>                       _in;
		Plug<Vector3>                       _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
