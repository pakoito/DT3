#ifndef DT3_SCRIPTINGCHANGERANGE
#define DT3_SCRIPTINGCHANGERANGE
//==============================================================================
///	
///	File: ScriptingChangeRange.hpp
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
/// Rescales an input range to a specified output range.
//==============================================================================

class ScriptingChangeRange: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingChangeRange,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingChangeRange	(void);	
									ScriptingChangeRange	(const ScriptingChangeRange &rhs);
        ScriptingChangeRange &		operator =				(const ScriptingChangeRange &rhs);	
        virtual						~ScriptingChangeRange	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in_low;
		Plug<DTfloat>				_in_high;
		Plug<DTfloat>				_out_low;
		Plug<DTfloat>				_out_high;
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
