#ifndef DT3_COMPONENTDRAWCAMERA
#define DT3_COMPONENTDRAWCAMERA
//==============================================================================
///	
///	File: ComponentDrawCamera.hpp
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

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Component that can draw a camera wireframe. This is useful for
/// visualizing different cameras in the world.
//==============================================================================

class ComponentDrawCamera: public ComponentBase {
    public:
        DEFINE_TYPE(ComponentDrawCamera,ComponentBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentDrawCamera	(void);	
									ComponentDrawCamera	(const ComponentDrawCamera &rhs);
        ComponentDrawCamera &		operator =			(const ComponentDrawCamera &rhs);	
        virtual                     ~ComponentDrawCamera(void);
    
        virtual void                archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize			(void);
        
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

		/// Get and retrieve the material for this image
  		DEFINE_ACCESSORS (material, set_material, std::shared_ptr<MaterialResource>, _material);

		/// Get and retrieve the shader for this image
  		DEFINE_ACCESSORS (shader, set_shader, std::shared_ptr<ShaderResource>, _shader);

    private:
		Plug<std::shared_ptr<MaterialResource> >    _material;
		Plug<std::shared_ptr<ShaderResource> >      _shader;

		Plug<Color4f>                               _color;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
