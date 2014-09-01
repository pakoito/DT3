#ifndef DT3_SCRIPTINGGREATERTHAN
#define DT3_SCRIPTINGGREATERTHAN
//==============================================================================
///	
///	File: ScriptingGreaterThan.hpp
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
/// Test for one number greater than another.
//==============================================================================

class ScriptingGreaterThan: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingGreaterThan,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingGreaterThan    (void);	
									ScriptingGreaterThan    (const ScriptingGreaterThan &rhs);
        ScriptingGreaterThan &      operator =				(const ScriptingGreaterThan &rhs);
        virtual						~ScriptingGreaterThan   (void);
    
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
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
