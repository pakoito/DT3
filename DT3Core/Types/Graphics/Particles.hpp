#ifndef DT3_PARTICLES
#define DT3_PARTICLES
//==============================================================================
///	
///	File: Particles.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class DT3GLAttribBufferResource;
class CameraObject;
class Stream;

//==============================================================================
/// Particles resource type
//==============================================================================

class Particles: public BaseClass {
    public:
        DEFINE_TYPE(Particles,BaseClass)
		DEFINE_CREATE
    
                                                    Particles   (void);
	private:
                                                    Particles   (const Particles &rhs);
        Particles &                                 operator =  (const Particles &rhs);
		
	public:
        virtual                                     ~Particles  (void);

    public:

		DEFINE_ACCESSORS(active_start, set_active_start, DTint, _active_start);
		DEFINE_ACCESSORS(active_end, set_active_end, DTint, _active_end);

		DEFINE_ACCESSORS_REFERENCED_CONST(default_velocity, set_default_velocity, Vector3, _default_velocity);
		DEFINE_ACCESSORS(default_size, set_default_size, DTfloat, _default_size);
		DEFINE_ACCESSORS(default_lifetime, set_default_lifetime, DTfloat, _default_lifetime);
		DEFINE_ACCESSORS(default_rotation, set_default_rotation, DTfloat, _default_rotation);
		DEFINE_ACCESSORS_REFERENCED_CONST(default_color, set_default_color, Color4b, _default_color);


		DEFINE_ACCESSORS_REFERENCED(translations_stream, set_translations_stream, std::vector<Vector3>, _translations);
		DEFINE_ACCESSORS_REFERENCED(velocity_stream, set_velocity_stream, std::vector<Vector3>, _velocities);
		DEFINE_ACCESSORS_REFERENCED(sizes_stream, set_sizes_stream, std::vector<DTfloat>, _sizes);
		DEFINE_ACCESSORS_REFERENCED(lifetimes_stream, set_lifetimes_stream, std::vector<DTfloat>, _lifetimes);
		DEFINE_ACCESSORS_REFERENCED(color_stream, set_color_stream, std::vector<Color4b>, _colors);
		DEFINE_ACCESSORS_REFERENCED(rotations_stream, set_rotations_stream, std::vector<DTfloat>, _rotations);
    
		/// Description
		/// \param param description
		/// \return description
		void                                        build_velocity_stream   (void);
		void                                        build_sizes_stream		(void);
		void                                        build_lifetimes_stream  (void);
		void                                        build_colors_stream		(void);
		void                                        build_rotations_stream	(void);
	
		/// Description
		/// \param param description
		/// \return description
		void                                        resize					(DTuint size);
		
    
        /// Draws the resource
        /// \param camera Camera used for drawing
        void                                        draw                    (const std::shared_ptr<CameraObject> &camera, DTint first, DTint num);
    
	private:
        static void                                 reload_if_changed       (void);

        void                                        screen_opened           (DTuint width, DTuint height);
        void                                        screen_closed           (void);
    
		std::vector<Vector3>                        _translations;
		std::vector<Vector3>                        _velocities;
		std::vector<DTfloat>                        _sizes;
		std::vector<DTfloat>                        _lifetimes;
		std::vector<Color4b>                        _colors;
		std::vector<DTfloat>                        _rotations;
		
		Vector3                                     _default_velocity;
		DTfloat                                     _default_size;
		DTfloat                                     _default_lifetime;
		DTfloat                                     _default_rotation;
		Color4b                                     _default_color;
		
		DTint                                       _active_start;
		DTint                                       _active_end;
    
        // Resources
        std::shared_ptr<DT3GLAttribBufferResource>        _translations_resource;
        std::shared_ptr<DT3GLAttribBufferResource>        _texcoords_resource;
        std::shared_ptr<DT3GLAttribBufferResource>        _color_resource;
    
        DTsize                                      _stream_size;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<Particles> &r);
Stream& operator >>(Stream &s, std::shared_ptr<Particles> &r);

//==============================================================================
//==============================================================================


} // DT3

#endif
