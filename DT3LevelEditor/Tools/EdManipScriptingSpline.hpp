#ifndef EDMANIPSCRIPTINGSPLINE
#define EDMANIPSCRIPTINGSPLINE
//==============================================================================
///	
///	File: EdManipScriptingSpline.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
// Engine includes

#include "EdLevelTool.hpp"
#include "MaterialResource.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlaceableObject;
class CameraObject;

//==============================================================================
/// Class
//==============================================================================

class EdManipScriptingSpline: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipScriptingSpline, EdLevelTool)
        DEFINE_CREATE

                                    EdManipScriptingSpline      (void);
    private:
									EdManipScriptingSpline      (const EdManipScriptingSpline &rhs);
        EdManipScriptingSpline &    operator =                  (const EdManipScriptingSpline &rhs);	
    
    public:
        virtual                     ~EdManipScriptingSpline     (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, CameraObject *camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        void            drawRing                (MaterialResource *mat, Matrix4 transform);

        DTuint          toID                    (DTuint index, DTuint type);
        void            fromID                  (DTuint id, DTuint &index, DTuint &type);

        enum {
            NONE = 0,
            VERTEX = 1,
            TANGENT = 2,
            ROTATE_X = 3,
            ROTATE_Y = 4,
            ROTATE_Z = 5
        };

        MaterialResource                _line_material;
        MaterialResource                _grey_line_material;
        MaterialResource                _shadow_line_material;
        MaterialResource                _red_material;
        MaterialResource                _green_material;
        MaterialResource                _blue_material;

        // Tracking information
        DTuint                          _starting_index;
        DTuint                          _starting_type;
        DTfloat                         _starting_location_dist;        

        DTint                           _mouse_x;
        DTint                           _mouse_y;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
