#ifndef DT3_SCRIPTINGKEYFRAMESTIMER
#define DT3_SCRIPTINGKEYFRAMESTIMER
//==============================================================================
///	
///	File: ScriptingKeyframesRoot.hpp
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
/// Root for keyframing
//==============================================================================

class ScriptingKeyframesRoot: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingKeyframesRoot,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingKeyframesRoot	(void);	
									ScriptingKeyframesRoot	(const ScriptingKeyframesRoot &rhs);
        ScriptingKeyframesRoot &	operator =				(const ScriptingKeyframesRoot &rhs);	
        virtual                     ~ScriptingKeyframesRoot (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		/// Sets the output time
		/// \param time time to set
		void						set_time_milliseconds   (DTfloat time)				{	_time_out = time/1000.0F;	}

		/// Sets the output time
		/// \param time time to set
		DTfloat						time_milliseconds       (void) const				{	return _time_out*1000.0F;	}
		
		/// Sets the output time
		/// \param time time to set
		void						set_time                (DTfloat time)				{	_time_out = time;	}

		/// Sets the output time
		/// \param time time to set
		DTfloat						time                    (void) const				{	return _time_out;	}
		
	private:			
		Plug<DTfloat>				_time_in;
		Plug<DTfloat>				_time_out;		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
