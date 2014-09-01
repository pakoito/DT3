#ifndef EDMANIPCOMPONENTDRAWGRIDIMAGEPLANE
#define EDMANIPCOMPONENTDRAWGRIDIMAGEPLANE
//==============================================================================
///	
///	File: EdManipComponentDrawGridImagePlane.hpp
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

class EdManipComponentDrawGridImagePlane: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipComponentDrawGridImagePlane, EdLevelTool)
        DEFINE_CREATE

                                                EdManipComponentDrawGridImagePlane      (void);
    private:
                                                EdManipComponentDrawGridImagePlane      (const EdManipComponentDrawGridImagePlane &rhs);
        EdManipComponentDrawGridImagePlane &    operator =                              (const EdManipComponentDrawGridImagePlane &rhs);
    
    public:
        virtual                                 ~EdManipComponentDrawGridImagePlane     (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        
        DTuint          toID                    (DTint x, DTint y);
        void            fromID                  (DTint id, DTint &x, DTint &y);

        MaterialResource                _line_material;
        MaterialResource                _grey_line_material;
        MaterialResource                _shadow_line_material;
        MaterialResource                _red_material;
        
        // Tracking information
        DTint                           _starting_x;
        DTint                           _starting_y;
        DTfloat                         _starting_location_dist;        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
