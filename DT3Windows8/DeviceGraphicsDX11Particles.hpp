#ifndef DT2_DEVICEGRAPHICSDX11PARTICLES
#define DT2_DEVICEGRAPHICSDX11PARTICLES
//==============================================================================
///	
///	File: DeviceGraphicsDX11Particles.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "BaseClass.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "Texcoord.hpp"
#include "Weights.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class DeviceGraphicsDX11IndexStream;
class DeviceGraphicsDX11DataStream;
class ParticlesResource;

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11Particles: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11Particles,BaseClass)
		DEFINE_CREATE
         
                                    DeviceGraphicsDX11Particles		(void);	
	private:
                                    DeviceGraphicsDX11Particles		(const DeviceGraphicsDX11Particles &rhs);
        DeviceGraphicsDX11Particles &		operator =				(const DeviceGraphicsDX11Particles &rhs);	
	public:
        virtual                     ~DeviceGraphicsDX11Particles		(void);

        virtual void				archiveRead			(Archive *archive);
        virtual void				archiveWrite			(Archive *archive);
	        
	public:
		/// Description
		/// \param param description
		/// \return description
		void						syncToResource		(ParticlesResource *geometry);
	
		/// Description
		/// \param param description
		/// \return description
		void						setColorsStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*		getColorsStream		(void)	{	return _color_stream;					}

		/// Description
		/// \param param description
		/// \return description
		void						setVertexStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*		getVertexStream		(void)	{	return _vertex_stream;					}
	
		/// Description
		/// \param param description
		/// \return description
		void						setTexcoordStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*		getTexcoordStream		(void)	{	return _texcoord_stream;				}
		
		/// Description
		/// \param param description
		/// \return description
        void						drawRange				(const DTuint first, const DTuint num);

	private:
		DeviceGraphicsDX11DataStream*		_color_stream;
		DeviceGraphicsDX11DataStream*		_vertex_stream;
		DeviceGraphicsDX11DataStream*		_texcoord_stream;

		DTuint						_num_particles;
		DTuint						_size;
	
};

//==============================================================================
//==============================================================================

} // DT2

#endif
