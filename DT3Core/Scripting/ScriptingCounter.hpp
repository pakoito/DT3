#ifndef DT3_SCRIPTINGCOUNTER
#define DT3_SCRIPTINGCOUNTER
//==============================================================================
///	
///	File: ScriptingCounter.hpp
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
/// Event counter.
//==============================================================================

class ScriptingCounter: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCounter,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCounter		(void);	
									ScriptingCounter		(const ScriptingCounter &rhs);
        ScriptingCounter &			operator =				(const ScriptingCounter &rhs);	
        virtual                     ~ScriptingCounter		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
        
		/// Set Event
        void                        event_incr              (PlugNode *sender);
        
		/// Reset Event
        void                        event_decr              (PlugNode *sender);

		/// Toggle Event
        void                        event_reset             (PlugNode *sender);

	private:		
		Plug<DTint>                 _min;
		Plug<DTint>                 _max;
		Plug<DTboolean>				_wrap;

		Event                       _incr_e;
		Event                       _decr_e;
		Event                       _reset_e;

		Plug<DTint>                 _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
