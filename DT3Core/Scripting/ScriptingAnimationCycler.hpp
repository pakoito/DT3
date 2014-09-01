#ifndef DT3_SCRIPTINGANIMATIONCYCLER
#define DT3_SCRIPTINGANIMATIONCYCLER
//==============================================================================
///	
///	File: ScriptingAnimationCycler.hpp
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
/// Class
//==============================================================================

class ScriptingAnimationCycler: public ScriptingBase {
		/// Constant speed spline.

    public:
        DEFINE_TYPE(ScriptingAnimationCycler,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingAnimationCycler    (void);	
									ScriptingAnimationCycler    (const ScriptingAnimationCycler &rhs);
        ScriptingAnimationCycler &  operator =                  (const ScriptingAnimationCycler &rhs);	
        virtual						~ScriptingAnimationCycler   (void);

        virtual void				archive                     (const std::shared_ptr<Archive> &archive);

	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute                     (const PlugBase *plug);
        
	private:	
            
		Plug<DTfloat>				_in;
		Plug<DTfloat>				_out;

		Plug<DTfloat>				_min_time;
		Plug<DTfloat>				_max_time;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
