#ifndef DT3_COMPONENTDRAWGRIDIMAGEPLANE
#define DT3_COMPONENTDRAWGRIDIMAGEPLANE
//==============================================================================
///	
///	File: ComponentDrawGridImagePlane.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Utility/GridImageData.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;

//==============================================================================
/// Component that can draw a grid warped image. A grid warp is a simple
/// way to animate a distortion in an image.
//==============================================================================

class ComponentDrawGridImagePlane: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentDrawGridImagePlane,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                        ComponentDrawGridImagePlane	(void);	
                                        ComponentDrawGridImagePlane	(const ComponentDrawGridImagePlane &rhs);
        ComponentDrawGridImagePlane &	operator =                  (const ComponentDrawGridImagePlane &rhs);	
        virtual                         ~ComponentDrawGridImagePlane(void);
    
        virtual void                    archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize			(void);
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        virtual ComponentType       component_type      (void)  {   return COMPONENT_DRAW;  }

		/// Draw Callback for component
		/// \param camera Camera used for drawing
        /// \param lag frame lag
        void                        draw                (const std::shared_ptr<CameraObject> &camera, const DTfloat lag);
    
		/// Returns the grid data.
		/// \return Grid data
        GridImageData&              grid                (void) {    return *_grid;  }
        
		/// Sets the number of grid points in the X direction
		/// \param num_x Number of grid points
   		void                        set_num_x           (const DTuint num_x);

		/// Returns the number of grid points in the X direction
		/// \return Number of grid points in X
   		const DTuint				num_x				(void) const;

		/// Sets the number of grid points in the Y direction
		/// \param num_y Number of grid points
   		void                        set_num_y			(const DTuint num_y);

		/// Returns the number of grid points in the Y direction
		/// \return Number of grid points in Y
   		const DTuint				num_y				(void) const;

        
		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                add_to_owner        (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                remove_from_owner   (void);

		/// Get and retrieve the color for this image
  		DEFINE_ACCESSORS (color, set_color, Color4f, _color);
    
		/// Get and retrieve the aspect ratio for this image
  		DEFINE_ACCESSORS (aspect, set_aspect, DTfloat, _aspect);
    
		/// Get and retrieve the material for this image
  		DEFINE_ACCESSORS (material, set_material, std::shared_ptr<MaterialResource>, _material);

		/// Get and retrieve the shader for this image
  		DEFINE_ACCESSORS (shader, set_shader, std::shared_ptr<ShaderResource>, _shader);

    private:
        DrawBatcher                                 _b;

		Plug<std::shared_ptr<MaterialResource> >    _material;
		Plug<std::shared_ptr<ShaderResource> >      _shader;

		Plug<Color4f>                               _color;
        DTfloat                                     _aspect;
        
        Plug<GridImageData>                         _grid;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
