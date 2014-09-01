//==============================================================================
///	
///	File: DeviceGraphicsDX11Geometry.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Geometry.hpp"
#include "Assert.hpp"
#include "System.hpp"
#include "Archive.hpp"
#include "ArchiveObjectQueue.hpp"
#include "ArchiveProcessPointersRef.hpp"

#include "DeviceGraphics.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"

#include "DeviceGraphicsDX11IndexStream.hpp"
#include "DeviceGraphicsDX11DataStream.hpp"
#include "DeviceGraphicsDX11Shader.hpp"
#include "ShaderResource.hpp"

#include "DeviceConsole.hpp"
#include "Factory.hpp"
#include "Error.hpp"

#include "Vector.hpp"
#include "Texcoord.hpp"
#include "Triangle.hpp"

#include "GeometryResource.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Geometry)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Geometry::DeviceGraphicsDX11Geometry (void)
	:	_index_stream	(NULL),
		_color_stream	(NULL),
		_vertex_stream	(NULL),
		_normal_stream	(NULL),
        _weights_strength_stream    (NULL),
        _weights_index_stream       (NULL),
		_num_faces		(0),
		_num_vertices	(0),
		_size			(0)
{
	for (DTuint i = 0; i < ARRAY_SIZE(_texcoord_stream); ++i) {
		_texcoord_stream[i] = NULL;
	}
}
					
DeviceGraphicsDX11Geometry::~DeviceGraphicsDX11Geometry (void)
{
	RELEASE(_index_stream);
	RELEASE(_color_stream);
	RELEASE(_vertex_stream);
	RELEASE(_normal_stream);

    RELEASE(_weights_strength_stream);
    RELEASE(_weights_index_stream);

	for (DTuint i = 0; i < ARRAY_SIZE(_texcoord_stream); ++i) {
		RELEASE(_texcoord_stream[i]);
	}	
	
	_num_faces = 0;
	_num_vertices = 0;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Geometry::archiveRead (Archive *archive)
{

    archive->pushDomain (getClassID ());
    
	BaseClass::archiveRead(archive);

	// New style geometry
	archive->pushDomain ("Streams");		
	*archive << ARCHIVE_DATA(_num_faces, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_num_vertices, DATA_PERSISTENT);
	
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_index_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_color_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_vertex_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_normal_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_weights_strength_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_weights_index_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[0]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[1]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[2]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[3]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[4]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[5]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[6]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[7]));

	archive->popDomain ();
    archive->popDomain ();

}

