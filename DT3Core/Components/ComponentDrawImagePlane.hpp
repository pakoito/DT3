#ifndef DT3_COMPONENTDRAWIMAGEPLANE
#define DT3_COMPONENTDRAWIMAGEPLANE
//==============================================================================
///	
///	File: ComponentDrawImagePlane.hpp
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
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;

//==============================================================================
/// Component that can draw a rectangular image.
//==============================================================================

class ComponentDrawImagePlane: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentDrawImagePlane,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentDrawImagePlane     (void);
									ComponentDrawImagePlane     (const ComponentDrawImagePlane &rhs);
        ComponentDrawImagePlane &	operator =                  (const ComponentDrawImagePlane &rhs);
        virtual                     ~ComponentDrawImagePlane    (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                        initialize          (void);
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        virtual ComponentType               component_type      (void)  {   return COMPONENT_DRAW;  }

		/// Draw Callback for component
		/// \param camera Camera used for drawing
        /// \param lag frame lag
        void                                draw                (const std::shared_ptr<CameraObject> &camera, const DTfloat lag);
        
		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                        add_to_owner        (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                        remove_from_owner   (void);

        /// Defines standard accessors for the color of the image plane
  		DEFINE_ACCESSORS (color, set_color, Color4f, _color);
        
        /// Defines standard accessors for the aspect ratio
  		DEFINE_ACCESSORS (aspect, set_aspect, DTfloat, _aspect);

        /// Defines standard accessors for flip flags
  		DEFINE_ACCESSORS (flip_horz, set_flip_horz, DTboolean, _flip_horz);
  		DEFINE_ACCESSORS (flip_vert, set_flip_vert, DTboolean, _flip_vert);
    
		/// Returns the material for the Image Plane
		/// \return The material for the image plane
        const std::shared_ptr<MaterialResource>&    material            (void) const;
        
		/// Sets the material for the Image Plane
        /// \param material Pointer to new material
        void                                        set_material        (const std::shared_ptr<MaterialResource> &material);
    
		/// Sets the material for the image. This is only called from the editor when
        /// a material is changed in the property list.
		/// \param material Material for the image
        void                                        set_material_prop   (const std::shared_ptr<MaterialResource> &material);


		/// Returns the material for the Image Plane
		/// \return The material for the image plane
        const std::shared_ptr<ShaderResource>&      shader              (void) const;
        
		/// Sets the material for the Image Plane
        /// \param material Pointer to new material
        void                                        set_shader          (const std::shared_ptr<ShaderResource> &shader);

    private:
        DrawBatcher                                 _b;

		Plug<std::shared_ptr<MaterialResource> >    _material;
		Plug<std::shared_ptr<ShaderResource> >      _shader;
    
		Plug<Color4f>                               _color;
        DTfloat                                     _aspect;
        
        Plug<DTboolean>                             _flip_horz;
        Plug<DTboolean>                             _flip_vert;
    
        Plug<DTfloat>                               _tex_mul_s;
        Plug<DTfloat>                               _tex_mul_t;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
