#ifndef DT3_SCRIPTINGMIN
#define DT3_SCRIPTINGMIN
//==============================================================================
///	
///	File: ScriptingMin.hpp
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
/// Outputs minimum of two values.
//==============================================================================

class ScriptingMin: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMin,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMin			(void);	
									ScriptingMin			(const ScriptingMin &rhs);
        ScriptingMin &				operator =				(const ScriptingMin &rhs);	
        virtual						~ScriptingMin			(void);
    
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
