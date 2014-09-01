#ifndef DT3_SCRIPTINGSTRINGCONCAT
#define DT3_SCRIPTINGSTRINGCONCAT
//==============================================================================
///	
///	File: ScriptingStringConcat.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class ScriptingStringConcat: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingStringConcat,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingStringConcat	(void);	
									ScriptingStringConcat	(const ScriptingStringConcat &rhs);
        ScriptingStringConcat &     operator =				(const ScriptingStringConcat &rhs);	
        virtual						~ScriptingStringConcat  (void);
    
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
        Plug<DTboolean>             _insert_space;
		Plug<std::string>			_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
