#ifndef DT3_SCRIPTINGAND
#define DT3_SCRIPTINGAND
//==============================================================================
///	
///	File: ScriptingAND.hpp
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
/// Logical AND.
//==============================================================================

class ScriptingAND: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingAND,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingAND			(void);	
									ScriptingAND			(const ScriptingAND &rhs);
        ScriptingAND &				operator =				(const ScriptingAND &rhs);	
        virtual                     ~ScriptingAND			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTboolean>				_in1;
		Plug<DTboolean>				_in2;
		Plug<DTboolean>				_in3;
		Plug<DTboolean>				_in4;
		Plug<DTboolean>				_in5;
		Plug<DTboolean>				_in6;
		Plug<DTboolean>				_in7;
		Plug<DTboolean>				_in8;

		Plug<DTboolean>				_out;
		Plug<DTboolean>				_out_inv;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
