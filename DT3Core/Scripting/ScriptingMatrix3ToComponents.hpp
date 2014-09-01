#ifndef DT3_SCRIPTINGMATRIX3TOCOMPONENTS
#define DT3_SCRIPTINGMATRIX3TOCOMPONENTS
//==============================================================================
///	
///	File: ScriptingMatrix3ToComponents.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Breaks a matrix out into its components.
//==============================================================================

class ScriptingMatrix3ToComponents: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMatrix3ToComponents,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
										ScriptingMatrix3ToComponents    (void);
										ScriptingMatrix3ToComponents    (const ScriptingMatrix3ToComponents &rhs);
        ScriptingMatrix3ToComponents &	operator =						(const ScriptingMatrix3ToComponents &rhs);	
        virtual							~ScriptingMatrix3ToComponents	(void);
    
        virtual void					archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Matrix3>				_in;

		Plug<Vector3>				_out_x_axis;
		Plug<Vector3>				_out_y_axis;
		Plug<Vector3>				_out_z_axis;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
