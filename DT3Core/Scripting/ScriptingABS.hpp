#ifndef DT3_SCRIPTINGABS
#define DT3_SCRIPTINGABS
//==============================================================================
///	
///	File: ScriptingABS.hpp
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
/// Absolute value.
//==============================================================================

class ScriptingABS: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingABS,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingABS			(void);	
									ScriptingABS			(const ScriptingABS &rhs);
        ScriptingABS &				operator =				(const ScriptingABS &rhs);	
        virtual                     ~ScriptingABS			(void);
    
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
