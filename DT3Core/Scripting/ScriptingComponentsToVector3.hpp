#ifndef DT3_SCRIPTINGCOMPONENTSTOVECTOR3
#define DT3_SCRIPTINGCOMPONENTSTOVECTOR3
//==============================================================================
///	
///	File: ScriptingComponentsToVector3.hpp
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
/// Combines X,Y,Z components into a vector.
//==============================================================================

class ScriptingComponentsToVector3: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingComponentsToVector3,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingComponentsToVector3    (void);	
										ScriptingComponentsToVector3    (const ScriptingComponentsToVector3 &rhs);
        ScriptingComponentsToVector3 &	operator =						(const ScriptingComponentsToVector3 &rhs);	
        virtual							~ScriptingComponentsToVector3	(void);
    
        virtual void					archive                         (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void					initialize                      (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean						compute                         (const PlugBase *plug);

	private:		
		Plug<DTfloat>                   _in_x;
		Plug<DTfloat>                   _in_y;
		Plug<DTfloat>                   _in_z;
		Plug<Vector3>                   _out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
