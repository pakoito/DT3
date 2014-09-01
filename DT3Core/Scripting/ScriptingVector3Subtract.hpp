#ifndef DT3_SCRIPTINGVECTOR3SUBTRACT
#define DT3_SCRIPTINGVECTOR3SUBTRACT
//==============================================================================
///	
///	File: ScriptingVector3Subtract.hpp
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
/// Subtract two vectors.
//==============================================================================

class ScriptingVector3Subtract: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Subtract,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Subtract    (void);	
									ScriptingVector3Subtract    (const ScriptingVector3Subtract &rhs);
        ScriptingVector3Subtract &	operator =					(const ScriptingVector3Subtract &rhs);	
        virtual						~ScriptingVector3Subtract	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in1;
		Plug<Vector3>				_in2;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
