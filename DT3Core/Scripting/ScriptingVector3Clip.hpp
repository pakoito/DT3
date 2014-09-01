#ifndef DT3_SCRIPTINGVECTOR3CLIP
#define DT3_SCRIPTINGVECTOR3CLIP
//==============================================================================
///	
///	File: ScriptingVector3Clip.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Plane.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Figures out a clipping plane.
//==============================================================================

class ScriptingVector3Clip: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Clip,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Clip    (void);	
									ScriptingVector3Clip    (const ScriptingVector3Clip &rhs);
        ScriptingVector3Clip &		operator =				(const ScriptingVector3Clip &rhs);	
        virtual						~ScriptingVector3Clip	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<Plane>					_in_plane;
		Plug<Vector3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
