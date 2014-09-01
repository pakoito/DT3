#ifndef DT3_SCRIPTINGCOMPONENTSTOVECTOR2
#define DT3_SCRIPTINGCOMPONENTSTOVECTOR2
//==============================================================================
///	
///	File: ScriptingComponentsToVector2.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Combines X,Y components into a vector2.
//==============================================================================

class ScriptingComponentsToVector2: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingComponentsToVector2,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingComponentsToVector2    (void);	
										ScriptingComponentsToVector2    (const ScriptingComponentsToVector2 &rhs);
        ScriptingComponentsToVector2 &	operator =						(const ScriptingComponentsToVector2 &rhs);	
        virtual							~ScriptingComponentsToVector2	(void);
    
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
		Plug<Vector2>                   _out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
