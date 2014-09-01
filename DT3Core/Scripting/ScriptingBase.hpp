#ifndef DT3_SCRIPTINGBASE
#define DT3_SCRIPTINGBASE
//==============================================================================
///	
///	File: ScriptingBase.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/World/WorldNode.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Base class for all scripting nodes within the engine.
//==============================================================================

class ScriptingBase: public WorldNode {
    public:
        DEFINE_TYPE(ScriptingBase,WorldNode)
		//DEFINE_CREATE_AND_CLONE
 		DEFINE_PLUG_NODE
        
                                    ScriptingBase			(void);	
									ScriptingBase			(const ScriptingBase &rhs);
        ScriptingBase &				operator =				(const ScriptingBase &rhs);	
        virtual                     ~ScriptingBase			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
        /// Returns the preferred name of the object for when it is created
		/// \return preferred name of object
        std::string                 preferred_name          (void) const;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
