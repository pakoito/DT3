#ifndef DT3_SCRIPTINGPOWER
#define DT3_SCRIPTINGPOWER
//==============================================================================
///	
///	File: ScriptingPower.hpp
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
/// Class
//==============================================================================

class ScriptingPower: public ScriptingBase {
		/// Multiplies two numbers together.

    public:
        DEFINE_TYPE(ScriptingPower,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingPower          (void);	
									ScriptingPower          (const ScriptingPower &rhs);
        ScriptingPower &			operator =				(const ScriptingPower &rhs);	
        virtual						~ScriptingPower         (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_exp;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
