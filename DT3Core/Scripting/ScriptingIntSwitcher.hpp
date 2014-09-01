#ifndef DT3_SCRIPTINGINTSWITCHER
#define DT3_SCRIPTINGINTSWITCHER
//==============================================================================
///	
///	File: ScriptingIntSwitcher.hpp
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
/// Switcher for int values.
//==============================================================================

class ScriptingIntSwitcher: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingIntSwitcher,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingIntSwitcher	(void);	
									ScriptingIntSwitcher	(const ScriptingIntSwitcher &rhs);
        ScriptingIntSwitcher &      operator =				(const ScriptingIntSwitcher &rhs);	
        virtual                     ~ScriptingIntSwitcher	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTboolean>				_in1;
		Plug<DTint>					_in_int1;
		Plug<DTboolean>				_in2;
		Plug<DTint>					_in_int2;
		Plug<DTboolean>				_in3;
		Plug<DTint>					_in_int3;
		Plug<DTboolean>				_in4;
		Plug<DTint>					_in_int4;

		Plug<DTint>					_out;
        DTboolean                   _buffer_output;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
