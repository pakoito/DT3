#ifndef DT3_SCRIPTINGVECTOR3NORMALIZE
#define DT3_SCRIPTINGVECTOR3NORMALIZE
//==============================================================================
///	
///	File: ScriptingVector3Normalize.hpp
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
/// Normalize a vector.
//==============================================================================

class ScriptingVector3Normalize: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Normalize,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Normalize   (void);	
									ScriptingVector3Normalize   (const ScriptingVector3Normalize &rhs);
        ScriptingVector3Normalize &	operator =					(const ScriptingVector3Normalize &rhs);	
        virtual						~ScriptingVector3Normalize	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<DTfloat>				_length;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
