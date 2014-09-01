#ifndef DT3_SCRIPTINGSUBTRACT
#define DT3_SCRIPTINGSUBTRACT
//==============================================================================
///	
///	File: ScriptingSubtract.hpp
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
/// Subtract two floats.
//==============================================================================

class ScriptingSubtract: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSubtract,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingSubtract		(void);	
									ScriptingSubtract		(const ScriptingSubtract &rhs);
        ScriptingSubtract &			operator =				(const ScriptingSubtract &rhs);	
        virtual						~ScriptingSubtract		(void);
    
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
