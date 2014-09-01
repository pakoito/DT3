#ifndef DT3_SCRIPTINGEQUALINT
#define DT3_SCRIPTINGEQUALINT
//==============================================================================
///	
///	File: ScriptingEqualInt.hpp
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
/// Tests two ints for equality.
//==============================================================================

class ScriptingEqualInt: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingEqualInt,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingEqualInt		(void);	
									ScriptingEqualInt		(const ScriptingEqualInt &rhs);
        ScriptingEqualInt &			operator =				(const ScriptingEqualInt &rhs);	
        virtual						~ScriptingEqualInt		(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTint>					_in1;
		Plug<DTint>					_in2;
		Plug<DTboolean>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
