#ifndef DT3_SCRIPTINGLESSTHANEQUAL
#define DT3_SCRIPTINGLESSTHANEQUAL
//==============================================================================
///	
///	File: ScriptingLessThanEqual.hpp
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
/// Test for one number less than or equal to another.
//==============================================================================

class ScriptingLessThanEqual: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingLessThanEqual,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingLessThanEqual  (void);	
									ScriptingLessThanEqual  (const ScriptingLessThanEqual &rhs);
        ScriptingLessThanEqual &	operator =				(const ScriptingLessThanEqual &rhs);
        virtual						~ScriptingLessThanEqual (void);
    
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
