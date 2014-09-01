#ifndef DT3_SCRIPTINGSTRINGEQUALS
#define DT3_SCRIPTINGSTRINGEQUALS
//==============================================================================
///	
///	File: ScriptingStringEquals.hpp
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
/// Check string equality.
//==============================================================================

class ScriptingStringEquals: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingStringEquals,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingStringEquals		(void);	
									ScriptingStringEquals		(const ScriptingStringEquals &rhs);
        ScriptingStringEquals &     operator =					(const ScriptingStringEquals &rhs);	
        virtual						~ScriptingStringEquals      (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<std::string>           _in1;
		Plug<std::string>			_in2;
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
