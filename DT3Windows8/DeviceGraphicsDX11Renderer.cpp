//==============================================================================
///	
///	File: DeviceGraphicsDX11Renderer.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Renderer.hpp"
#include "DeviceConsole.hpp"
#include "ResourceManager.hpp"
#include "System.hpp"

#include "Factory.hpp"

#include "CameraObject.hpp"

#include "ParticlesResource.hpp"
#include "GeometryResource.hpp"
#include "TextureResource.hpp"
#include "MaterialResource.hpp"
#include "FragmentShaderResource.hpp"
#include "VertexShaderResource.hpp"
#include "GeometryShaderResource.hpp"
#include "ShaderResource.hpp"
#include "DrawBatcher.hpp"
#include "Framebuffer.hpp"

#include <algorithm>
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Renderer)

//==============================================================================
//==============================================================================

DeviceGraphicsDX11Renderer::DeviceGraphicsDX11Renderer (void)
	:	_width              (0),
		_height             (0),
		_aspect             (1.0F),
		_num_tex_units      (0),
		_max_texture_size   (0),
		_has_cube_maps      (false),
        _stencil_ref        (0),
		_tick_accum         (0.0F),
        _clear_color        (0.0F,0.0F,0.0F,0.0F),
        _clear_depth        (1.0F),
        _clear_stencil      (0),
        _modelview_uniform  (NULL),
        _projection_uniform (NULL),
        _camera_uniform     (NULL),
        _num_elements       (0),
        _vertices           (NULL),
        _normals            (NULL),
        _texcoords1         (NULL),
        _texcoords2         (NULL),
        _colors             (NULL),
        _current_render_target_view(NULL),
        _current_depth_stencil_view(NULL)
{

}

