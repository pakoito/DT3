#ifndef DT3_SCRIPTINGVECTOR3PASSTHROUGH
#define DT3_SCRIPTINGVECTOR3PASSTHROUGH
//==============================================================================
///	
///	File: ScriptingVector3Passthrough.hpp
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
/// Allows a vector to pass if condition is set.
//==============================================================================

class ScriptingVector3Passthrough: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Passthrough,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingVector3Passthrough     (void);	
										ScriptingVector3Passthrough     (const ScriptingVector3Passthrough &rhs);
        ScriptingVector3Passthrough &	operator =                      (const ScriptingVector3Passthrough &rhs);
        virtual							~ScriptingVector3Passthrough    (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<DTboolean>				_enable;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
