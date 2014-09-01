#ifndef DT3_SCRIPTINGTOUCH
#define DT3_SCRIPTINGTOUCH
//==============================================================================
///	
///	File: ScriptingTouch.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TouchEvent;
class CameraObject;

//==============================================================================
/// Class
//==============================================================================

class ScriptingTouch: public ScriptingBase {
		/// Checks for a key down event.

    public:
        DEFINE_TYPE(ScriptingTouch,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTouch          (void);	
									ScriptingTouch          (const ScriptingTouch &rhs);
        ScriptingTouch &			operator =				(const ScriptingTouch &rhs);	
        virtual                     ~ScriptingTouch         (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
        virtual void                add_to_world            (World *world);
        virtual void                remove_from_world       (void);

	public:

		/// Callback called when the object is getting a touch event
		/// \param event Touch events
		/// \param camera The current camera
		/// \param handled reference to the currently handled widget
        void                        touch                   (const TouchEvent *event, const std::shared_ptr<CameraObject> &camera, WorldNode* &handled);

        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Draw Callback for component
		/// \param camera Camera used for drawing
        void                        draw                    (const std::shared_ptr<CameraObject> &camera, const DTfloat lag);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
	private:				
		Plug<Vector2>				_touch_pos_1;
		Plug<Vector2>				_touch_vel_1;
		Plug<Vector2>				_touch_initial_1;
		Plug<DTboolean>				_touch_active_1;

		Plug<Vector2>				_touch_pos_2;
		Plug<Vector2>				_touch_vel_2;
		Plug<Vector2>				_touch_initial_2;
		Plug<DTboolean>				_touch_active_2;

		Plug<Vector2>				_touch_pos_3;
		Plug<Vector2>				_touch_vel_3;
		Plug<Vector2>				_touch_initial_3;
		Plug<DTboolean>				_touch_active_3;
    
        DTboolean                   _clear_vel_1;
        DTboolean                   _clear_vel_2;
        DTboolean                   _clear_vel_3;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
