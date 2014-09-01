//==============================================================================
///	
///	File: DeviceGraphicsDX11Particles.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Particles.hpp"
#include "Assert.hpp"
#include "System.hpp"
#include "Archive.hpp"
#include "ArchiveObjectQueue.hpp"
#include "ArchiveProcessPointersRef.hpp"

#include "DeviceGraphics.hpp"
#include "ShaderResource.hpp"

#include "DeviceGraphicsDX11IndexStream.hpp"
#include "DeviceGraphicsDX11DataStream.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "DeviceGraphicsDX11Shader.hpp"

#include "DeviceConsole.hpp"
#include "Assert.hpp"
#include "Factory.hpp"

#include "Vector.hpp"
#include "Weights.hpp"
#include "Texcoord.hpp"
#include "Triangle.hpp"

#include "ParticlesResource.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Particles)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Particles::DeviceGraphicsDX11Particles (void)
	:	_color_stream	(NULL),
		_vertex_stream	(NULL),
		_texcoord_stream(NULL),
		_num_particles	(0),
		_size			(0)
{

}
					
DeviceGraphicsDX11Particles::~DeviceGraphicsDX11Particles (void)
{
	RELEASE(_color_stream);
	RELEASE(_vertex_stream);
	RELEASE(_texcoord_stream);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Particles::archiveRead (Archive *archive)
{
    archive->pushDomain (getClassID ());
    
	BaseClass::archiveRead(archive);

    archive->popDomain ();
}

void DeviceGraphicsDX11Particles::archiveWrite (Archive *archive)
{
    archive->pushDomain (getClassID ());

	BaseClass::archiveWrite(archive);
	
    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Particles::syncToResource		(ParticlesResource *particles)
{
	if (particles->getRecacheData()) {
		DTuint num_particles = particles->getTranslationsStream()->size();
	
		Vector3*		vertices = NULL;
		Texcoord2*		texcoords = NULL;
		Color*			colors = NULL;

		// Make sure arrays are the same size
		if (num_particles != _num_particles) {
			_num_particles = num_particles;
			_size = 0;
			
			if (particles->getTranslationsStream()) {
				DeviceGraphicsDX11DataStream *vertex_stream = DeviceGraphicsDX11DataStream::create();
				vertices = (Vector3*) vertex_stream->allocateData(sizeof(Vector3) * _num_particles * 6, DeviceGraphicsDX11DataStream::FMT_3_FLOAT, true);
				setVertexStream(vertex_stream);
				_size += sizeof(Vector3) * _num_particles * 6;

				DeviceGraphicsDX11DataStream *texcoord_stream = DeviceGraphicsDX11DataStream::create();
				texcoords = (Texcoord2*) texcoord_stream->allocateData(sizeof(Texcoord2) * _num_particles * 6, DeviceGraphicsDX11DataStream::FMT_2_FLOAT, true);
				setTexcoordStream(texcoord_stream);
				_size += sizeof(Texcoord2) * _num_particles * 6;
				
				_vertex_stream->uploadData();
				_texcoord_stream->uploadData();
				
				RELEASE(vertex_stream);
				RELEASE(texcoord_stream);
			}
			
			if (particles->getColorsStream()) {
				DeviceGraphicsDX11DataStream *color_stream = DeviceGraphicsDX11DataStream::create();
				colors = (Color*) color_stream->allocateData(sizeof(Color) * _num_particles, DeviceGraphicsDX11DataStream::FMT_4_BYTE, true);
				setColorsStream(color_stream);
				_size += sizeof(Color) * _num_particles * 6;

				color_stream->uploadData();
				RELEASE(color_stream);
			}
		}
		
		// Upload vertex data
		if (_vertex_stream) {

			// map the stream and change it
			Vector3* vertex_stream = (Vector3*) _vertex_stream->map(false, true);
			Assert(vertex_stream);

			Texcoord2* texcoord_stream = (Texcoord2*) _texcoord_stream->map(false, true);
			Assert(texcoord_stream);
            
            DTfloat size_mul = System::getRenderer()->getViewportHeight() / 768.0F;
			
			for (DTuint i = 0; i < _num_particles; ++i) {
				const Vector3 &translation = particles->getTranslationsStream()->get(i);

                // Default texcoords
                Vector2 tc0(0.0F,0.0F);
                Vector2 tc1(1.0F,0.0F);
                Vector2 tc2(1.0F,1.0F);
                Vector2 tc3(0.0F,1.0F);

                // Default vector offsets
				Vector3 vp0(-size_mul,-size_mul,0.0F);
				Vector3 vp1(size_mul,-size_mul,0.0F);
				Vector3 vp2(size_mul,size_mul,0.0F);
				Vector3 vp3(-size_mul,size_mul,0.0F);

				// Change size if need be
				if (particles->getSizesStream()) {
                    DTfloat size = particles->getSizesStream()->get(i);

                    vp0 *= size;
                    vp1 *= size;
                    vp2 *= size;
                    vp3 *= size;
                }
				
				// Change rotation if need be
				if (particles->getRotationsStream()) {
                    DTfloat rot = particles->getRotationsStream()->get(i);

            	    DTfloat cosa = std::cos(rot);
				    DTfloat sina = std::sin(rot);

				    Matrix2 rotm = Matrix2( cosa,-sina,
                                            sina,cosa   );

				    tc0 -= Vector2(0.5F,0.5F);
                    tc0 = rotm * tc0;
                    tc0 += Vector2(0.5F,0.5F);

                    tc1 -= Vector2(0.5F,0.5F);
                    tc1 = rotm * tc1;
                    tc1 += Vector2(0.5F,0.5F);

                    tc2 -= Vector2(0.5F,0.5F);
                    tc2 = rotm * tc2;
                    tc2 += Vector2(0.5F,0.5F);

                    tc3 -= Vector2(0.5F,0.5F);
                    tc3 = rotm * tc3;
                    tc3 += Vector2(0.5F,0.5F);
				}

                vp0 += translation;
                vp1 += translation;
                vp2 += translation;
                vp3 += translation;

			    vertex_stream[i*6+0] = vp0;
                vertex_stream[i*6+1] = vp1;
                vertex_stream[i*6+2] = vp3;
                vertex_stream[i*6+3] = vp1;
                vertex_stream[i*6+4] = vp2;
                vertex_stream[i*6+5] = vp3;	

                texcoord_stream[i*6+0] = Texcoord2(tc0.x,tc0.y);
                texcoord_stream[i*6+1] = Texcoord2(tc1.x,tc1.y);
                texcoord_stream[i*6+2] = Texcoord2(tc3.x,tc3.y);
                texcoord_stream[i*6+3] = Texcoord2(tc1.x,tc1.y);
                texcoord_stream[i*6+4] = Texcoord2(tc2.x,tc2.y);
                texcoord_stream[i*6+5] = Texcoord2(tc3.x,tc3.y);	
			}

			_vertex_stream->unmap();
            _texcoord_stream->unmap();
		}
		
		// Upload color data
		if (_color_stream) {
		
			// map the stream and change it
			Color* color_stream = (Color*) _color_stream->map(false, true);
			Assert(color_stream);

			for (DTuint i = 0; i < _num_particles; ++i) {
				const Color &color = particles->getColorsStream()->get(i);
				
				color_stream[i*6+0] = color;
				color_stream[i*6+1] = color;
				color_stream[i*6+2] = color;
				color_stream[i*6+3] = color;
				color_stream[i*6+4] = color;
				color_stream[i*6+5] = color;
			}
			
			_color_stream->unmap();
		}
	
	
		particles->setRecacheData(false);
	}
	
	if (particles->getRecacheParameters()) {
		particles->setRecacheParameters(false);
	}
	
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Particles::setColorsStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_color_stream, stream);
}

void DeviceGraphicsDX11Particles::setVertexStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_vertex_stream, stream);
}

