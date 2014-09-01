#ifndef EDMANIPGUIOBJECT
#define EDMANIPGUIOBJECT
//==============================================================================
///	
///	File: EdManipGUIObject.hpp
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

//==============================================================================
/// Forward declarations
//==============================================================================

class PlaceableObject;
class CameraObject;

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class EdManipGUIObject: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipGUIObject, EdLevelTool)
        DEFINE_CREATE

                                    EdManipGUIObject        (void);
    private:
									EdManipGUIObject        (const EdManipGUIObject &rhs);
        EdManipGUIObject &          operator =              (const EdManipGUIObject &rhs);	
    
    public:
        virtual                     ~EdManipGUIObject       (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        
        enum {
            NONE = 0,
            TRANSLATE = 1,
            MINUS_X = 2,
            PLUS_X = 3,
            MINUS_Y = 4,
            PLUS_Y = 5
        };

        MaterialResource                _line_material;
        MaterialResource                _red_material;
        
        // Tracking information
        DTuint                          _starting_control_id;
        DTfloat                         _starting_location_t1;
        DTfloat                         _starting_location_t2;        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
