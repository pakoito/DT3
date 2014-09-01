#ifndef DT3_SCRIPTINGVECTOR3TOLENGTHSQUARED
#define DT3_SCRIPTINGVECTOR3TOLENGTHSQUARED
//==============================================================================
///	
///	File: ScriptingVector3ToLengthSquared.hpp
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
/// Length^2 of vector.
//==============================================================================

class ScriptingVector3ToLengthSquared: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3ToLengthSquared,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                            ScriptingVector3ToLengthSquared	(void);
                                            ScriptingVector3ToLengthSquared	(const ScriptingVector3ToLengthSquared &rhs);
        ScriptingVector3ToLengthSquared &	operator =                      (const ScriptingVector3ToLengthSquared &rhs);
        virtual                             ~ScriptingVector3ToLengthSquared(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_in;
		Plug<DTfloat>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
