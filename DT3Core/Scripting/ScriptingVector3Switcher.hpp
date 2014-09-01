#ifndef DT3_SCRIPTINGVECTOR3SWITCHER
#define DT3_SCRIPTINGVECTOR3SWITCHER
//==============================================================================
///	
///	File: ScriptingVector3Switcher.hpp
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
/// Switcher for Vector3.
//==============================================================================

class ScriptingVector3Switcher: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Switcher,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingVector3Switcher    (void);	
									ScriptingVector3Switcher    (const ScriptingVector3Switcher &rhs);
        ScriptingVector3Switcher &  operator =                  (const ScriptingVector3Switcher &rhs);
        virtual                     ~ScriptingVector3Switcher   (void);
    
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

		Plug<Vector3>				_in_vector1;
		Plug<Vector3>				_in_vector2;
		Plug<Vector3>				_in_vector3;
		Plug<Vector3>				_in_vector4;
		Plug<Vector3>				_in_vector5;
		Plug<Vector3>				_in_vector6;
		Plug<Vector3>				_in_vector7;
		Plug<Vector3>				_in_vector8;

		Plug<Vector3>				_out;
        DTboolean                   _buffer_output;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