DeviceGraphicsDX11Renderer::~DeviceGraphicsDX11Renderer (void)
{
	closeDisplay();

    SAFE_RELEASE(_vertices);
    SAFE_RELEASE(_normals);
    SAFE_RELEASE(_texcoords1);
    SAFE_RELEASE(_texcoords2);
    SAFE_RELEASE(_colors);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::getDisplayModes (std::vector<DisplayMode> &modes)
{
	modes.clear();
	
	// add the current mode to the list which is the only one
	DisplayMode m;
			
	m.setWidth(_width);
	m.setHeight(_height);
	modes.push_back(m);	
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::openDisplay			(DTuint width, DTuint height, void *data)
{
	//LOG_MESSAGE << "Vendor:     " << //::glGetString(GL_VENDOR);
	//LOG_MESSAGE << "Renderer:   " << //::glGetString(GL_RENDERER);
	//LOG_MESSAGE << "Version:    " << //::glGetString(GL_VERSION);
	//LOG_MESSAGE << "Extensions: " << //::glGetString(GL_EXTENSIONS);

    // Extract all of the extra data
    OpenDisplayData *odata = reinterpret_cast<OpenDisplayData*>(data);

    _d3dDevice = odata->d3dDevice;
    _d3dContext = odata->d3dContext;
    _swapChain = odata->swapChain;
    _renderTargetView = odata->renderTargetView;
    _depthStencilView = odata->depthStencilView;
    _swapCallback = odata->swapCallback;

    _current_render_target_view = _renderTargetView.Get();
	_current_depth_stencil_view = _depthStencilView.Get();  

	getRenderParameters();
    
	changeDisplay(width,height);
    DeviceGraphics::openDisplay(width, height, data);


}

void DeviceGraphicsDX11Renderer::getRenderParameters   (void)
{
	//// get gl extensions
	//DTcharacter ext[1024 * 16];
	//strcpy(ext, (DTcharacter *) //::glGetString(GL_EXTENSIONS));
	//
 //   // Fill with gl extensions
 //   DTcharacter *tok = ::strtok(ext," ");

 //   while (tok) {       
	//	_extensions.push_back(tok);
 //       tok = ::strtok(NULL," ");
 //   };
 //   
	//
 //   // read system parameters
	//_num_tex_units = 4; // TODO What should be here?	glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*) &_num_tex_units);
	//glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*) &_max_texture_size);
	//	
 //   // Query openGL extensions
 //   _has_cube_maps =			hasExtension("GL_ARB_texture_cube_map");    
}

void DeviceGraphicsDX11Renderer::changeDisplay			(DTuint width, DTuint height)
{	
	// width and height are swapped because of landscape mode
	_width = width;
	_height = height;
	_aspect = (DTfloat) _width / (DTfloat) _height;

	setViewport (0, 0, width, height);
	
    // clear out all of the buffers
    glClearColor(Color(0.0F,0.0F,0.0F,0.0F)); 
    glClearDepth(1.0f);
	   
    DeviceGraphics::changeDisplay(width,height);
}

void DeviceGraphicsDX11Renderer::closeDisplay			(void)
{
    DeviceGraphics::closeDisplay();
}

void DeviceGraphicsDX11Renderer::swapDisplay			(void)
{
    // Fire the callback
    (*_swapCallback)();
}

//==============================================================================
//==============================================================================

DTuint DeviceGraphicsDX11Renderer::getScreenWidth		(void)
{
	return _width;
}

DTuint DeviceGraphicsDX11Renderer::getScreenHeight		(void)
{
	return _height;
}

DTfloat DeviceGraphicsDX11Renderer::getScreenAspect		(void)
{
	return _aspect;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::getViewport (DTint &x, DTint &y, DTint &width, DTint &height)
{
	x = _viewport_x;
	y = _viewport_y;
	width = _viewport_width;
	height = _viewport_height;
}
//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::setViewport (DTint x, DTint y, DTint width, DTint height, DTint rt_width, DTint rt_height)
{
    if (rt_width == 0)   rt_width = _width;
    if (rt_height == 0)  rt_height = _height;

    // Calculate the viewport
    D3D11_VIEWPORT viewport;

    viewport.TopLeftX = x;
    viewport.TopLeftY = rt_height - y - height;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = D3D11_MIN_DEPTH;
    viewport.MaxDepth = D3D11_MAX_DEPTH;

    _d3dContext->RSSetViewports(1,&viewport);

	_viewport_x = x;
	_viewport_y = y;
	_viewport_width = width;
	_viewport_height = height;
	
	_viewport_aspect = (DTfloat) _viewport_width / (DTfloat) _viewport_height;
}

//==============================================================================
//==============================================================================

DTuint DeviceGraphicsDX11Renderer::getViewportWidth		(void)
{
	return _viewport_width;
}

DTuint DeviceGraphicsDX11Renderer::getViewportHeight	(void)
{
	return _viewport_height;
}

DTfloat DeviceGraphicsDX11Renderer::getViewportAspect	(void)
{
	return _viewport_aspect;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::activateFramebuffer (Framebuffer *fb)
{
    if (fb) {
        DeviceGraphicsDX11Framebuffer*	fb_ptr = getFramebufferCached (fb);

        _current_render_target_view = fb_ptr->getRenderTargetView();
		_current_depth_stencil_view = fb_ptr->getDepthStencilView();
    } else {
        _current_render_target_view = _renderTargetView.Get();
		_current_depth_stencil_view = _depthStencilView.Get();  
    }

    // Set render targets
    _d3dContext->OMSetRenderTargets(1, &_current_render_target_view, _current_depth_stencil_view);
}

//==============================================================================
///	Queries
//==============================================================================
 
DTint DeviceGraphicsDX11Renderer::getNumTextureUnits	(void)
{
	return 8;   // TODO: make this dynamic
}

DTint DeviceGraphicsDX11Renderer::getMaxTextureSize	(void)
{
	return 2048;
}

DTboolean DeviceGraphicsDX11Renderer::hasCubeMaps			(void)
{
	return false;
}

//==============================================================================
/// Things that the materials don't handle
//==============================================================================

void DeviceGraphicsDX11Renderer::glClearColor			(const Color &c)
{
    _clear_color = c;
}

void DeviceGraphicsDX11Renderer::glClearDepth			(DTfloat d)
{
	_clear_depth = d;
}

void DeviceGraphicsDX11Renderer::glClearStencil		(DTint s)
{
	_clear_stencil = s;
}

void DeviceGraphicsDX11Renderer::glClear               (DTboolean depth, DTboolean color, DTboolean stencil)
{

    if (color) {
	    const float c[] = { _clear_color.getRFloat(), 
                            _clear_color.getGFloat(),
                            _clear_color.getBFloat(), 
                            _clear_color.getAFloat() };

	    _d3dContext->ClearRenderTargetView(
		    _current_render_target_view,
		    c
		    );
    }

    if (depth | stencil) {
	    _d3dContext->ClearDepthStencilView(
		    _current_depth_stencil_view,
		    D3D11_CLEAR_DEPTH * (depth ? 1:0) | D3D11_CLEAR_STENCIL * (stencil ? 1:0),
		    _clear_depth,
		    _clear_stencil
		    );
    }
}

void DeviceGraphicsDX11Renderer::glShadeModel			(DTuint s)
{
	//::glShadeModel(s);
}

void DeviceGraphicsDX11Renderer::glPolygonOffset				(DTfloat factor, DTfloat units)
{
    //::glPolygonOffset(factor,units);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::glScissor (DTint x, DTint y, DTint width, DTint height)
{
	if (x == 0 && y == 0 && width == _width && _height == _height) {
		//::glDisable(GL_SCISSOR_TEST);
	} else {
		//::glEnable(GL_SCISSOR_TEST);
        //::glScissor(x,y,width,height);
	}
}

//==============================================================================
//==============================================================================

DTboolean DeviceGraphicsDX11Renderer::hasExtension			(const String ext)
{
	if (std::find(_extensions.begin(), _extensions.end(), ext) != _extensions.end())
		return true;
	else
		return false;
}
		
//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::tick (const DTfloat dt)
{

	//// Only tick at 1/5 FPS
	//const DTfloat TICK_PERIOD = 10.0F;
	//_tick_accum += dt;
	//if (_tick_accum > TICK_PERIOD) {
	//	_tick_accum = 0.0F;

	//	_particles_cache.removeOldItems(15.0F);			// delete if not used for 15 seconds
	//	_geometry_cache.removeOldItems(15.0F);			// delete if not used for 15 seconds
	//	_texture_cache.removeOldItems(15.0F);				// delete if not used for 15 seconds
	//	_material_cache.removeOldItems(15.0F);			// delete if not used for 15 seconds
	//	_pixel_shader_cache.removeOldItems(15.0F);	// delete if not used for 15 seconds
	//	_vertex_shader_cache.removeOldItems(15.0F);		// delete if not used for 15 seconds
	//	_framebuffer_cache.removeOldItems(15.0F);		// delete if not used for 15 seconds

	//	LOG_MESSAGE << "Geometry size in kB: " << getGeometrySize()/1024;
	//	LOG_MESSAGE << "Textures size in kB: " << getTexturesSize()/1024;
	//}
}
		
//==============================================================================
//==============================================================================

DeviceGraphicsDX11Particles* DeviceGraphicsDX11Renderer::getParticlesCached (ParticlesResource *particles)
{
    particles->lock();

    DeviceGraphicsDX11Particles* par_ptr = _particles_cache.get(particles->uniqueID());
	
	// Found in hash table
    if (!par_ptr) {
		DeviceGraphicsDX11Particles *new_par_ptr = DeviceGraphicsDX11Particles::create();
		_particles_cache.add(particles->uniqueID(), new_par_ptr);	// Keep Ref count at 1
        particles->setRecacheData(true);
        particles->setRecacheParameters(true);
		
		par_ptr = new_par_ptr;
		RELEASE(new_par_ptr);
	}
	
	par_ptr->syncToResource(particles);

    particles->unlock();
	return par_ptr;
}

DeviceGraphicsDX11Geometry* DeviceGraphicsDX11Renderer::getGeometryCached (GeometryResource *geometry)
{
    geometry->lock();

    DeviceGraphicsDX11Geometry* geo_ptr = _geometry_cache.get(geometry->uniqueID());
	
	// Found in hash table
    if (!geo_ptr) {
		DeviceGraphicsDX11Geometry *new_geo_ptr = DeviceGraphicsDX11Geometry::create();
		_geometry_cache.add(geometry->uniqueID(), new_geo_ptr);	// Keep Ref count at 1
        geometry->setRecacheData(true);
        geometry->setRecacheParameters(true);
		
		geo_ptr = new_geo_ptr;
		RELEASE(new_geo_ptr);
    }
	
	geo_ptr->syncToResource(geometry);

    geometry->unlock();
	return geo_ptr;
}

DeviceGraphicsDX11Texture* DeviceGraphicsDX11Renderer::getTextureCached (TextureResource *texture)
{
    texture->lock();

    DeviceGraphicsDX11Texture* tex_ptr = _texture_cache.get(texture->uniqueID());
	
	// Found in hash table
    if (!tex_ptr) {
        DeviceGraphicsDX11Texture *new_tex_ptr = DeviceGraphicsDX11Texture::create();
		_texture_cache.add(texture->uniqueID(), new_tex_ptr, texture->isResident());	// Keep Ref count at 1
        texture->setRecacheData(true);
        texture->setRecacheParameters(true);

		tex_ptr = new_tex_ptr;
		RELEASE(new_tex_ptr);
    }
	
	tex_ptr->syncToResource(texture);

    texture->unlock();
	return tex_ptr;
}

DeviceGraphicsDX11Material* DeviceGraphicsDX11Renderer::getMaterialCached (MaterialResource *material)
{
    material->lock();

    DeviceGraphicsDX11Material* mat_ptr = _material_cache.get(material->uniqueID());
	
	// Found in hash table
    if (!mat_ptr) {
        DeviceGraphicsDX11Material *new_mat_ptr = DeviceGraphicsDX11Material::create();
		_material_cache.add(material->uniqueID(), new_mat_ptr);	// Keep Ref count at 1
        material->setRecacheData(true);
        material->setRecacheParameters(true);

		mat_ptr = new_mat_ptr;
		RELEASE(new_mat_ptr);
    }
	
	mat_ptr->syncToResource(material);

    material->unlock();
	return mat_ptr;
}

DeviceGraphicsDX11PixelShader*	DeviceGraphicsDX11Renderer::getPixelShaderCached	(FragmentShaderResource *shader)
{
    shader->lock();

    DeviceGraphicsDX11PixelShader* prog_ptr = _pixel_shader_cache.get(shader->uniqueID());
	
	// Found in hash table
    if (!prog_ptr) {
        DeviceGraphicsDX11PixelShader *new_prog_ptr = DeviceGraphicsDX11PixelShader::create();
		_pixel_shader_cache.add(shader->uniqueID(), new_prog_ptr);	// Keep Ref count at 1
        shader->setRecacheData(true);
        shader->setRecacheParameters(true);
		
		prog_ptr = new_prog_ptr;
		RELEASE(new_prog_ptr);
    }
	
	prog_ptr->syncToResource(shader);

    shader->unlock();
	return prog_ptr;
}

DeviceGraphicsDX11VertexShader*	DeviceGraphicsDX11Renderer::getVertexShaderCached	(VertexShaderResource *shader)
{
    shader->lock();
    DeviceGraphicsDX11VertexShader* prog_ptr = _vertex_shader_cache.get(shader->uniqueID());
	
	// Found in hash table
    if (!prog_ptr) {
        DeviceGraphicsDX11VertexShader *new_prog_ptr = DeviceGraphicsDX11VertexShader::create();
		_vertex_shader_cache.add(shader->uniqueID(), new_prog_ptr);	// Keep Ref count at 1
        shader->setRecacheData(true);
        shader->setRecacheParameters(true);

		prog_ptr = new_prog_ptr;
		RELEASE(new_prog_ptr);
	}
	
	prog_ptr->syncToResource(shader);
    
    shader->unlock();
	return prog_ptr;
}

DeviceGraphicsDX11Shader*	DeviceGraphicsDX11Renderer::getShaderCached	(ShaderResource *shader)
{
    shader->lock();

    DeviceGraphicsDX11Shader* prog_ptr = _shader_cache.get(shader->uniqueID());
	
	// Found in hash table
    if (!prog_ptr) {
        DeviceGraphicsDX11Shader *new_prog_ptr = DeviceGraphicsDX11Shader::create();
		_shader_cache.add(shader->uniqueID(), new_prog_ptr);	// Keep Ref count at 1
        shader->setRecacheData(true);
        shader->setRecacheParameters(true);

		prog_ptr = new_prog_ptr;
		RELEASE(new_prog_ptr);
	}
	
	prog_ptr->syncToResource(shader);

    shader->unlock();
	return prog_ptr;
}

//==============================================================================
//==============================================================================

DeviceGraphicsDX11Framebuffer*	DeviceGraphicsDX11Renderer::getFramebufferCached	(Framebuffer *fb)
{
    DeviceGraphicsDX11Framebuffer* fb_ptr = _framebuffer_cache.get(fb->uniqueID());
	
	// Found in hash table
    if (!fb_ptr) {
        DeviceGraphicsDX11Framebuffer *new_fb_ptr = DeviceGraphicsDX11Framebuffer::create();
        new_fb_ptr->buildFramebuffer(fb);
        
		_framebuffer_cache.add(fb->uniqueID(), new_fb_ptr);	// Keep Ref count at 1

		fb_ptr = new_fb_ptr;
		RELEASE(new_fb_ptr);
	}
	
	return fb_ptr;
}

//==============================================================================
//==============================================================================

// Helper object to get the size of all of the textures in the cache
struct totalTextureSize {
	totalTextureSize()	{	size = 0;	}
	void operator()(const DeviceGraphicsDX11Texture *texture) {
		size += texture->size();
	}
	
	DTuint size;
};

DTuint DeviceGraphicsDX11Renderer::getTexturesSize(void)
{
	totalTextureSize total_size;
	_texture_cache.transform(total_size);
	return total_size.size;
}


// Helper object to get the size of all of the geometry in the cache
struct totalGeometrySize {
	totalGeometrySize()	{	size = 0;	}
	void operator()(const DeviceGraphicsDX11Geometry *geometry) {
		size += geometry->size();
	}
	
	DTuint size;
};

DTuint DeviceGraphicsDX11Renderer::getGeometrySize(void)
{
	totalGeometrySize total_size;
	_geometry_cache.transform(total_size);
	return total_size.size;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::activateCamera (CameraObject *camera)
{
	camera->activated();
    camera->calculateFrustum();

    Matrix4 ogl2d3d (   1.0F,   0.0F,   0.0F,   0.0F,
                        0.0F,   1.0F,   0.0F,   0.0F,
                        0.0F,   0.0F,   0.5F,   0.5F,
                        0.0F,   0.0F,   0.0F,   1.0F    );
	
	_projection = ogl2d3d * camera->getProjection();
	_camera = camera->getTransform();
    _modelview = camera->getModelview();

    DeviceGraphicsDX11Shader::setConstantValue(_modelview_uniform, (DTubyte *) &_modelview, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_projection_uniform, (DTubyte *) &_projection, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_camera_uniform, (DTubyte *) &_camera, sizeof(Matrix4) );
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::drawBatch (	const Matrix4 &transform,
										GeometryResource *geometry, 
										MaterialResource *material,
                                        Array<Matrix4> *skeleton)
{
	if (geometry) {
    
        if (material)
            activateMaterial(material);

		Matrix4 t = _modelview * transform;
		//::glUniformMatrix4fv(_modelview_uniform, 1, GL_FALSE, (GLfloat*) &t);
		//::glUniformMatrix4fv(_projection_uniform, 1, GL_FALSE, (GLfloat*) &_projection);
		//::glUniformMatrix4fv(_camera_uniform, 1, GL_FALSE, (GLfloat*) &_camera);
        	
        Array<GeometryResource::Batch> &batches = geometry->getBatches();

        DeviceGraphicsDX11Geometry *ogl_geometry = getGeometryCached(geometry);
        for (DTuint i = 0; i < batches.size(); ++i) {
            GeometryResource::Batch &b = batches[i];
            
            // Upload skeleton
            if (skeleton) {
                Array<Matrix4> transforms;
                transforms.resize(b.weight_to_joint.size());
            
                for (DTuint i = 0; i < b.weight_to_joint.size(); ++i) {
                    transforms[i] = (*skeleton)[b.weight_to_joint[i]];
                }
                
                DeviceGraphicsDX11Shader *shader = getShaderCached(material->getShader());
                //DTint skeleton_uniform = shader->getUniform(DeviceGraphicsDX11Shader::UNIFORM_SKELETON);
                //if (skeleton_uniform != -1)
                    //::glUniformMatrix4fv(skeleton_uniform, transforms.size(), GL_FALSE, (GLfloat*) &transforms[0]);
            }
            
            ogl_geometry->drawRange(b.first_index, b.num_indices);
        }

	}
}      
															
//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::drawParticles (	const Matrix4 &transform,
										ParticlesResource *particles,
										MaterialResource *material,
										DTuint first, DTuint num	)
{
	if (material && particles) {
		activateMaterial(material);

		Matrix4 t = _modelview * transform;
        DeviceGraphicsDX11Shader::setConstantValue(_modelview_uniform, (DTubyte *) &t, sizeof(Matrix4) );
        DeviceGraphicsDX11Shader::setConstantValue(_projection_uniform, (DTubyte *) &_projection, sizeof(Matrix4) );
        DeviceGraphicsDX11Shader::setConstantValue(_camera_uniform, (DTubyte *) &_camera, sizeof(Matrix4) );
         
        DeviceGraphicsDX11Particles *ogl_particles = getParticlesCached(particles);
        ogl_particles->drawRange(first, num);
	}
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::reallocBuffers (DTuint num)
{
    if (num <= _num_elements)
        return;

    SAFE_RELEASE(_vertices);
    SAFE_RELEASE(_normals);
    SAFE_RELEASE(_texcoords1);
    SAFE_RELEASE(_texcoords2);
    SAFE_RELEASE(_colors);

    HRESULT hr;
    D3D11_BUFFER_DESC desc;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // Create the buffer
    desc.ByteWidth = sizeof(Vector3) * num;
    hr = _d3dDevice->CreateBuffer(&desc, NULL, &_vertices);
    Assert(SUCCEEDED(hr));

    desc.ByteWidth = sizeof(Vector3) * num;
    hr = _d3dDevice->CreateBuffer(&desc, NULL, &_normals);
    Assert(SUCCEEDED(hr));

    desc.ByteWidth = sizeof(Texcoord2) * num;
    hr = _d3dDevice->CreateBuffer(&desc, NULL, &_texcoords1);
    Assert(SUCCEEDED(hr));

    desc.ByteWidth = sizeof(Texcoord2) * num;
    hr = _d3dDevice->CreateBuffer(&desc, NULL, &_texcoords2);
    Assert(SUCCEEDED(hr));

    desc.ByteWidth = sizeof(Color) * num;
    hr = _d3dDevice->CreateBuffer(&desc, NULL, &_colors);
    Assert(SUCCEEDED(hr));

    _num_elements = num;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::drawRawBatch	(	const Matrix4 &transform, 
										Vector3 *v, 
										Vector3 *n, 
										Texcoord2 *t1, 
										Texcoord2 *t2, 
										Color *c,
										DTuint num,
										MaterialResource *material,
										BatchType batch_type)
{
	if (material) 
		activateMaterial(material);

	Matrix4 t = _modelview * transform;
    DeviceGraphicsDX11Shader::setConstantValue(_modelview_uniform, (DTubyte *) &t, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_projection_uniform, (DTubyte *) &_projection, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_camera_uniform, (DTubyte *) &_camera, sizeof(Matrix4) );
	
    reallocBuffers(num);

    UINT startSlot = 0;
    UINT numBuffers = 0;

    ID3D11Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
    UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
    UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

    D3D11_MAPPED_SUBRESOURCE res;

    _d3dContext->Map(_vertices,0,D3D11_MAP_WRITE_DISCARD,0,&res);
    ::memcpy(res.pData,v,sizeof(Vector3)*num);
    _d3dContext->Unmap(_vertices,0);

    buffers[ShaderResource::ATTRIB_POSITION] = _vertices;
    strides[ShaderResource::ATTRIB_POSITION] = sizeof(Vector3);

	
	if (n) {
        _d3dContext->Map(_normals,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,n,sizeof(Vector3)*num);
        _d3dContext->Unmap(_normals,0);

        buffers[ShaderResource::ATTRIB_NORMAL] = _normals;
        strides[ShaderResource::ATTRIB_NORMAL] = sizeof(Vector3);
	}

	if (t1) {
        _d3dContext->Map(_texcoords1,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,t1,sizeof(Texcoord2)*num);
        _d3dContext->Unmap(_texcoords1,0);

        buffers[ShaderResource::ATTRIB_TEXCOORD0] = _texcoords1;
        strides[ShaderResource::ATTRIB_TEXCOORD0] = sizeof(Texcoord2);
	}
	
	if (t2) {
        _d3dContext->Map(_texcoords2,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,t2,sizeof(Texcoord2)*num);
        _d3dContext->Unmap(_texcoords2,0);

        buffers[ShaderResource::ATTRIB_TEXCOORD1] = _texcoords2;
        strides[ShaderResource::ATTRIB_TEXCOORD1] = sizeof(Texcoord2);
	}
	
	if (c) {
        _d3dContext->Map(_colors,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,c,sizeof(Color)*num);
        _d3dContext->Unmap(_colors,0);

        buffers[ShaderResource::ATTRIB_COLOR] = _colors;
        strides[ShaderResource::ATTRIB_COLOR] = sizeof(Color);
	}

    // Bind buffers
    _d3dContext->IASetVertexBuffers(0,ShaderResource::NUM_ATTRIBS,buffers,strides,offsets);
	
	// draw primitives
    D3D11_PRIMITIVE_TOPOLOGY primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (batch_type == BATCH_TRI_STRIP) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	} else if (batch_type == BATCH_TRI_POLY) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	} else if (batch_type == BATCH_LINES) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;        
	} else if (batch_type == BATCH_LINE_LOOP) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;        
	} else if (batch_type == BATCH_POINTS) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;        
	} else {
		Assert(0);
	}
		
    _d3dContext->IASetPrimitiveTopology(primType);
    _d3dContext->Draw(num,0);
 }

void DeviceGraphicsDX11Renderer::drawRawBatchScreenSpace	(	const Matrix4 &transform, 
														Vector3 *v, 
														Vector3 *n, 
														Texcoord2 *t1, 
														Texcoord2 *t2, 
														Color *c,
														DTuint num,
														MaterialResource *material,
														BatchType batch_type)
{
	Matrix4 projection;
	
	// Same as //::glOrthof(0.0F,1.0F,0.0F,1.0F,-1.0F,1.0F);
	projection._m11 = 2.0F;		projection._m12 = 0.0F;		projection._m13 = 0.0F;		projection._m14 = -1.0F;
	projection._m21 = 0.0F;		projection._m22 = 2.0F;		projection._m23 = 0.0F;		projection._m24 = -1.0F;
	projection._m31 = 0.0F;		projection._m32 = 0.0F;		projection._m33 = -1.0F;	projection._m34 = 0.0F;
	projection._m41 = 0.0F;		projection._m42 = 0.0F;		projection._m43 = 0.0F;		projection._m44 = 1.0F;

	if (material) 
		activateMaterial(material);
	
    DeviceGraphicsDX11Shader::setConstantValue(_modelview_uniform, (DTubyte *) &transform, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_projection_uniform, (DTubyte *) &projection, sizeof(Matrix4) );
    DeviceGraphicsDX11Shader::setConstantValue(_camera_uniform, (DTubyte *) &_camera, sizeof(Matrix4) );

    UINT startSlot = 0;
    UINT numBuffers = 0;

    ID3D11Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {NULL};
    UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};
    UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {0};

    D3D11_MAPPED_SUBRESOURCE res;

    _d3dContext->Map(_vertices,0,D3D11_MAP_WRITE_DISCARD,0,&res);
    ::memcpy(res.pData,v,sizeof(Vector3)*num);
    _d3dContext->Unmap(_vertices,0);

    buffers[ShaderResource::ATTRIB_POSITION] = _vertices;
    strides[ShaderResource::ATTRIB_POSITION] = sizeof(Vector3);
	
	if (n) {
        _d3dContext->Map(_normals,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,n,sizeof(Vector3)*num);
        _d3dContext->Unmap(_normals,0);

        buffers[ShaderResource::ATTRIB_NORMAL] = _normals;
        strides[ShaderResource::ATTRIB_NORMAL] = sizeof(Vector3);
	}

	if (t1) {
        _d3dContext->Map(_texcoords1,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,t1,sizeof(Texcoord2)*num);
        _d3dContext->Unmap(_texcoords1,0);

        buffers[ShaderResource::ATTRIB_TEXCOORD0] = _texcoords1;
        strides[ShaderResource::ATTRIB_TEXCOORD0] = sizeof(Texcoord2);
	}
	
	if (t2) {
        _d3dContext->Map(_texcoords2,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,t2,sizeof(Texcoord2)*num);
        _d3dContext->Unmap(_texcoords2,0);

        buffers[ShaderResource::ATTRIB_TEXCOORD1] = _texcoords2;
        strides[ShaderResource::ATTRIB_TEXCOORD1] = sizeof(Texcoord2);
	}
	
	if (c) {
        _d3dContext->Map(_colors,0,D3D11_MAP_WRITE_DISCARD,0,&res);
        ::memcpy(res.pData,c,sizeof(Color)*num);
        _d3dContext->Unmap(_colors,0);

        buffers[ShaderResource::ATTRIB_COLOR] = _colors;
        strides[ShaderResource::ATTRIB_COLOR] = sizeof(Color);
	}

    // Resolve offsets
    DTuint offset = 0;
    for (DTuint i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++i) {
        offsets[i] = offset;
        offset += strides[i];
    }

    // Bind buffers
    _d3dContext->IASetVertexBuffers(0,ShaderResource::NUM_ATTRIBS,buffers,strides,offsets);
	
	// draw primitives
    D3D11_PRIMITIVE_TOPOLOGY primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (batch_type == BATCH_TRI_STRIP) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	} else if (batch_type == BATCH_TRI_POLY) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	} else if (batch_type == BATCH_LINES) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;        
	} else if (batch_type == BATCH_LINE_LOOP) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;        
	} else if (batch_type == BATCH_POINTS) {
        primType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;        
	} else {
		Assert(0);
	}
		
    _d3dContext->IASetPrimitiveTopology(primType);
    _d3dContext->Draw(num,0);
}	
														
