#ifndef DT3_SCRIPTINGEASEINEASEOUT
#define DT3_SCRIPTINGEASEINEASEOUT
//==============================================================================
///	
///	File: ScriptingEaseInEaseOut.hpp
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
/// Takes 0-1 value and outputs an S-Curve.
//==============================================================================

class ScriptingEaseInEaseOut: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingEaseInEaseOut,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingEaseInEaseOut	(void);	
									ScriptingEaseInEaseOut	(const ScriptingEaseInEaseOut &rhs);
        ScriptingEaseInEaseOut &	operator =				(const ScriptingEaseInEaseOut &rhs);	
        virtual                     ~ScriptingEaseInEaseOut	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
