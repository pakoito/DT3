#ifndef DT3_SCRIPTINGROTATEX
#define DT3_SCRIPTINGROTATEX
//==============================================================================
///	
///	File: ScriptingRotateX.hpp
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
/// Rotate around X-Axis.
//==============================================================================

class ScriptingRotateX: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingRotateX,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingRotateX		(void);	
									ScriptingRotateX		(const ScriptingRotateX &rhs);
        ScriptingRotateX &			operator =				(const ScriptingRotateX &rhs);	
        virtual                     ~ScriptingRotateX		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Matrix3>				_in;
		Plug<DTfloat>				_angle;
		Plug<Matrix3>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
