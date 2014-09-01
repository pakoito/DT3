#ifndef DT3_SCRIPTINGXOR
#define DT3_SCRIPTINGXOR
//==============================================================================
///	
///	File: ScriptingXOR.hpp
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
/// Logical XOR.
//==============================================================================

class ScriptingXOR: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingXOR,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingXOR			(void);	
									ScriptingXOR			(const ScriptingXOR &rhs);
        ScriptingXOR &				operator =				(const ScriptingXOR &rhs);	
        virtual                     ~ScriptingXOR			(void);
    
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

		Plug<DTboolean>				_out;
		Plug<DTboolean>				_out_inv;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
