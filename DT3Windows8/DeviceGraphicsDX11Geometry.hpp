#ifndef DT2_DEVICEGRAPHICSDX11GEOMETRY
#define DT2_DEVICEGRAPHICSDX11GEOMETRY
//==============================================================================
///	
///	File: DeviceGraphicsDX11Geometry.hpp
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
#include "WeightsIndex.hpp"
#include "DeviceGraphicsDX11Shader.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class DeviceGraphicsDX11IndexStream;
class DeviceGraphicsDX11DataStream;
class GeometryResource;

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11Geometry: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11Geometry,BaseClass)
		DEFINE_CREATE
         
                                    DeviceGraphicsDX11Geometry			(void);	
	private:
                                    DeviceGraphicsDX11Geometry			(const DeviceGraphicsDX11Geometry &rhs);
        DeviceGraphicsDX11Geometry &			operator =				(const DeviceGraphicsDX11Geometry &rhs);	
	public:
        virtual                     ~DeviceGraphicsDX11Geometry			(void);

        virtual void				archiveRead			(Archive *archive);
        virtual void				archiveWrite			(Archive *archive);
	        
	public:
		/// Description
		/// \param param description
		/// \return description
		void						syncToResource		(GeometryResource *geometry);
	
		/// Description
		/// \param param description
		/// \return description
		void						setIndexStream		(DeviceGraphicsDX11IndexStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11IndexStream*		getIndexStream		(void)	{	return _index_stream;					}
		
		/// Description
		/// \param param description
		/// \return description
		void						setColorsStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getColorsStream		(void)	{	return _color_stream;					}

		/// Description
		/// \param param description
		/// \return description
		void						setVertexStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getVertexStream		(void)	{	return _vertex_stream;					}

		/// Description
		/// \param param description
		/// \return description
		void						setNormalStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getNormalStream		(void)	{	return _normal_stream;					}

		/// Description
		/// \param param description
		/// \return description
		void						setWeightsStrengthStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getWeightsStrengthStream		(void)	{	return _weights_strength_stream;					}

		/// Description
		/// \param param description
		/// \return description
		void						setWeightsIndexStream		(DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getWeightsIndexStream		(void)	{	return _weights_index_stream;					}


		/// Description
		/// \param param description
		/// \return description
		void						setTexcoordStream		(DTushort tex_unit, DeviceGraphicsDX11DataStream *stream);

		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11DataStream*			getTexcoordStream		(DTushort tex_unit)	{	return _texcoord_stream[tex_unit];		}
		


		/// Description
		/// \param param description
		/// \return description
        void						draw					(void);

		/// Description
		/// \param param description
		/// \return description
        void						drawRange				(const DTuint first_tri, const DTuint num);
		
		/// Description
		/// \param param description
		/// \return description
		DTuint						countVertices			(void)  {   return _num_vertices;	    }

		/// Description
		/// \param param description
		/// \return description
		DTuint						countFaces				(void)  {   return _num_faces;			}


		// Compatibility routines

		/// Description
		/// \param param description
		/// \return description
		Vector3 *					getVertices			(void);

		/// Description
		/// \param param description
		/// \return description
		Vector3 *					getNormals				(void);

		/// Description
		/// \param param description
		/// \return description
		Vector3 *					getTangents			(void);

		/// Description
		/// \param param description
		/// \return description
		Texcoord2 *					getTexcoords1			(void);

		/// Description
		/// \param param description
		/// \return description
		Texcoord2 *					getTexcoords0			(void);

		/// Description
		/// \param param description
		/// \return description
		Triangle *					getFaces				(void);

		/// Description
		/// \param param description
		/// \return description
		Texcoord4 *					getWeightsStrength      (void);

		/// Description
		/// \param param description
		/// \return description
		WeightsIndex *				getWeightsIndex         (void);

		/// Description
		/// \param param description
		/// \return description
		DTuint						size					(void) const		{	return _size;	}

	private:
		DeviceGraphicsDX11IndexStream*		_index_stream;
		DeviceGraphicsDX11DataStream*		_color_stream;
		DeviceGraphicsDX11DataStream*		_vertex_stream;
		DeviceGraphicsDX11DataStream*		_normal_stream;
		DeviceGraphicsDX11DataStream*		_texcoord_stream[8];
		
        DeviceGraphicsDX11DataStream*      _weights_strength_stream;
        DeviceGraphicsDX11DataStream*      _weights_index_stream;

		DTuint						_num_faces;
		DTuint						_num_vertices;
		DTuint						_size;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
