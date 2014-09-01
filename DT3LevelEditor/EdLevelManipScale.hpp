#ifndef EDLEVELMANIPSCALE
#define EDLEVELMANIPSCALE
//==============================================================================
///	
///	File: EdLevelManipScale.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelTool.hpp"

// Qt include

// Engine includes
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlaceableObject;
    class CameraObject;
    class Matrix4;
}

class EdLevelToolWindow;
class EdLevelToolEvent;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelManipScale: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdLevelManipScale, EdLevelTool)
        DEFINE_CREATE

                        EdLevelManipScale       (void);
        virtual         ~EdLevelManipScale      (void)  {}

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        enum {
            PLUS_X = 1,
            PLUS_Y = 2,
            PLUS_Z = 3,
            CENTER = 4
        };

        std::shared_ptr<MaterialResource>   _tool_material;
        std::shared_ptr<ShaderResource>     _shader;

        DTuint                              _starting_axis;
        DTint                               _mouse_x;
        DTint                               _mouse_y;
};

//==============================================================================
//==============================================================================

#endif
