#ifndef DT3_SCRIPTINGANIMATION
#define DT3_SCRIPTINGANIMATION
//==============================================================================
///	
///	File: ScriptingAnimation.hpp
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

class AnimationResource;
class AnimationPose;

//==============================================================================
/// Get a pose from an animation.
//==============================================================================

class ScriptingAnimation: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingAnimation,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                                    ScriptingAnimation		(void);
                                                    ScriptingAnimation		(const ScriptingAnimation &rhs);
        ScriptingAnimation &                        operator =				(const ScriptingAnimation &rhs);
        virtual                                     ~ScriptingAnimation		(void);
    
        virtual void                                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                                initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                                   compute					(const PlugBase *plug);


		/// Set the animation
		/// \param attr animation
        void                                        set_animation_property	(const std::shared_ptr<AnimationResource> &attr);

		/// Gets the animation
		/// \return animation
        const std::shared_ptr<AnimationResource>&   animation_property      (void) const				{   return _animation;	}

	private:		
		std::shared_ptr<AnimationResource>              _animation;
		Plug<DTfloat>									_time;
		Plug<std::shared_ptr<AnimationPose>>           _out_pose;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
