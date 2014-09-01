#ifndef DT3_SCRIPTINGSIN
#define DT3_SCRIPTINGSIN
//==============================================================================
///	
///	File: ScriptingSIN.hpp
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
/// Implements SIN.
//==============================================================================

class ScriptingSIN: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSIN,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSIN			(void);	
									ScriptingSIN			(const ScriptingSIN &rhs);
        ScriptingSIN &				operator =				(const ScriptingSIN &rhs);	
        virtual                     ~ScriptingSIN			(void);
    
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
