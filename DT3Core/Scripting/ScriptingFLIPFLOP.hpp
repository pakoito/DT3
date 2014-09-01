#ifndef DT3_SCRIPTINGFLIPFLOP
#define DT3_SCRIPTINGFLIPFLOP
//==============================================================================
///	
///	File: ScriptingFLIPFLOP.hpp
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
/// Flip-Flop.
//==============================================================================

class ScriptingFLIPFLOP: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingFLIPFLOP,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingFLIPFLOP		(void);	
									ScriptingFLIPFLOP		(const ScriptingFLIPFLOP &rhs);
        ScriptingFLIPFLOP &			operator =				(const ScriptingFLIPFLOP &rhs);	
        virtual                     ~ScriptingFLIPFLOP		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
        
		/// Set Event
        void                        event_set               (PlugNode *sender);
    
		/// Reset Event
        void                        event_reset             (PlugNode *sender);

		/// Toggle Event
        void                        event_toggle            (PlugNode *sender);

	private:		
		Plug<DTboolean>				_set;
		Plug<DTboolean>				_reset;

		Event                       _set_e;
		Event                       _reset_e;

		Event                       _toggle_e;

		Plug<DTboolean>				_out;
		Plug<DTboolean>				_out_inv;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