void DeviceGraphicsDX11Geometry::archiveWrite (Archive *archive)
{
    archive->pushDomain (getClassID ());

	BaseClass::archiveWrite(archive);
	
	archive->pushDomain ("Streams");
		
	*archive << ARCHIVE_DATA(_num_faces, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_num_vertices, DATA_PERSISTENT);
				
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_index_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_color_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_vertex_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_normal_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_weights_strength_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_weights_index_stream));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[0]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[1]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[2]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[3]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[4]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[5]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[6]));
	archive->addPostProcess(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,_texcoord_stream[7]));

    archive->popDomain ();
    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Geometry::syncToResource		(GeometryResource *geometry)
{
	if (geometry->getRecacheData()) {
        std::size_t num_vertices = geometry->getVertexStream()->size();
		
		std::size_t num_faces;
		if (geometry->getIndicesStream())
			num_faces = geometry->getIndicesStream()->size();
		else
			num_faces = geometry->getVertexStream()->size() / 3;
			
		_num_faces = (DTuint) num_faces;
		_num_vertices = (DTuint) num_vertices;
		
		DeviceGraphicsDX11DataStream *vertices_stream = NULL;
		DeviceGraphicsDX11DataStream *normals_stream = NULL;
		DeviceGraphicsDX11DataStream *texcoord0_stream = NULL;
		DeviceGraphicsDX11DataStream *texcoord1_stream = NULL;
		DeviceGraphicsDX11DataStream *tangents_stream = NULL;
		DeviceGraphicsDX11DataStream *colors_stream = NULL;
		DeviceGraphicsDX11DataStream *weights_index_stream = NULL;
		DeviceGraphicsDX11DataStream *weights_strength_stream = NULL;
		DeviceGraphicsDX11IndexStream *faces_stream = NULL;
		
		Vector3*		vertices = NULL;
		Vector3*		normals = NULL;
		Texcoord2*		texcoord0 = NULL;
		Texcoord2*		texcoord1 = NULL;
		Vector3*		tangents = NULL;
		Color*			colors = NULL;
		WeightsIndex*	weights_index = NULL;
		Texcoord4*		weights_strength = NULL;
		Triangle*		faces = NULL;

		if (geometry->getVertexStream()) {
			vertices_stream = DeviceGraphicsDX11DataStream::create();
			vertices = (Vector3*) vertices_stream->allocateData(sizeof(Vector3) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_3_FLOAT, true);
			setVertexStream(vertices_stream);
			_size += sizeof(Vector3) * _num_vertices;
		}

		if (geometry->getNormalsStream()) {
			normals_stream = DeviceGraphicsDX11DataStream::create();
			normals = (Vector3*) normals_stream->allocateData(sizeof(Vector3) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_3_FLOAT, false);
			setNormalStream(normals_stream);
			_size += sizeof(Vector3) * _num_vertices;
		}
		
		if (geometry->getUVsStream0()) {
			texcoord0_stream = DeviceGraphicsDX11DataStream::create();
			texcoord0 = (Texcoord2*) texcoord0_stream->allocateData(sizeof(Texcoord2) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_2_FLOAT, false);
			setTexcoordStream(0, texcoord0_stream);
			_size += sizeof(Texcoord2) * _num_vertices;
		}
		
		if (geometry->getUVsStream1()) {
			texcoord1_stream = DeviceGraphicsDX11DataStream::create();
			texcoord1 = (Texcoord2*) texcoord1_stream->allocateData(sizeof(Texcoord2) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_2_FLOAT, false);
			setTexcoordStream(1, texcoord1_stream);
			_size += sizeof(Texcoord2) * _num_vertices;
		}

		if (geometry->getTangentsStream()) {
			tangents_stream = DeviceGraphicsDX11DataStream::create();
			tangents = (Vector3*) tangents_stream->allocateData(sizeof(Vector3) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_3_FLOAT, false);
			setTexcoordStream(2, tangents_stream);
			_size += sizeof(Vector3) * _num_vertices;
		}
		
		if (geometry->getColorsStream()) {
			colors_stream = DeviceGraphicsDX11DataStream::create();
			colors = (Color*) colors_stream->allocateData(sizeof(Color) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_4_BYTE, false);
			setColorsStream(colors_stream);
			_size += sizeof(Color) * _num_vertices;
		}

		if (geometry->getWeightsIndexStream()) {
			weights_index_stream = DeviceGraphicsDX11DataStream::create();
			weights_index = (WeightsIndex*) weights_index_stream->allocateData(sizeof(WeightsIndex) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_4_USHORT, false);
			setWeightsIndexStream(weights_index_stream);
			_size += sizeof(Texcoord4) * _num_vertices;
		}

		if (geometry->getWeightsStrengthStream()) {
			weights_strength_stream = DeviceGraphicsDX11DataStream::create();
			weights_strength = (Texcoord4*) weights_strength_stream->allocateData(sizeof(Texcoord4) * _num_vertices, DeviceGraphicsDX11DataStream::FMT_4_FLOAT, false);
			setWeightsStrengthStream(weights_strength_stream);
			_size += sizeof(Texcoord4) * _num_vertices;
		}

		if (geometry->getIndicesStream()) {
			faces_stream = DeviceGraphicsDX11IndexStream::create();
			faces = (Triangle*) faces_stream->allocateData(sizeof(Triangle) * _num_faces, false);
			setIndexStream(faces_stream);
			_size += sizeof(Triangle) * _num_faces;
		}
		
		
		// copy data
		for (DTuint v = 0; v < num_vertices; ++v) {
			if (vertices)			vertices[v] = geometry->getVertexStream()->get(v);
			if (normals)			normals[v] = geometry->getNormalsStream()->get(v);
			if (texcoord0)			texcoord0[v] = geometry->getUVsStream0()->get(v);
			if (texcoord1)			texcoord1[v] = geometry->getUVsStream1()->get(v);				
			if (tangents)			tangents[v] = geometry->getTangentsStream()->get(v);				
			if (colors)				colors[v] = geometry->getColorsStream()->get(v);				
			if (weights_index)		weights_index[v] = geometry->getWeightsIndexStream()->get(v);	
			if (weights_strength)	weights_strength[v] = geometry->getWeightsStrengthStream()->get(v);	
		}
		
		// copy FaceTri data
		for (DTuint f = 0; f < num_faces; ++f) {
			if (faces)				faces[f] = geometry->getIndicesStream()->get(f);	
		}

		if (vertices_stream)			vertices_stream->uploadData();
		if (normals_stream)				normals_stream->uploadData();
		if (texcoord0_stream)			texcoord0_stream->uploadData();
		if (texcoord1_stream)			texcoord1_stream->uploadData();
		if (tangents_stream)			tangents_stream->uploadData();
		if (colors_stream)				colors_stream->uploadData();
		if (weights_index_stream)		weights_index_stream->uploadData();
		if (weights_strength_stream)	weights_strength_stream->uploadData();
		if (faces_stream)				faces_stream->uploadData();
		
		RELEASE(vertices_stream);
		RELEASE(normals_stream);
		RELEASE(texcoord0_stream);
		RELEASE(texcoord1_stream);
		RELEASE(tangents_stream);
		RELEASE(colors_stream);
		RELEASE(weights_index_stream);
		RELEASE(weights_strength_stream);
		RELEASE(faces_stream);				
		
		geometry->setRecacheData(false);
	}
	
	if (geometry->getRecacheParameters()) {
		geometry->setRecacheParameters(false);
	}
	
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Geometry::setIndexStream	(DeviceGraphicsDX11IndexStream *stream)
{
	SAFE_ASSIGN(_index_stream, stream);
}

void DeviceGraphicsDX11Geometry::setColorsStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_color_stream, stream);
}

