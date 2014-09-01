#ifndef DT3_SCRIPTINGCOLORTOCOMPONENTS
#define DT3_SCRIPTINGCOLORTOCOMPONENTS
//==============================================================================
///	
///	File: ScriptingColorToComponents.hpp
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
/// Splits a color into R,G,B,A components.
//==============================================================================

class ScriptingColorToComponents: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingColorToComponents,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingColorToComponents		(void);	
										ScriptingColorToComponents		(const ScriptingColorToComponents &rhs);
        ScriptingColorToComponents &	operator =						(const ScriptingColorToComponents &rhs);	
        virtual							~ScriptingColorToComponents     (void);
    
        virtual void					archive                         (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                    initialize                      (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                       compute                         (const PlugBase *plug);

	private:		
		Plug<Color4f>                   _in;

		Plug<DTfloat>                   _out_r;
		Plug<DTfloat>                   _out_g;
		Plug<DTfloat>                   _out_b;
		Plug<DTfloat>                   _out_a;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
