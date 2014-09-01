#ifndef DT3_SCRIPTINGVECTOR2TOCOMPONENTS
#define DT3_SCRIPTINGVECTOR2TOCOMPONENTS
//==============================================================================
///	
///	File: ScriptingVector2ToComponents.hpp
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
/// Convert Vector2 to X,Y components.
//==============================================================================

class ScriptingVector2ToComponents: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector2ToComponents,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingVector2ToComponents    (void);	
										ScriptingVector2ToComponents    (const ScriptingVector2ToComponents &rhs);
        ScriptingVector2ToComponents &	operator =						(const ScriptingVector2ToComponents &rhs);	
        virtual							~ScriptingVector2ToComponents	(void);
    
        virtual void					archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector2>				_in;

		Plug<DTfloat>				_out_x;
		Plug<DTfloat>				_out_y;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