void DeviceGraphicsDX11Particles::setTexcoordStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_texcoord_stream, stream);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Particles::drawRange(const DTuint first, const DTuint num)
{
	// If there are VBO's
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

    ID3D11Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
    UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
    UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

	if (_color_stream) {
        buffers[ShaderResource::ATTRIB_COLOR] = _color_stream->getBuffer();

		if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	    strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 4;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)   strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 3;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_BYTE)	strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTubyte) * 4;
	}

	if (_texcoord_stream) {
        buffers[ShaderResource::ATTRIB_TEXCOORD0] = _texcoord_stream->getBuffer();
		
		if (_texcoord_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)		strides[ShaderResource::ATTRIB_TEXCOORD0] = sizeof(DTfloat) * 2;
		else if (_texcoord_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)strides[ShaderResource::ATTRIB_TEXCOORD0] = sizeof(DTfloat) * 3;
		else if (_texcoord_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)strides[ShaderResource::ATTRIB_TEXCOORD0] = sizeof(DTfloat) * 4;
	}

	if (_vertex_stream) {
        buffers[ShaderResource::ATTRIB_POSITION] = _vertex_stream->getBuffer();
		
		if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)		strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 3;
		else if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)	strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 2;
		else if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 4;
	}

    // Bind the streams
    context->IASetVertexBuffers(0,ShaderResource::NUM_ATTRIBS,buffers,strides,offsets);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->Draw(num*6,first*6);
}

//==============================================================================
//==============================================================================

} // DT2

