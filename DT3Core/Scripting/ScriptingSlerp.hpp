#ifndef DT3_SCRIPTINGSLERP
#define DT3_SCRIPTINGSLERP
//==============================================================================
///	
///	File: ScriptingSlerp.hpp
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
/// Slerping two transforms.
//==============================================================================

class ScriptingSlerp: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSlerp,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingSlerp          (void);	
									ScriptingSlerp          (const ScriptingSlerp &rhs);
        ScriptingSlerp &            operator =				(const ScriptingSlerp &rhs);	
        virtual						~ScriptingSlerp         (void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Matrix3>				_in_orientation_0;
		Plug<Matrix3>				_in_orientation_1;
		Plug<Matrix3>				_out_orientation;

		Plug<Vector3>				_in_translation_0;
		Plug<Vector3>				_in_translation_1;
		Plug<Vector3>				_out_translation;

		Plug<Vector3>				_in_scale_0;
		Plug<Vector3>				_in_scale_1;
		Plug<Vector3>				_out_scale;

		Plug<DTfloat>				_t;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
