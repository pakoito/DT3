#ifndef DT3_SCRIPTINGMULTIPLY
#define DT3_SCRIPTINGMULTIPLY
//==============================================================================
///	
///	File: ScriptingMultiply.hpp
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
/// Multiplies two numbers together.
//==============================================================================

class ScriptingMultiply: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMultiply,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMultiply		(void);	
									ScriptingMultiply		(const ScriptingMultiply &rhs);
        ScriptingMultiply &			operator =				(const ScriptingMultiply &rhs);	
        virtual						~ScriptingMultiply		(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in1;
		Plug<DTfloat>				_in2;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
