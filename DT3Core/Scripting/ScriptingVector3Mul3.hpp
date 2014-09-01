#ifndef DT3_ScriptingVectorMul33
#define DT3_ScriptingVectorMul33
//==============================================================================
///	
///	File: ScriptingVector3Mul3.hpp
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
/// Multiplies a Vector times three scalars.
//==============================================================================

class ScriptingVector3Mul3: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Mul3,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Mul3		(void);	
									ScriptingVector3Mul3		(const ScriptingVector3Mul3 &rhs);
        ScriptingVector3Mul3 &		operator =					(const ScriptingVector3Mul3 &rhs);	
        virtual						~ScriptingVector3Mul3       (void);
    
        virtual void				archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute                     (const PlugBase *plug);

	private:		
		Plug<Vector3>				_in1;
        
		Plug<DTfloat>				_in_mul_x;
		Plug<DTfloat>				_in_mul_y;
		Plug<DTfloat>				_in_mul_z;
        
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
