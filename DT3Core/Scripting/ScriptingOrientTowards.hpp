#ifndef DT3_SCRIPTINGORIENTTOWARDS
#define DT3_SCRIPTINGORIENTTOWARDS
//==============================================================================
///	
///	File: ScriptingOrientTowards.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ScriptingOrientTowards: public ScriptingBase {
		/// Transform a vector.

    public:
        DEFINE_TYPE(ScriptingOrientTowards,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingOrientTowards	(void);	
									ScriptingOrientTowards	(const ScriptingOrientTowards &rhs);
        ScriptingOrientTowards &	operator =              (const ScriptingOrientTowards &rhs);	
        virtual						~ScriptingOrientTowards	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<Vector3>				_direction;
		Plug<Vector3>				_up;
		Plug<Matrix3>				_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
