#ifndef DT3_SCRIPTINGSTRINGTOPATH
#define DT3_SCRIPTINGSTRINGTOPATH
//==============================================================================
///	
///	File: ScriptingStringToPath.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Converts a string to a path.
//==============================================================================

class ScriptingStringToPath: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingStringToPath,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingStringToPath		(void);	
									ScriptingStringToPath		(const ScriptingStringToPath &rhs);
        ScriptingStringToPath &     operator =					(const ScriptingStringToPath &rhs);	
        virtual						~ScriptingStringToPath      (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<std::string>			_in;
		Plug<FilePath>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
