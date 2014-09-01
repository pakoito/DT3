#ifndef DT3_SCRIPTINGNOT
#define DT3_SCRIPTINGNOT
//==============================================================================
///	
///	File: ScriptingNOT.hpp
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
/// Logical NOT.
//==============================================================================

class ScriptingNOT: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingNOT,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingNOT			(void);	
									ScriptingNOT			(const ScriptingNOT &rhs);
        ScriptingNOT &				operator =				(const ScriptingNOT &rhs);	
        virtual                     ~ScriptingNOT			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTboolean>				_in;
		Plug<DTboolean>				_out;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
