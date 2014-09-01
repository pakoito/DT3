#ifndef EDMANIPCOMPONENTDRAWPOLYIMAGEPLANE
#define EDMANIPCOMPONENTDRAWPOLYIMAGEPLANE
//==============================================================================
///	
///	File: EdManipComponentDrawPolyImagePlane.hpp
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

class EdManipComponentDrawPolyImagePlane: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipComponentDrawPolyImagePlane, EdLevelTool)
        DEFINE_CREATE

                                                EdManipComponentDrawPolyImagePlane     (void);
    private:
                                                EdManipComponentDrawPolyImagePlane     (const EdManipComponentDrawPolyImagePlane &rhs);
        EdManipComponentDrawPolyImagePlane &    operator =                              (const EdManipComponentDrawPolyImagePlane &rhs);	
    
    public:
        virtual                                 ~EdManipComponentDrawPolyImagePlane    (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        
        DTuint          toID                    (DTuint x, DTuint y);
        void            fromID                  (DTuint id, DTuint &x, DTuint &y);

        MaterialResource                _line_material;
        MaterialResource                _grey_line_material;
        MaterialResource                _shadow_line_material;
        MaterialResource                _red_material;
        
        // Tracking information
        DTuint                          _starting_type;
        DTuint                          _starting_n;
        DTfloat                         _starting_location_dist;        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
