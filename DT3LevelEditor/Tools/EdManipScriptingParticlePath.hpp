#ifndef EDMANIPSCRIPTINGPARTICLEPATH
#define EDMANIPSCRIPTINGPARTICLEPATH
//==============================================================================
///	
///	File: EdManipScriptingParticlePath.hpp
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

class EdManipScriptingParticlePath: public EdLevelTool
{
	public:
        DEFINE_TYPE(EdManipScriptingParticlePath, EdLevelTool)
        DEFINE_CREATE

                                        EdManipScriptingParticlePath    (void);
    private:
                                        EdManipScriptingParticlePath    (const EdManipScriptingParticlePath &rhs);
        EdManipScriptingParticlePath&   operator =                      (const EdManipScriptingParticlePath &rhs);	
    
    public:
        virtual                         ~EdManipScriptingParticlePath   (void);

    public:
        virtual void    draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
    
        virtual void    doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        
        virtual Matrix4 getManipulatorTransform (void) const;

    private:
        void            drawCube                (MaterialResource *mat, Matrix4 transform);
        
        DTuint          toID                    (DTuint index, DTuint type);
        void            fromID                  (DTuint id, DTuint &index, DTuint &type);

        enum {
            NONE = 0,
            VERTEX = 1,
            TANGENT = 2
        };

        MaterialResource                _line_material;
        MaterialResource                _grey_line_material;
        MaterialResource                _shadow_line_material;
        MaterialResource                _red_material;
        
        // Tracking information
        DTuint                          _starting_index;
        DTuint                          _starting_type;
        DTfloat                         _starting_location_dist;        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
