#ifndef DT3_SCRIPTINGMAX
#define DT3_SCRIPTINGMAX
//==============================================================================
///	
///	File: ScriptingMax.hpp
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
/// Outputs maximum of two values.
//==============================================================================

class ScriptingMax: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMax,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMax			(void);	
									ScriptingMax			(const ScriptingMax &rhs);
        ScriptingMax &				operator =				(const ScriptingMax &rhs);	
        virtual						~ScriptingMax			(void);
    
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
