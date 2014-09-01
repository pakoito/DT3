#ifndef DT3_SCRIPTINGCOLORINTERPOLATOR
#define DT3_SCRIPTINGCOLORINTERPOLATOR
//==============================================================================
///	
///	File: ScriptingColorInterpolator.hpp
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
/// Lerp for color values.
//==============================================================================

class ScriptingColorInterpolator: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingColorInterpolator,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                        ScriptingColorInterpolator	(void);	
                                        ScriptingColorInterpolator	(const ScriptingColorInterpolator &rhs);
        ScriptingColorInterpolator &    operator =                  (const ScriptingColorInterpolator &rhs);
        virtual                         ~ScriptingColorInterpolator	(void);
    
        virtual void                    archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                    initialize                  (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                       compute                     (const PlugBase *plug);

	private:		
		Plug<Color4f>                   _in_1;
		Plug<Color4f>                   _in_2;
		Plug<DTfloat>                   _t;
		Plug<Color4f>                   _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
