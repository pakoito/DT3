#ifndef DT3_SCRIPTINGVECTOR3TRANSFORM
#define DT3_SCRIPTINGVECTOR3TRANSFORM
//==============================================================================
///	
///	File: ScriptingVector3Transform.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Transform a vector.
//==============================================================================

class ScriptingVector3Transform: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Transform,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Transform	(void);	
									ScriptingVector3Transform	(const ScriptingVector3Transform &rhs);
        ScriptingVector3Transform &	operator =					(const ScriptingVector3Transform &rhs);	
        virtual						~ScriptingVector3Transform	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<Vector3>				_translation;
		Plug<Matrix3>				_orientation;
		Plug<DTfloat>				_scale;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
