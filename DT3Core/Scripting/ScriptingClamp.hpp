#ifndef DT3_SCRIPTINGCLAMP
#define DT3_SCRIPTINGCLAMP
//==============================================================================
///	
///	File: ScriptingClamp.hpp
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
/// Clamps an input between two values.
//==============================================================================

class ScriptingClamp: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingClamp,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingClamp			(void);	
									ScriptingClamp			(const ScriptingClamp &rhs);
        ScriptingClamp &			operator =				(const ScriptingClamp &rhs);	
        virtual                     ~ScriptingClamp			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_min;
		Plug<DTfloat>				_max;

		Plug<DTfloat>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
