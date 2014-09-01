#ifndef DT2_SCRIPTINGCAMERATEXTURE
#define DT2_SCRIPTINGCAMERATEXTURE
//==============================================================================
///	
///	File: ScriptingCameraTexture.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ScriptingBase.hpp"
#include "Vector.hpp"
#include "TextureResource.hpp"
#include "Rectangle.hpp"

namespace DT2 {

//==============================================================================
/// Accelerometer input.
//==============================================================================

class ScriptingCameraTexture: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCameraTexture,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCameraTexture	(void);	
									ScriptingCameraTexture	(const ScriptingCameraTexture &rhs);
        ScriptingCameraTexture &	operator =				(const ScriptingCameraTexture &rhs);	
        virtual                     ~ScriptingCameraTexture (void);
    
        virtual void                archiveRead             (Archive *archive);
        virtual void                archiveWrite			(Archive *archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                addToWorld              (World *world);

		/// Object was removed from a world
        virtual void                removeFromWorld         (void);

	public:

		/// Called to initialize the object
		virtual void				initialize				(void);
				
        /// Registered with world to tick this node
		/// \param dt delta time
		virtual void                tick                    (const DTfloat dt);

		/// Start
        void                        start                   (PlugNode *sender);

		/// Stop
        void                        stop                    (PlugNode *sender);
    
		/// Callback for when there's a new frame
        /// \param tex texture holding the pixel data
        /// \rect rectangle in the texture holding the pixel data
        void                        gotFrame                (std::shared_ptr<TextureResource> tex, Rectangle &rect);

	private:		
		Event                               _start;
		Event                               _stop;
    
        std::shared_ptr<TextureResource>           _frame;
        Rectangle                           _frame_rect;

		Plug<std::shared_ptr<TextureResource> >    _out;
		Plug<Rectangle>                     _rectangle;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
