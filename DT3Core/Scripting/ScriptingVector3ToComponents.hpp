#ifndef DT3_SCRIPTINGVECTOR3TOCOMPONENTS
#define DT3_SCRIPTINGVECTOR3TOCOMPONENTS
//==============================================================================
///	
///	File: ScriptingVector3ToComponents.hpp
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
/// Split Vector3 into X,Y,Z components.
//==============================================================================

class ScriptingVector3ToComponents: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3ToComponents,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingVector3ToComponents    (void);	
										ScriptingVector3ToComponents    (const ScriptingVector3ToComponents &rhs);
        ScriptingVector3ToComponents &	operator =						(const ScriptingVector3ToComponents &rhs);	
        virtual							~ScriptingVector3ToComponents	(void);
    
        virtual void					archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;

		Plug<DTfloat>				_out_x;
		Plug<DTfloat>				_out_y;
		Plug<DTfloat>				_out_z;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
