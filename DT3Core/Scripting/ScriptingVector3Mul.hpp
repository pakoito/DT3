#ifndef DT3_SCRIPTINGVECTOR3MUL
#define DT3_SCRIPTINGVECTOR3MUL
//==============================================================================
///	
///	File: ScriptingVector3Mul.hpp
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
/// Multiplies a Vector times a scalar.
//==============================================================================

class ScriptingVector3Mul: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Mul,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Mul		(void);	
									ScriptingVector3Mul		(const ScriptingVector3Mul &rhs);
        ScriptingVector3Mul &		operator =				(const ScriptingVector3Mul &rhs);
        virtual						~ScriptingVector3Mul	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in1;
		Plug<DTfloat>				_in2;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
