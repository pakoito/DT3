#ifndef DT3_SCRIPTINGGREATERTHAN
#define DT3_SCRIPTINGGREATERTHAN
//==============================================================================
///	
///	File: ScriptingInRange.hpp
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
/// Check if a number is in range.
//==============================================================================

class ScriptingInRange: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingInRange,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingInRange		(void);	
									ScriptingInRange		(const ScriptingInRange &rhs);
        ScriptingInRange &			operator =				(const ScriptingInRange &rhs);	
        virtual						~ScriptingInRange		(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_lower;
		Plug<DTfloat>				_upper;
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
