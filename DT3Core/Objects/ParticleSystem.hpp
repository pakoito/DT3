#ifndef DT3_PARTICLESYSTEM
#define DT3_PARTICLESYSTEM
//==============================================================================
///	
///	File: ParticleSystem.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;
class Stream;
class World;

//==============================================================================
/// Class
//==============================================================================

class ParticleSystem: public PlaceableObject {
    public:
        DEFINE_TYPE(ParticleSystem,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ParticleSystem					(void);	
                                    ParticleSystem					(const ParticleSystem &rhs);
        ParticleSystem &			operator =						(const ParticleSystem &rhs);	
        virtual                     ~ParticleSystem					(void);
        		
        virtual void                archive                         (const std::shared_ptr<Archive> &archive);
	
	public:
		/// Called to initialize the object
		virtual void				initialize						(void);

		DEFINE_ACCESSORS(material_property, set_material_property, std::shared_ptr<MaterialResource>, _material);
		DEFINE_ACCESSORS(particles_property, set_particles_property, std::shared_ptr<Particles>, _particles);
		DEFINE_ACCESSORS(run_up, set_run_up, DTfloat, _run_up);

		/// Draw Callback for object
		/// \param camera Camera used for drawing
		virtual void				draw							(const std::shared_ptr<CameraObject> &camera, const DTfloat lag);

		/// Callback called when the object is getting a tick
		/// \param dt delta time
		virtual void                tick							(const DTfloat dt);
	
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                    (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world               (void);

#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code                       (const std::string &object_name, Stream &s);
#endif

	private:        
		void						eval_particles					(const DTfloat dt);
		
        DTfloat                                     _run_up;
		Plug<std::shared_ptr<Particles>>            _particles;
		Plug<std::shared_ptr<MaterialResource>>		_material;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
