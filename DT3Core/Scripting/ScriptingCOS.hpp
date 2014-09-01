#ifndef DT3_SCRIPTINGCOS
#define DT3_SCRIPTINGCOS
//==============================================================================
///	
///	File: ScriptingCOS.hpp
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
/// Cosine. Input is Radians.
//==============================================================================

class ScriptingCOS: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCOS,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCOS			(void);	
									ScriptingCOS			(const ScriptingCOS &rhs);
        ScriptingCOS &				operator =				(const ScriptingCOS &rhs);	
        virtual                     ~ScriptingCOS			(void);
    
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
