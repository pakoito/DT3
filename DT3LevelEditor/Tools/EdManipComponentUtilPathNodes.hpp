#ifndef EDMANIPCOMPONENTUTILPATHNODES
#define EDMANIPCOMPONENTUTILPATHNODES
//==============================================================================
///	
///	File: EdManipComponentUtilPathNodes.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
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

class EdManipComponentUtilPathNodes: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipComponentUtilPathNodes, EdLevelTool)
        DEFINE_CREATE

                                                EdManipComponentUtilPathNodes       (void);
    private:
                                                EdManipComponentUtilPathNodes       (const EdManipComponentUtilPathNodes &rhs);
        EdManipComponentUtilPathNodes &         operator =                          (const EdManipComponentUtilPathNodes &rhs);	
    
    public:
        virtual                                 ~EdManipComponentUtilPathNodes      (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        
        DTint           toID                    (DTint type, DTint n1, DTint n2 = 0);
        void            fromID                  (DTint id, DTint &type, DTint &n1, DTint &n2);
    
        enum {
            HANDLE=1,
            RING=2,
            CONNECTION=3
        };

        MaterialResource                _line_material;
        MaterialResource                _grey_line_material;
        MaterialResource                _shadow_line_material;
        MaterialResource                _red_material;
        MaterialResource                _green_material;
        
        // Tracking information
        DTint                           _starting_type;
        DTint                           _starting_n1;
        DTint                           _starting_n2;
        DTfloat                         _starting_location_dist;
    
        // Feedback when connecting
        DTint                           _feedback_from_connection;
        DTint                           _feedback_to_connection;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
