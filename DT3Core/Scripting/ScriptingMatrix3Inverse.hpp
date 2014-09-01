#ifndef DT3_SCRIPTINGMATRIXINVERSE
#define DT3_SCRIPTINGMATRIXINVERSE
//==============================================================================
///	
///	File: ScriptingMatrix3Inverse.hpp
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

class ScriptingMatrix3Inverse: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMatrix3Inverse,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingMatrix3Inverse (void);	
									ScriptingMatrix3Inverse (const ScriptingMatrix3Inverse &rhs);
        ScriptingMatrix3Inverse &	operator =              (const ScriptingMatrix3Inverse &rhs);
        virtual						~ScriptingMatrix3Inverse(void);
    
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
