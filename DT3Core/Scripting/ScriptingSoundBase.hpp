#ifndef DT3_SCRIPTINGSOUNDBASE
#define DT3_SCRIPTINGSOUNDBASE
//==============================================================================
///	
///	File: ScriptingSoundBaseBase.hpp
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

class SoundResource;

//==============================================================================
/// Base object for all sound related nodes.
//==============================================================================

class ScriptingSoundBase: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSoundBase,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundBase      (void);	
									ScriptingSoundBase      (const ScriptingSoundBase &rhs);
        ScriptingSoundBase &		operator =				(const ScriptingSoundBase &rhs);	
        virtual                     ~ScriptingSoundBase     (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);

};

//==============================================================================
// Fixed point help
//==============================================================================

#define UNIT_TO_10BIT_INT(f)    ((DTint)(f * 1024))
#define PAD_10BIT(i)            (((DTint) i) << 10)
#define REM_10BIT(i)            (((DTint) i) >> 10)

#define CLAMP_PAD_10BIT(i)      ( MoreMath::max(PAD_10BIT(DTSHORT_MIN), MoreMath::min(PAD_10BIT(DTSHORT_MAX), i)) )
#define CLAMP_16BIT(i)          ( MoreMath::max(DTSHORT_MIN, MoreMath::min(DTSHORT_MAX,i)) )

//==============================================================================
//==============================================================================

} // DT3

#endif
