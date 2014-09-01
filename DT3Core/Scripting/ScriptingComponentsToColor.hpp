#ifndef DT3_SCRIPTINGCOMPONENTSTOCOLOR
#define DT3_SCRIPTINGCOMPONENTSTOCOLOR
//==============================================================================
///	
///	File: ScriptingComponentsToColor.hpp
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
/// Combines R,G,B,A components into a color.
//==============================================================================

class ScriptingComponentsToColor: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingComponentsToColor,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingComponentsToColor		(void);	
										ScriptingComponentsToColor		(const ScriptingComponentsToColor &rhs);
        ScriptingComponentsToColor &	operator =						(const ScriptingComponentsToColor &rhs);	
        virtual							~ScriptingComponentsToColor     (void);
    
        virtual void					archive                         (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void					initialize                      (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean						compute                         (const PlugBase *plug);

	private:		
		Plug<DTfloat>                   _in_r;
		Plug<DTfloat>                   _in_g;
		Plug<DTfloat>                   _in_b;
		Plug<DTfloat>                   _in_a;
		Plug<Color4f>                   _out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
