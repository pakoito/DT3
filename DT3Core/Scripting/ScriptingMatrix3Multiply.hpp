#ifndef DT3_SCRIPTINGMATRIX3MULTIPLY
#define DT3_SCRIPTINGMATRIX3MULTIPLY
//==============================================================================
///	
///	File: ScriptingMatrix3Multiply.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Multiplies two numbers together.
//==============================================================================

class ScriptingMatrix3Multiply: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMatrix3Multiply,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMatrix3Multiply    (void);
									ScriptingMatrix3Multiply    (const ScriptingMatrix3Multiply &rhs);
        ScriptingMatrix3Multiply &  operator =                  (const ScriptingMatrix3Multiply &rhs);
        virtual						~ScriptingMatrix3Multiply   (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Matrix3>				_in1;
		Plug<Matrix3>				_in2;
		Plug<Matrix3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
