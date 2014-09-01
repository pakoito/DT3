#ifndef DT3_SCRIPTINGVECTOR3TOLENGTH
#define DT3_SCRIPTINGVECTOR3TOLENGTH
//==============================================================================
///	
///	File: ScriptingVector3ToLength.hpp
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
/// Length of vector.
//==============================================================================

class ScriptingVector3ToLength: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3ToLength,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3ToLength    (void);	
									ScriptingVector3ToLength    (const ScriptingVector3ToLength &rhs);
        ScriptingVector3ToLength &	operator =					(const ScriptingVector3ToLength &rhs);	
        virtual						~ScriptingVector3ToLength	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
