#ifndef DT3_SCRIPTINGFRAC
#define DT3_SCRIPTINGFRAC
//==============================================================================
///	
///	File: ScriptingFrac.hpp
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
/// Calculates the fractional component of an input.
//==============================================================================

class ScriptingFrac: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingFrac,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingFrac			(void);	
									ScriptingFrac			(const ScriptingFrac &rhs);
        ScriptingFrac &				operator =				(const ScriptingFrac &rhs);	
        virtual                     ~ScriptingFrac			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
