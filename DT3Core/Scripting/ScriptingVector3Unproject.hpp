#ifndef DT3_SCRIPTINGVECTOR3UNPROJECT
#define DT3_SCRIPTINGVECTOR3UNPROJECT
//==============================================================================
///	
///	File: ScriptingVector3Unproject.hpp
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
/// Class
//==============================================================================

class ScriptingVector3Unproject: public ScriptingBase {
		/// Base object for the different placeable types of objects in the engine.

    public:
        DEFINE_TYPE(ScriptingVector3Unproject,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Unproject   (void);
									ScriptingVector3Unproject   (const ScriptingVector3Unproject &rhs);
        ScriptingVector3Unproject &	operator =                  (const ScriptingVector3Unproject &rhs);
        virtual						~ScriptingVector3Unproject  (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
        Plug<std::shared_ptr<WorldNode>>   _camera;
		Plug<Vector3>                       _in;
		Plug<Vector3>                       _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
