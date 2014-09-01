#ifndef DT3_SCRIPTINGCRC32
#define DT3_SCRIPTINGCRC32
//==============================================================================
///	
///	File: ScriptingCRC32.hpp
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
/// Checksums a string.
//==============================================================================

class ScriptingCRC32: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCRC32,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCRC32			(void);	
									ScriptingCRC32			(const ScriptingCRC32 &rhs);
        ScriptingCRC32 &            operator =              (const ScriptingCRC32 &rhs);	
        virtual                     ~ScriptingCRC32			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<std::string>           _in;
		Plug<DTuint>				_checksum;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
