#ifndef DT3_SCRIPTINGATAN2
#define DT3_SCRIPTINGATAN2
//==============================================================================
///	
///	File: ScriptingATAN2.hpp
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
/// Arctan. Equivalent to atan2 in C++.
//==============================================================================

class ScriptingATAN2: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingATAN2,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingATAN2			(void);	
									ScriptingATAN2			(const ScriptingATAN2 &rhs);
        ScriptingATAN2 &			operator =				(const ScriptingATAN2 &rhs);	
        virtual                     ~ScriptingATAN2			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTfloat>				_in_y;
		Plug<DTfloat>				_in_x;
		Plug<DTfloat>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
