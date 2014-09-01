#ifndef DT3_SCRIPTINGSINSUPERPOSITION
#define DT3_SCRIPTINGSINSUPERPOSITION
//==============================================================================
///	
///	File: ScriptingSinSuperposition.hpp
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
/// Superposition of SIN waves.
//==============================================================================

class ScriptingSinSuperposition: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSinSuperposition,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingSinSuperposition   (void);
									ScriptingSinSuperposition   (const ScriptingSinSuperposition &rhs);
        ScriptingSinSuperposition & operator =                  (const ScriptingSinSuperposition &rhs);
        virtual						~ScriptingSinSuperposition  (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_mul1;
		Plug<DTfloat>				_mul2;
		Plug<DTfloat>				_mul3;
		Plug<DTfloat>				_mul4;
		Plug<DTfloat>				_mul5;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