void DeviceGraphicsDX11Geometry::setVertexStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_vertex_stream, stream);
}

void DeviceGraphicsDX11Geometry::setNormalStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_normal_stream, stream);
}

void DeviceGraphicsDX11Geometry::setWeightsStrengthStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_weights_strength_stream, stream);
}

void DeviceGraphicsDX11Geometry::setWeightsIndexStream	(DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_weights_index_stream, stream);
}

void DeviceGraphicsDX11Geometry::setTexcoordStream	(DTushort tex_unit, DeviceGraphicsDX11DataStream *stream)
{
	SAFE_ASSIGN(_texcoord_stream[tex_unit], stream);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Geometry::draw (void)
{
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11DeviceContext1 *context = renderer->getD3D11Context();


    ID3D11Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
    UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
    UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

	// If there are VBO's
	if (_normal_stream) {
        buffers[ShaderResource::ATTRIB_NORMAL] = _normal_stream->getBuffer();
        		
		if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)		strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 3;
		else if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)	strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 2;
		else if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 4;
    }	
	
	if (_color_stream) {
        buffers[ShaderResource::ATTRIB_COLOR] = _color_stream->getBuffer();

		if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	    strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 4;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)   strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 3;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_BYTE)	strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTubyte) * 4;
	}
        
	if (_weights_strength_stream) {
        buffers[ShaderResource::ATTRIB_WEIGHTS_STRENGTH] = _weights_strength_stream->getBuffer();

		if (_weights_strength_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	    strides[ShaderResource::ATTRIB_WEIGHTS_STRENGTH] = sizeof(DTfloat) * 4;
	}

	if (_weights_index_stream) {
        buffers[ShaderResource::ATTRIB_WEIGHTS_INDEX] = _weights_index_stream->getBuffer();
		
        if (_weights_index_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_USHORT)	    strides[ShaderResource::ATTRIB_WEIGHTS_INDEX] = sizeof(DTushort) * 4;
	}

	DTint num_units = min2(4,System::getRenderer()->getNumTextureUnits());
	for (DTint unit = 0; unit < num_units; ++unit) {
			
		if (_texcoord_stream[unit]) {
            buffers[ShaderResource::ATTRIB_TEXCOORD0+unit] = _texcoord_stream[unit]->getBuffer();
						
			if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)		strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 2;
			else if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)	strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 3;
			else if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 4;
		}
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


	// draw if there is an index stream
	if (_index_stream) {
        context->IASetIndexBuffer(_index_stream->getBuffer(), DXGI_FORMAT_R16_UINT, 0);
        context->DrawIndexed(_num_faces*3,0,0);
	}
}