//==============================================================================
//==============================================================================
                                                           
void DeviceGraphicsDX11Renderer::activateMaterial(MaterialResource *material)
{
	DeviceGraphicsDX11Material *m = getMaterialCached(material);
	m->activateMaterial();
	
	if (material->getShader()) {
		DeviceGraphicsDX11Shader *shader = getShaderCached(material->getShader());
		_modelview_uniform = shader->getConstant(ShaderResource::UNIFORM_MODELVIEW);
		_projection_uniform = shader->getConstant(ShaderResource::UNIFORM_PROJECTION);
		_camera_uniform = shader->getConstant(ShaderResource::UNIFORM_CAMERA);
		
		material->setCurrentUnit(0);
        m->setCurrentUnit(0);
		ID3D11Buffer *texture_uniform0 = shader->getConstant(ShaderResource::UNIFORM_TEXTURE0);
		if (texture_uniform0)
            DeviceGraphicsDX11Shader::setConstantValue(texture_uniform0, (DTubyte *) &m->getTextureMatrix(), sizeof(Matrix4) );

		material->setCurrentUnit(1);
        m->setCurrentUnit(1);
	    ID3D11Buffer *texture_uniform1 = shader->getConstant(ShaderResource::UNIFORM_TEXTURE1);
		if (texture_uniform1)
            DeviceGraphicsDX11Shader::setConstantValue(texture_uniform1, (DTubyte *) &m->getTextureMatrix(), sizeof(Matrix4) );

	} else {
        _modelview_uniform = NULL;
		_projection_uniform = NULL;
		_camera_uniform = NULL;
	}
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::blitImageToTexture          (TextureResource *texture, DTfloat x_pos, DTfloat y_pos, DTfloat width, DTfloat height, DTuint data_width, DTuint data_height, Color *buffer)
{
    DeviceGraphicsDX11Texture *t = getTextureCached(texture);
    //GLint name = t->getOpenGLName();
 
    //GLint savetex;
    //::glGetIntegerv(GL_TEXTURE_BINDING_2D, &savetex);

	//::glActiveTexture(GL_TEXTURE0);
    //::glBindTexture(GL_TEXTURE_2D, name);          //	bind the texture
    
    //if (texture->getFormat() == TextureResource::FORMAT_RGBA)
        //::glTexSubImage2D(GL_TEXTURE_2D, 0, x_pos, y_pos, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    //else if (texture->getFormat() == TextureResource::FORMAT_BGRA)
        //::glTexSubImage2D(GL_TEXTURE_2D, 0, x_pos, y_pos, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

	//::glBindTexture(GL_TEXTURE_2D, savetex);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Renderer::emergencyFreeMemory(void)
{	
    _particles_cache.removeOldItems(0.0f);
    _geometry_cache.removeOldItems(0.0f);
    _texture_cache.removeOldItems(0.0f); 
    _material_cache.removeOldItems(0.0f);	
    _pixel_shader_cache.removeOldItems(0.0f);
    _vertex_shader_cache.removeOldItems(0.0f);
    _framebuffer_cache.removeOldItems (0.0f);
}

//==============================================================================
//==============================================================================
                                                        
} // DT2

