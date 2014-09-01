#ifndef DT3_SCRIPTINGMATRIXTRANSPOSE
#define DT3_SCRIPTINGMATRIXTRANSPOSE
//==============================================================================
///	
///	File: ScriptingMatrix3Transpose.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Normalize a vector.
//==============================================================================

class ScriptingMatrix3Transpose: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMatrix3Transpose,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMatrix3Transpose	(void);	
									ScriptingMatrix3Transpose	(const ScriptingMatrix3Transpose &rhs);
        ScriptingMatrix3Transpose &	operator =					(const ScriptingMatrix3Transpose &rhs);	
        virtual						~ScriptingMatrix3Transpose	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Matrix3>				_in;
		Plug<Matrix3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