void DeviceGraphicsDX11Geometry::drawRange	(const DTuint first_tri, const DTuint num)
{
	// If there are VBO's
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

    ID3D11Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
    UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
    UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

	// If there are VBO's
	if (_normal_stream) {
        buffers[ShaderResource::ATTRIB_NORMAL] = _normal_stream->getBuffer();
        		
		if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)		strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 3;
		else if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)	strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 2;
		else if (_normal_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_NORMAL] = sizeof(DTfloat) * 4;
    }
	
	if (_color_stream) {
        buffers[ShaderResource::ATTRIB_COLOR] = _color_stream->getBuffer();

		if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	    strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 4;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)   strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTfloat) * 3;
		else if (_color_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_BYTE)	strides[ShaderResource::ATTRIB_COLOR] = sizeof(DTubyte) * 4;
	}
        
	if (_weights_strength_stream) {
        buffers[ShaderResource::ATTRIB_WEIGHTS_STRENGTH] = _weights_strength_stream->getBuffer();

		if (_weights_strength_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	    strides[ShaderResource::ATTRIB_WEIGHTS_STRENGTH] = sizeof(DTfloat) * 4;
	}

	if (_weights_index_stream) {
        buffers[ShaderResource::ATTRIB_WEIGHTS_INDEX] = _weights_index_stream->getBuffer();
		
        if (_weights_index_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_USHORT)	    strides[ShaderResource::ATTRIB_WEIGHTS_INDEX] = sizeof(DTushort) * 4;
	}

	DTint num_units = min2(4,System::getRenderer()->getNumTextureUnits());
	for (DTint unit = 0; unit < num_units; ++unit) {
			
		if (_texcoord_stream[unit]) {
            buffers[ShaderResource::ATTRIB_TEXCOORD0+unit] = _texcoord_stream[unit]->getBuffer();
						
			if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)		strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 2;
			else if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)	strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 3;
			else if (_texcoord_stream[unit]->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_TEXCOORD0+unit] = sizeof(DTfloat) * 4;
		}
	}


	if (_vertex_stream) {
        buffers[ShaderResource::ATTRIB_POSITION] = _vertex_stream->getBuffer();
		
		if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_3_FLOAT)		strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 3;
		else if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_2_FLOAT)	strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 2;
		else if (_vertex_stream->getFormat() == DeviceGraphicsDX11DataStream::FMT_4_FLOAT)	strides[ShaderResource::ATTRIB_POSITION] = sizeof(DTfloat) * 4;
	}
	
    // Bind the streams
    context->IASetVertexBuffers(0,ShaderResource::NUM_ATTRIBS,buffers,strides,offsets);

	// draw if there is an index stream
	if (_index_stream) {
        context->IASetIndexBuffer(_index_stream->getBuffer(), DXGI_FORMAT_R16_UINT, 0);
        context->DrawIndexed(num*3,first_tri*3,0);
	}
	
}

//==============================================================================
//==============================================================================

Vector3 * DeviceGraphicsDX11Geometry::getVertices (void)
{
	return _vertex_stream ? (Vector3*) _vertex_stream->getData() : NULL;
}

Vector3 * DeviceGraphicsDX11Geometry::getNormals (void)
{
	return _normal_stream ? (Vector3*) _normal_stream->getData() : NULL;
}

Vector3 * DeviceGraphicsDX11Geometry::getTangents (void)
{
	return _texcoord_stream[2] ? (Vector3*) _texcoord_stream[2]->getData() : NULL;
}

Texcoord2 * DeviceGraphicsDX11Geometry::getTexcoords1 (void)
{
	return _texcoord_stream[1] ? (Texcoord2*) _texcoord_stream[1]->getData() : NULL;
}

Texcoord2 * DeviceGraphicsDX11Geometry::getTexcoords0 (void)
{
	return _texcoord_stream[0] ? (Texcoord2*) _texcoord_stream[0]->getData() : NULL;
}

Triangle * DeviceGraphicsDX11Geometry::getFaces (void)
{
	return _index_stream ? (Triangle*) _index_stream->getData() : NULL;
}

Texcoord4* DeviceGraphicsDX11Geometry::getWeightsStrength (void)
{
	return _weights_strength_stream ? (Texcoord4*) _weights_strength_stream->getData() : NULL;
}

WeightsIndex * DeviceGraphicsDX11Geometry::getWeightsIndex (void)
{
	return _weights_index_stream ? (WeightsIndex*) _weights_index_stream->getData() : NULL;
}

//==============================================================================
//==============================================================================

} // DT2

