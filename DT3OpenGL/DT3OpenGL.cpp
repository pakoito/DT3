//==============================================================================
///	
///	File: DT3GL.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3OpenGL/DT3OpenGL.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"

extern "C" {
    extern unsigned int hashlittle( const void *key, unsigned int length, unsigned int initval);
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

namespace {

    DTuint gBlending[] = {
        /*DT3GL_BLEND_ZERO*/                GL_ZERO,
        /*DT3GL_BLEND_ONE*/                 GL_ONE,
        /*DT3GL_BLEND_SRC_ALPHA*/           GL_SRC_ALPHA,
        /*DT3GL_BLEND_ONE_MINUS_SRC_ALPHA*/ GL_ONE_MINUS_SRC_ALPHA,
        /*DT3GL_BLEND_DST_ALPHA*/           GL_DST_ALPHA,
        /*DT3GL_BLEND_ONE_MINUS_DST_ALPHA*/ GL_ONE_MINUS_DST_ALPHA,
        /*DT3GL_BLEND_SRC_COLOR*/           GL_SRC_COLOR,
        /*DT3GL_BLEND_DST_COLOR*/           GL_DST_COLOR,
        /*DT3GL_BLEND_ONE_MINUS_SRC_COLOR*/ GL_ONE_MINUS_SRC_COLOR,
        /*DT3GL_BLEND_ONE_MINUS_DST_COLOR*/ GL_ONE_MINUS_DST_COLOR
    };

    DTuint gCulling[] = {
        /*DT3GL_FRONT*/                     GL_FRONT,
        /*DT3GL_BACK*/                      GL_BACK,
        /*DT3GL_CULL_NONE*/                 GL_NONE
    };
    
    DTuint gWrap[] = {
        /*TEXTURE_WRAP_REPEAT*/             GL_REPEAT,
        /*TEXTURE_WRAP_CLAMP*/              GL_CLAMP_TO_EDGE
    };

    DTuint gFilterMinMode[] = {
        /*TEXTURE_FILTER_NEAREST*/          GL_NEAREST,
        /*TEXTURE_FILTER_LINEAR*/           GL_LINEAR,
        /*TEXTURE_FILTER_NEAREST_MIPMAP*/   GL_NEAREST_MIPMAP_NEAREST,
        /*TEXTURE_FILTER_LINEAR_MIPMAP*/    GL_LINEAR_MIPMAP_LINEAR
    };

    DTuint gFilterMagMode[] = {
        /*TEXTURE_FILTER_NEAREST*/          GL_NEAREST,
        /*TEXTURE_FILTER_LINEAR*/           GL_LINEAR,
        /*TEXTURE_FILTER_NEAREST_MIPMAP*/   GL_NEAREST,
        /*TEXTURE_FILTER_LINEAR_MIPMAP*/    GL_LINEAR
    };
    
	DTuint gComparison[] = {
        /*DT3GL_ALWAYS*/                    GL_ALWAYS,
        /*DT3GL_NEVER*/                     GL_NEVER,
        /*DT3GL_EQUAL*/                     GL_EQUAL,
        /*DT3GL_NOTEQUAL*/                  GL_NOTEQUAL,
        /*DT3GL_GREATER*/                   GL_GREATER,
        /*DT3GL_GEQUAL*/                    GL_GEQUAL,
        /*DT3GL_LESS*/                      GL_LESS,
        /*DT3GL_LEQUAL*/                    GL_LEQUAL
	};
    
	DTuint gStencilOp[] = {
        /*DT3GL_STENCILOP_KEEP*/            GL_KEEP,
        /*DT3GL_STENCILOP_ZERO*/            GL_ZERO,
        /*DT3GL_STENCILOP_REPLACE*/         GL_REPLACE,
        /*DT3GL_STENCILOP_INCR*/            GL_INCR,
        /*DT3GL_STENCILOP_DECR*/            GL_DECR,
        /*DT3GL_STENCILOP_INCR_WRAP*/       GL_INCR_WRAP,
        /*DT3GL_STENCILOP_DECR_WRAP*/       GL_DECR_WRAP,
        /*DT3GL_STENCILOP_INVERT*/          GL_INVERT
	};

	DTuint gPrimitiveType[] = {
        /*DT3GL_PRIM_TRIS*/                 GL_TRIANGLES,
        /*DT3GL_PRIM_TRI_STRIP*/            GL_TRIANGLE_STRIP,
        /*DT3GL_PRIM_LINE_LOOP*/            GL_LINE_LOOP,
        /*DT3GL_PRIM_LINES*/                GL_LINES
	};
    
}

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DT3OpenGL::DT3OpenGL (void)
    :   _current_stencil_ref    (0),
        _pending_stencil_ref    (0),
        _width                  (0),
        _height                 (0),
        _viewport_x             (0),
        _viewport_y             (0),
        _viewport_width         (0),
        _viewport_height        (0),
        _default_framebuffer    (0)
{

}
            
DT3OpenGL::~DT3OpenGL (void)
{
    _depth_stencil_states.clear();
    _blend_states.clear();
    _rasterizer_states.clear();
    _sampler_states.clear();
}

//==============================================================================
//==============================================================================

void DT3OpenGL::init_display (void)
{
    DeviceGraphics::init_display();
}

void DT3OpenGL::open_display (DTint width, DTint height)
{
    // Update Geometry
	_width = width;
	_height = height;
    
    // Called to say we are ready to init this device
    DeviceGraphics::init_display();
    
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
	// init Glew
    GLenum err = glewInit();
	if (GLEW_OK != err) {
		WARNINGMSG("Could not initialize glew.");
		return;
	}
#endif
	
	set_viewport (0, 0, width, height, width, height);

	LOG_MESSAGE << "Vendor:     " << ::glGetString(GL_VENDOR);
	LOG_MESSAGE << "Renderer:   " << ::glGetString(GL_RENDERER);
	LOG_MESSAGE << "Version:    " << ::glGetString(GL_VERSION);
	LOG_MESSAGE << "Extensions: " << ::glGetString(GL_EXTENSIONS);
	
    //
    // Default State
    //
    
    // clear out all of the buffers
    clear_color_value(Color4f(0.0F,0.0F,0.0F,0.0F));
    clear_depth_value(1.0f);
    
    ::glGetIntegerv(GL_FRAMEBUFFER_BINDINGM, &_default_framebuffer);
	   
    DeviceGraphics::open_display(width,height);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::change_display (DTuint width, DTuint height)
{
	_width = width;
	_height = height;
	
	set_viewport (0, 0, width, height, width, height);
	   
    DeviceGraphics::change_display(width,height);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::close_display (void)
{
    DeviceGraphics::close_display();
}

DTuint DT3OpenGL::screen_width (void)
{
	return _width;
}

DTuint DT3OpenGL::screen_height (void)
{
	return _height;
}

DTfloat DT3OpenGL::screen_aspect (void)
{
	return (DTfloat) _width / (DTfloat) _height;
}

//==============================================================================
//==============================================================================

void DT3OpenGL::viewport (DTint &x, DTint &y, DTint &width, DTint &height)
{
	x = _viewport_x;
	y = _viewport_y;
	width = _viewport_width;
	height = _viewport_height;
}

void DT3OpenGL::set_viewport (DTint x, DTint y, DTint width, DTint height, DTint rt_width, DTint rt_height)
{
	::glViewport(x,y,width,height);
	
	_viewport_x = x;
	_viewport_y = y;
	_viewport_width = width;
	_viewport_height = height;

    ASSERT(::glGetError() == GL_NO_ERROR);
}

DTuint DT3OpenGL::viewport_width (void)
{
	return _viewport_width;
}

DTuint DT3OpenGL::viewport_height (void)
{
	return _viewport_height;
}

DTfloat DT3OpenGL::viewport_aspect (void)
{
	return (DTfloat) _viewport_width / (DTfloat) _viewport_height;
}

//==============================================================================
//==============================================================================

void DT3OpenGL::clear_viewport (DTboolean depth, DTboolean color, DTboolean stencil)
{
	GLenum flags =	(GL_DEPTH_BUFFER_BIT * (depth ? 1:0)) |
					(GL_COLOR_BUFFER_BIT * (color ? 1:0)) |
                    (GL_STENCIL_BUFFER_BIT * (stencil ? 1:0));
	
    GLboolean depth_mask;
    GLboolean stencil_mask;

    if (depth) {
        ::glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
        ::glDepthMask(GL_TRUE);
    }
                    
    if (stencil) {
        ::glGetBooleanv(GL_STENCIL_WRITEMASK, &stencil_mask);
        ::glStencilMask(0xFFFFFFFF);
    }
        
    ::glClear(flags);
        
    if (stencil)
        ::glStencilMask(stencil_mask);

    if (depth)
        ::glDepthMask(depth_mask);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::clear_stencil_value (DTuint s)
{
	::glClearStencil(s);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::clear_color_value (const Color4f &c)
{
	::glClearColor(c.r_as_float(),c.g_as_float(),c.b_as_float(),c.a_as_float());

    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::clear_depth_value (DTfloat d)
{
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
	::glClearDepth(d);
#else
	::glClearDepthf(d);
#endif

    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
// _____             _   _        _____ _                  _ _
//|  __ \           | | | |      / ____| |                (_) |
//| |  | | ___ _ __ | |_| |__   | (___ | |_ ___ _ __   ___ _| |
//| |  | |/ _ \ '_ \| __| '_ \   \___ \| __/ _ \ '_ \ / __| | |
//| |__| |  __/ |_) | |_| | | |  ____) | ||  __/ | | | (__| | |
//|_____/ \___| .__/ \__|_| |_| |_____/ \__\___|_| |_|\___|_|_|
//            | |
//            |_|
//==============================================================================
#pragma mark Depth Stencil

std::shared_ptr<DT3GLDepthStencilResource> DT3OpenGL::create_depth_stencil_state (const DT3GLDepthStencilState &state)
{
    DTuint hash = hashlittle( &state, sizeof(DT3GLDepthStencilState), 0);

    auto i = _depth_stencil_states.find(hash);
    if (i == _depth_stencil_states.end()) {
    
        // Build resource
        DT3OpenGLDepthStencilResource *r = new DT3OpenGLDepthStencilResource();
        r->state = state;
        
        auto rr = std::shared_ptr<DT3GLDepthStencilResource>(r);
        
        _depth_stencil_states[hash] = rr;
    
        return rr;
    } else {
        return i->second;
    }

}

void DT3OpenGL::set_stencil_ref (DTuint ref)
{
    _pending_stencil_ref = ref;
}

//==============================================================================
// ____  _                _
//|  _ \| |              | |
//| |_) | | ___ _ __   __| |
//|  _ <| |/ _ \ '_ \ / _` |
//| |_) | |  __/ | | | (_| |
//|____/|_|\___|_| |_|\__,_|
//==============================================================================
#pragma mark Blend

std::shared_ptr<DT3GLBlendResource> DT3OpenGL::create_blend_state (const DT3GLBlendState &state)
{
    DTuint hash = hashlittle( &state, sizeof(DT3GLBlendState), 0);

    auto i = _blend_states.find(hash);
    if (i == _blend_states.end()) {
    
        // Build resource
        DT3OpenGLBlendResource *r = new DT3OpenGLBlendResource();
        r->state = state;
        
        auto rr = std::shared_ptr<DT3GLBlendResource>(r);
        
        _blend_states[hash] = rr;
    
        return rr;
    } else {
        return i->second;
    }

}

//==============================================================================
// _____           _            _
//|  __ \         | |          (_)
//| |__) |__ _ ___| |_ ___ _ __ _ _______ _ __
//|  _  // _` / __| __/ _ \ '__| |_  / _ \ '__|
//| | \ \ (_| \__ \ ||  __/ |  | |/ /  __/ |
//|_|  \_\__,_|___/\__\___|_|  |_/___\___|_|
//
//==============================================================================
#pragma mark Rasterizer

std::shared_ptr<DT3GLRasterizerResource> DT3OpenGL::create_rasterizer_state (const DT3GLRasterizerState &state)
{
    DTuint hash = hashlittle( &state, sizeof(DT3GLRasterizerState), 0);

    auto i = _rasterizer_states.find(hash);
    if (i == _rasterizer_states.end()) {
    
        // Build resource
        DT3OpenGLRasterizerResource *r = new DT3OpenGLRasterizerResource();
        r->state = state;
        
        auto rr = std::shared_ptr<DT3OpenGLRasterizerResource>(r);
        
        _rasterizer_states[hash] = rr;
    
        return rr;
    } else {
        return i->second;
    }
}

//==============================================================================
//  _____                       _
// / ____|                     | |
//| (___   __ _ _ __ ___  _ __ | | ___ _ __
// \___ \ / _` | '_ ` _ \| '_ \| |/ _ \ '__|
// ____) | (_| | | | | | | |_) | |  __/ |
//|_____/ \__,_|_| |_| |_| .__/|_|\___|_|
//                       | |
//                       |_|
//==============================================================================
#pragma mark Sampler

std::shared_ptr<DT3GLSamplerResource> DT3OpenGL::create_sampler_state (const DT3GLSamplerState &state)
{
    DTuint hash = hashlittle( &state, sizeof(DT3GLSamplerState), 0);

    auto i = _sampler_states.find(hash);
    if (i == _sampler_states.end()) {
    
        // Build resource
        DT3OpenGLSamplerResource *r = new DT3OpenGLSamplerResource();
        r->state = state;
        
        auto rr = std::shared_ptr<DT3GLSamplerResource>(r);
        
        _sampler_states[hash] = rr;
    
        return rr;
    } else {
        return i->second;
    }

}

//==============================================================================
// _______        _                    ___  _____
//|__   __|      | |                  |__ \|  __ \
//   | | _____  _| |_ _   _ _ __ ___     ) | |  | |
//   | |/ _ \ \/ / __| | | | '__/ _ \   / /| |  | |
//   | |  __/>  <| |_| |_| | | |  __/  / /_| |__| |
//   |_|\___/_/\_\\__|\__,_|_|  \___| |____|_____/
//==============================================================================
#pragma mark Texture2D

std::shared_ptr<DT3GLTexture2DResource> DT3OpenGL::create_texture_2D (DTint width, DTint height, DTubyte *textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags)
{
    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_2D, &save_tex);

    DT3OpenGLTexture2DResource *r = new DT3OpenGLTexture2DResource();

    r->format = format;
    r->mipmapped = mipmapped;

    ::glGenTextures(1,&r->name);
    
    // Create OpenGL2 Texture
    ::glBindTexture(GL_TEXTURE_2D, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Build texture based on the format
    switch (r->format) {
    
        case DT3GL_FORMAT_RGBA:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_BGRA:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRAM, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGBA_5551:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, textels);
            break;
        case DT3GL_FORMAT_RGBA_4444:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, textels);
            break;
        case DT3GL_FORMAT_RGB_565:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, textels);
            break;
        case DT3GL_FORMAT_LUM_8:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGB:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_DEPTH_16:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16M, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            break;
        case DT3GL_FORMAT_DEPTH_24:
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24M, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            break;

//#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
//        case DT3GL_FORMAT_PVR2_RGBA:
//            ::glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, width, height, 0, compressed_size, textels);
//            break;
//        case DT3GL_FORMAT_PVR4_RGBA:
//            ::glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, width, height, 0, compressed_size, textels);
//            break;
//        case DT3GL_FORMAT_PVR2_RGB:
//            ::glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, width, height, 0, compressed_size, textels);
//            break;
//        case DT3GL_FORMAT_PVR4_RGB:
//            ::glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, width, height, 0, compressed_size, textels);
//            break;
//#endif

        default:
            ASSERT(0);

    }
    
    ASSERT(::glGetError() == GL_NO_ERROR);

	if (r->mipmapped) {
		::glGenerateMipmapM(GL_TEXTURE_2D);   // establish a mipmap chain for the texture
        ASSERT(::glGetError() == GL_NO_ERROR);
	}

	::glBindTexture(GL_TEXTURE_2D, save_tex);

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLTexture2DResource>(r);
}

void DT3OpenGL::update_texture_2D (const std::shared_ptr<DT3GLTexture2DResource> &res, DTint x, DTint y, DTint width, DTint height, DTubyte *textels)
{
    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_2D, &save_tex);

    DT3OpenGLTexture2DResource *r = static_cast<DT3OpenGLTexture2DResource*>(res.get());

    // Create OpenGLES2 Texture
    ::glBindTexture(GL_TEXTURE_2D, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	   
    // Build texture based on the format
    switch (r->format) {

        case DT3GL_FORMAT_RGBA:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_BGRA:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRAM, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGBA_5551:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, textels);
            break;
        case DT3GL_FORMAT_RGBA_4444:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, textels);
            break;
        case DT3GL_FORMAT_RGB_565:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, textels);
            break;
        case DT3GL_FORMAT_LUM_8:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGB:
            ::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_DEPTH_16:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_DEPTH_24:
            ASSERT(0);
            break;
#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
        case DT3GL_FORMAT_PVR2_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR2_RGB:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGB:
            ASSERT(0);
            break;
#endif

        default:
            ASSERT(0);

    }

    ASSERT(::glGetError() == GL_NO_ERROR);

	if (r->mipmapped) {
		::glGenerateMipmapM(GL_TEXTURE_2D);   // establish a mipmap chain for the texture
        ASSERT(::glGetError() == GL_NO_ERROR);
	}

	::glBindTexture(GL_TEXTURE_2D, save_tex);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
// _______        _                    ____  _____
//|__   __|      | |                  |___ \|  __ \
//   | | _____  _| |_ _   _ _ __ ___    __) | |  | |
//   | |/ _ \ \/ / __| | | | '__/ _ \  |__ <| |  | |
//   | |  __/>  <| |_| |_| | | |  __/  ___) | |__| |
//   |_|\___/_/\_\\__|\__,_|_|  \___| |____/|_____/
//==============================================================================
#pragma mark Texture3D

std::shared_ptr<DT3GLTexture3DResource> DT3OpenGL::create_texture_3D (DTint width, DTint height, DTint depth, DTubyte *textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags)
{
#if DT3_OS != DT3_IOS && DT3_OS != DT3_ANDROID

    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_3D, &save_tex);

    DT3OpenGLTexture3DResource *r = new DT3OpenGLTexture3DResource();

    r->format = format;
    r->mipmapped = mipmapped;

    ::glGenTextures(1,&r->name);
    
    // Create OpenGL2 Texture
    ::glBindTexture(GL_TEXTURE_3D, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    switch (r->format) {
    
        case DT3GL_FORMAT_RGBA:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_BGRA:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_BGRAM, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGBA_5551:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, textels);
            break;
        case DT3GL_FORMAT_RGBA_4444:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, textels);
            break;
        case DT3GL_FORMAT_RGB_565:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, depth, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, textels);
            break;
        case DT3GL_FORMAT_LUM_8:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, width, height, depth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGB:
            ::glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_DEPTH_16:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_DEPTH_24:
            ASSERT(0);
            break;
    #if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
        case DT3GL_FORMAT_PVR2_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR2_RGB:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGB:
            ASSERT(0);
            break;
    #endif

        default:
            ASSERT(0);
    }
    
    ASSERT(::glGetError() == GL_NO_ERROR);

	if (r->mipmapped) {
		::glGenerateMipmapM(GL_TEXTURE_3D);   // establish a mipmap chain for the texture
        ASSERT(::glGetError() == GL_NO_ERROR);
	}
    
	::glBindTexture(GL_TEXTURE_3D, save_tex);

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLTexture3DResource>(r);
#else
    return std::shared_ptr<DT3GLTexture3DResource>();
#endif
}

void DT3OpenGL::update_texture_3D (const std::shared_ptr<DT3GLTexture3DResource> &res, DTint x, DTint y, DTint z, DTint width, DTint height, DTint depth, DTubyte *textels)
{
#if DT3_OS != DT3_IOS && DT3_OS != DT3_ANDROID

    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_3D, &save_tex);

    DT3OpenGLTexture3DResource *r = static_cast<DT3OpenGLTexture3DResource*>(res.get());

    // Create OpenGLES2 Texture
    ::glBindTexture(GL_TEXTURE_2D, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	   
    // Build texture based on the format
    switch (r->format) {

        case DT3GL_FORMAT_RGBA:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_BGRA:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_BGRAM, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGBA_5551:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, textels);
            break;
        case DT3GL_FORMAT_RGBA_4444:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, textels);
            break;
        case DT3GL_FORMAT_RGB_565:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, textels);
            break;
        case DT3GL_FORMAT_LUM_8:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_LUMINANCE, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_RGB:
            ::glTexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, GL_RGB, GL_UNSIGNED_BYTE, textels);
            break;
        case DT3GL_FORMAT_DEPTH_16:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_DEPTH_24:
            ASSERT(0);
            break;
    #if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
        case DT3GL_FORMAT_PVR2_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGBA:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR2_RGB:
            ASSERT(0);
            break;
        case DT3GL_FORMAT_PVR4_RGB:
            ASSERT(0);
            break;
    #endif

        default:
            ASSERT(0);

    }

    ASSERT(::glGetError() == GL_NO_ERROR);

	if (r->mipmapped) {
		::glGenerateMipmapM(GL_TEXTURE_3D);   // establish a mipmap chain for the texture
        ASSERT(::glGetError() == GL_NO_ERROR);
	}
    
	::glBindTexture(GL_TEXTURE_3D, save_tex);

    ASSERT(::glGetError() == GL_NO_ERROR);
#endif
}

//==============================================================================
// _______        _                     _____      _
//|__   __|      | |                   / ____|    | |
//   | | _____  _| |_ _   _ _ __ ___  | |    _   _| |__   ___
//   | |/ _ \ \/ / __| | | | '__/ _ \ | |   | | | | '_ \ / _ \
//   | |  __/>  <| |_| |_| | | |  __/ | |___| |_| | |_) |  __/
//   |_|\___/_/\_\\__|\__,_|_|  \___|  \_____\__,_|_.__/ \___|
//==============================================================================
#pragma mark TextureCube

std::shared_ptr<DT3GLTextureCubeResource> DT3OpenGL::create_texture_cube (  DTint width, DTint height,
                                                                            DTubyte *textels_front,
                                                                            DTubyte *textels_back,
                                                                            DTubyte *textels_left,
                                                                            DTubyte *textels_right,
                                                                            DTubyte *textels_top,
                                                                            DTubyte *textels_bottom,
                                                                            DT3GLTextelFormat format,
                                                                            DTboolean mipmapped,
                                                                            DTuint flags)
{
    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &save_tex);

    DT3OpenGLTextureCubeResource *r = new DT3OpenGLTextureCubeResource();

    r->format = format;
    r->mipmapped = mipmapped;

    ::glGenTextures(1,&r->name);
    
    // Create OpenGL2 Texture
    ::glBindTexture(GL_TEXTURE_CUBE_MAPM, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (DTuint f = 0; f < 6; ++f) {
    
        GLenum face;
        GLvoid *buffer;

        switch (f) {
            case 0: face = GL_TEXTURE_CUBE_MAP_POSITIVE_ZM;  buffer = textels_front;   break;
            case 1: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_ZM;  buffer = textels_back;    break;
            case 2: face = GL_TEXTURE_CUBE_MAP_POSITIVE_YM;  buffer = textels_top;     break;
            case 3: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_YM;  buffer = textels_bottom;  break;
            case 4: face = GL_TEXTURE_CUBE_MAP_POSITIVE_XM;  buffer = textels_left;    break;
            case 5: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_XM;  buffer = textels_right;   break;
        };
        
        switch (r->format) {
	    
            case DT3GL_FORMAT_RGBA:
                ::glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_BGRA:
                ::glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_BGRAM, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_RGBA_5551:
                ::glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, buffer);
                break;
            case DT3GL_FORMAT_RGBA_4444:
                ::glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, buffer);
                break;
            case DT3GL_FORMAT_RGB_565:
                ::glTexImage2D(face, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);
                break;
            case DT3GL_FORMAT_LUM_8:
                ::glTexImage2D(face, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_RGB:
                ::glTexImage2D(face, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                break;
            default:
                ASSERT(0);
                break;
        }
  
        ASSERT(::glGetError() == GL_NO_ERROR);
        if (r->mipmapped) {
            ::glGenerateMipmapM(GL_TEXTURE_CUBE_MAPM);   // establish a mipmap chain for the texture
            ASSERT(::glGetError() == GL_NO_ERROR);
        }
    
    }

	::glBindTexture(GL_TEXTURE_CUBE_MAP, save_tex);
 
    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLTextureCubeResource>(r);
}

void DT3OpenGL::update_texture_cube (   const std::shared_ptr<DT3GLTextureCubeResource> &res, DTint x, DTint y, DTint width, DTint height,
                                        DTubyte *textels_front,
                                        DTubyte *textels_back,
                                        DTubyte *textels_left,
                                        DTubyte *textels_right,
                                        DTubyte *textels_top,
                                        DTubyte *textels_bottom)
{
    GLint save_tex;
    ::glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &save_tex);

    DT3OpenGLTextureCubeResource *r = new DT3OpenGLTextureCubeResource();

    // Create OpenGLES2 Texture
    ::glBindTexture(GL_TEXTURE_2D, r->name);          //	bind the texture
    
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (DTuint f = 0; f < 6; ++f) {
    
        GLenum face;
        GLvoid *buffer;
        
        switch (f) {
            case 0: face = GL_TEXTURE_CUBE_MAP_POSITIVE_ZM;  buffer = textels_front;   break;
            case 1: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_ZM;  buffer = textels_back;    break;
            case 2: face = GL_TEXTURE_CUBE_MAP_POSITIVE_YM;  buffer = textels_top;     break;
            case 3: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_YM;  buffer = textels_bottom;  break;
            case 4: face = GL_TEXTURE_CUBE_MAP_POSITIVE_XM;  buffer = textels_left;    break;
            case 5: face = GL_TEXTURE_CUBE_MAP_NEGATIVE_XM;  buffer = textels_right;   break;
        };
        
        // Build texture based on the format
        switch (r->format) {

            case DT3GL_FORMAT_RGBA:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_BGRA:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_BGRAM, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_RGBA_5551:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, buffer);
                break;
            case DT3GL_FORMAT_RGBA_4444:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, buffer);
                break;
            case DT3GL_FORMAT_RGB_565:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);
                break;
            case DT3GL_FORMAT_LUM_8:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_RGB:
                ::glTexSubImage2D(face, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                break;
            case DT3GL_FORMAT_DEPTH_16:
                ASSERT(0);
                break;
            case DT3GL_FORMAT_DEPTH_24:
                ASSERT(0);
                break;
#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
            case DT3GL_FORMAT_PVR2_RGBA:
                ASSERT(0);
                break;
            case DT3GL_FORMAT_PVR4_RGBA:
                ASSERT(0);
                break;
            case DT3GL_FORMAT_PVR2_RGB:
                ASSERT(0);
                break;
            case DT3GL_FORMAT_PVR4_RGB:
                ASSERT(0);
                break;
#endif

            default:
                ASSERT(0);

        }
        
        ASSERT(::glGetError() == GL_NO_ERROR);
        if (r->mipmapped) {
            ::glGenerateMipmapM(GL_TEXTURE_CUBE_MAPM);   // establish a mipmap chain for the texture
            ASSERT(::glGetError() == GL_NO_ERROR);
        }
    
    }

	::glBindTexture(GL_TEXTURE_CUBE_MAP, save_tex);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
// ____         __  __
//|  _ \       / _|/ _|
//| |_) |_   _| |_| |_ ___ _ __
//|  _ <| | | |  _|  _/ _ \ '__|
//| |_) | |_| | | | ||  __/ |
//|____/ \__,_|_| |_| \___|_|
//==============================================================================
#pragma mark Buffer

std::shared_ptr<DT3GLAttribBufferResource> DT3OpenGL::create_buffer (DTubyte *buffer_data, DTsize buffer_size, DT3GLBufferFormat buffer_format, DTuint flags)
{
    DT3OpenGLAttribBufferResource *r = new DT3OpenGLAttribBufferResource();

    r->flags = flags;
    r->format = buffer_format;
    
    GLint save_buffer;
    ::glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &save_buffer);

	::glGenBuffers(1,&r->name);
	::glBindBuffer(GL_ARRAY_BUFFER, r->name);
    
    //if (r->flags == DT3GL_ACCESS_CPU_WRITE)
        ::glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) buffer_size, buffer_data, GL_STREAM_DRAW);
	//else
    //    ::glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) buffer_size, buffer_data, GL_STATIC_DRAW);
		
	::glBindBuffer(GL_ARRAY_BUFFER, save_buffer);

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLAttribBufferResource>(r);
}

void DT3OpenGL::update_buffer (const std::shared_ptr<DT3GLAttribBufferResource> &res, DTubyte *buffer_data, DTsize buffer_size, DTsize buffer_offset)
{
    DT3OpenGLAttribBufferResource *r = static_cast<DT3OpenGLAttribBufferResource*>(res.get());

    GLint save_buffer;
    ::glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &save_buffer);

	::glBindBuffer(GL_ARRAY_BUFFER, r->name);
    
//    void *p = ::glMapBufferRangeEXT(GL_ARRAY_BUFFER, (GLsizeiptr) buffer_offset, (GLsizeiptr) buffer_size, GL_MAP_WRITE_BIT_EXT | GL_MAP_UNSYNCHRONIZED_BIT_EXT);
//    ::memcpy(p, buffer_data, (DTuint) buffer_size);
//    ::glBufferSubData(GL_ARRAY_BUFFER, buffer_offset, (GLsizeiptr) buffer_size, buffer_data);
		
    ::glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) buffer_size, buffer_data, GL_STREAM_DRAW);    // Orphaning
    //::glBufferSubData(GL_ARRAY_BUFFER, buffer_offset, (GLsizeiptr) buffer_size, buffer_data);

	::glBindBuffer(GL_ARRAY_BUFFER, save_buffer);

    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
// _    _       _  __
//| |  | |     (_)/ _|
//| |  | |_ __  _| |_ ___  _ __ _ __ ___  ___
//| |  | | '_ \| |  _/ _ \| '__| '_ ` _ \/ __|
//| |__| | | | | | || (_) | |  | | | | | \__ \
// \____/|_| |_|_|_| \___/|_|  |_| |_| |_|___/
//==============================================================================
#pragma mark Uniforms

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const DTint buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_1_INT;
    r->num = 1;
    
    r->data.resize(sizeof(DTint));
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const Vector4 &buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_4_FLOAT;
    r->num = 1;
    
    r->data.resize(sizeof(Vector4));
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const std::vector<Vector4> &buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_4_FLOAT;
    r->num = (DTint) buffer_data.size();
    
    r->data.resize(sizeof(Vector4) * r->num);
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const Matrix4 &buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_16_FLOAT;
    r->num = 1;
    
    r->data.resize(sizeof(Matrix4));
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const std::vector<Matrix4> &buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_16_FLOAT;
    r->num = (DTint) buffer_data.size();
    
    r->data.resize(sizeof(Matrix4) * r->num);
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

std::shared_ptr<DT3GLUniformResource> DT3OpenGL::create_uniform (const Color4f &buffer_data)
{
    DT3OpenGLUniformResource *r = new DT3OpenGLUniformResource();
    
    r->format = DT3GL_UNIFORM_FORMAT_4_FLOAT;
    r->num = 1;
    
    r->data.resize(sizeof(Color4f));
    ::memcpy(&r->data[0], &buffer_data, r->data.size());

    r->use_stamp = ++_use_stamp;

    return std::shared_ptr<DT3GLUniformResource>(r);
}

//==============================================================================
//==============================================================================

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const DTint buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_1_INT);
    ASSERT(r->data.size() == sizeof(DTint));
    
    if (::memcmp(&r->data[0], &buffer_data, r->data.size()) != 0) {
        ::memcpy(&r->data[0], &buffer_data, r->data.size());
        r->use_stamp = ++_use_stamp;
    }
}

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const Vector4 &buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_4_FLOAT);
    ASSERT(r->data.size() == sizeof(Vector4));

    if (::memcmp(&r->data[0], &buffer_data, r->data.size()) != 0) {
        ::memcpy(&r->data[0], &buffer_data, r->data.size());
        r->use_stamp = ++_use_stamp;
    }
}

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const std::vector<Vector4> &buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_4_FLOAT);
    ASSERT(r->data.size() == sizeof(Vector4) * buffer_data.size());
    
    ::memcpy(&r->data[0], &buffer_data[0], r->data.size());
    r->use_stamp = ++_use_stamp;
}

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const Matrix4 &buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_16_FLOAT);
    ASSERT(r->data.size() == sizeof(Matrix4));
    
    if (::memcmp(&r->data[0], &buffer_data, r->data.size()) != 0) {
        ::memcpy(&r->data[0], &buffer_data, r->data.size());
        r->use_stamp = ++_use_stamp;
    }
}

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const std::vector<Matrix4> &buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_16_FLOAT);
    ASSERT(r->data.size() == sizeof(Matrix4) * buffer_data.size());
    
    ::memcpy(&r->data[0], &buffer_data[0], r->data.size());
    r->use_stamp = ++_use_stamp;
}

void DT3OpenGL::update_uniform (const std::shared_ptr<DT3GLUniformResource> &res, const Color4f &buffer_data)
{
    std::shared_ptr<DT3OpenGLUniformResource> r = std::static_pointer_cast<DT3OpenGLUniformResource>(res);
    
    ASSERT(r->format == DT3GL_UNIFORM_FORMAT_4_FLOAT);
    ASSERT(r->data.size() == sizeof(Color4f));
    
    if (::memcmp(&r->data[0], &buffer_data, r->data.size()) != 0) {
        ::memcpy(&r->data[0], &buffer_data, r->data.size());
        r->use_stamp = ++_use_stamp;
    }
}

//==============================================================================
//  _____ _               _
// / ____| |             | |
//| (___ | |__   __ _  __| | ___ _ __
// \___ \| '_ \ / _` |/ _` |/ _ \ '__|
// ____) | | | | (_| | (_| |  __/ |
//|_____/|_| |_|\__,_|\__,_|\___|_|
//==============================================================================
#pragma mark Shader

std::shared_ptr<DT3GLShaderResource> DT3OpenGL::create_shader (void)
{
    DT3OpenGLShaderResource *r = new DT3OpenGLShaderResource();
    
    // Create Program
    r->name = ::glCreateProgram();

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLShaderResource>(r);
}

void DT3OpenGL::link_shader (const std::shared_ptr<DT3GLShaderResource> &shader)
{
    std::shared_ptr<DT3OpenGLShaderResource> r = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);

    // Link Program
    ::glLinkProgram(r->name);
    
    GLint linked;
    ::glGetProgramiv(r->name, GL_LINK_STATUS, &linked);

    // Check link status
    if (!linked) {
        GLint infoLen = 0;
        
        ::glGetProgramiv(r->name, GL_INFO_LOG_LENGTH, &infoLen);
        
        if (infoLen > 1) {
            DTcharacter *infoLog = new ("OGL Log") DTcharacter[infoLen];
            ::glGetProgramInfoLog(r->name, infoLen, NULL, infoLog);
            
            LOG_MESSAGE << infoLog;
            
            delete[] infoLog;
        } else {
            LOG_MESSAGE << "Program is not linked and there is no error log.";
        }
        
        ::glDeleteProgram(r->name);
        return;	
    }
    
    ASSERT(::glGetError() == GL_NO_ERROR);

}

std::shared_ptr<DT3GLVertexShaderResource> DT3OpenGL::create_vertex_shader (const std::string &source)
{
    DT3OpenGLVertexShaderResource *r = new DT3OpenGLVertexShaderResource();

	// Create the shader object
	r->name = ::glCreateShader(GL_VERTEX_SHADER);
	if (r->name == 0) {
        delete r;
		return std::shared_ptr<DT3GLVertexShaderResource>();
    }
		
	// load the _shader source
	const DTcharacter *s = source.c_str();
	::glShaderSource(r->name, 1, &s, NULL);
	
	// compile the _shader
	::glCompileShader(r->name);
	
	// Check the compile status
	GLint	compiled;
	::glGetShaderiv(r->name, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		GLint infoLen = 0;
		
		::glGetShaderiv(r->name, GL_INFO_LOG_LENGTH, &infoLen);
		
		if (infoLen > 1) {
			DTcharacter *infoLog = new ("ShaderLog") DTcharacter[infoLen];
			::glGetShaderInfoLog(r->name, infoLen, NULL, infoLog);
			
			LOG_MESSAGE << infoLog;
			
			delete[] infoLog;
		}
		
		::glDeleteShader(r->name);
        
        delete r;
		return std::shared_ptr<DT3GLVertexShaderResource>();
	}
    
    ASSERT(::glGetError() == GL_NO_ERROR);
    
    return std::shared_ptr<DT3GLVertexShaderResource>(r);
}

std::shared_ptr<DT3GLGeometryShaderResource> DT3OpenGL::create_geometry_shader (const std::string &source)
{
#if DT3_OS != DT3_IOS && DT3_OS != DT3_ANDROID

    DT3OpenGLGeometryShaderResource *r = new DT3OpenGLGeometryShaderResource();

	// Create the shader object
	r->name = ::glCreateShader(GL_GEOMETRY_SHADER);
	if (r->name == 0) {
        delete r;
		return std::shared_ptr<DT3GLGeometryShaderResource>();
    }
		
	// load the _shader source
	const DTcharacter *s = source.c_str();
	::glShaderSource(r->name, 1, &s, NULL);
	
	// compile the _shader
	::glCompileShader(r->name);
	
	// Check the compile status
	GLint	compiled;
	::glGetShaderiv(r->name, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		GLint infoLen = 0;
		
		::glGetShaderiv(r->name, GL_INFO_LOG_LENGTH, &infoLen);
		
		if (infoLen > 1) {
			DTcharacter *infoLog = new ("ShaderLog") DTcharacter[infoLen];
			::glGetShaderInfoLog(r->name, infoLen, NULL, infoLog);
			
			LOG_MESSAGE << infoLog;
			
			delete[] infoLog;
		}
		
		::glDeleteShader(r->name);
        
        delete r;
		return std::shared_ptr<DT3GLGeometryShaderResource>();
	}

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLGeometryShaderResource>(r);
#else
    return std::shared_ptr<DT3GLGeometryShaderResource>();
#endif
}

std::shared_ptr<DT3GLFragmentShaderResource> DT3OpenGL::create_fragment_shader (const std::string &source)
{
    DT3OpenGLFragmentShaderResource *r = new DT3OpenGLFragmentShaderResource();

	// Create the shader object
	r->name = ::glCreateShader(GL_FRAGMENT_SHADER);
	if (r->name == 0) {
        delete r;
		return std::shared_ptr<DT3OpenGLFragmentShaderResource>();
    }
		
	// load the _shader source
	const DTcharacter *s = source.c_str();
	::glShaderSource(r->name, 1, &s, NULL);
	
	// compile the _shader
	::glCompileShader(r->name);
	
	// Check the compile status
	GLint	compiled;
	::glGetShaderiv(r->name, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		GLint infoLen = 0;
		
		::glGetShaderiv(r->name, GL_INFO_LOG_LENGTH, &infoLen);
		
		if (infoLen > 1) {
			DTcharacter *infoLog = new ("ShaderLog") DTcharacter[infoLen];
			::glGetShaderInfoLog(r->name, infoLen, NULL, infoLog);
			
			LOG_MESSAGE << infoLog;
			
			delete[] infoLog;
		}
		
		::glDeleteShader(r->name);

        delete r;
		return std::shared_ptr<DT3OpenGLFragmentShaderResource>();
	}

    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3OpenGLFragmentShaderResource>(r);
}

DTint DT3OpenGL::attribute_slot (const std::shared_ptr<DT3GLShaderResource> &shader, const std::string &name)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);

    ASSERT(::glGetError() == GL_NO_ERROR);
    
//    GLint save_p;
//    ::glGetIntegerv(GL_CURRENT_PROGRAM, &save_p);
//
//    ::glUseProgram(rs->name);
    GLint attrib = ::glGetAttribLocation(rs->name,name.c_str());
    
//    ::glUseProgram(save_p);

    return attrib;
}

DTint DT3OpenGL::uniform_slot (const std::shared_ptr<DT3GLShaderResource> &shader, const std::string &name)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);

    ASSERT(::glGetError() == GL_NO_ERROR);
    
//    GLint save_p;
//    ::glGetIntegerv(GL_CURRENT_PROGRAM, &save_p);
//
//    ::glUseProgram(rs->name);
    GLint attrib = ::glGetUniformLocation(rs->name,name.c_str());
    
//    ::glUseProgram(save_p);

    return attrib;
}


//==============================================================================
//         _   _             _                          _
//    /\  | | | |           | |                        | |
//   /  \ | |_| |_ __ _  ___| |__  _ __ ___   ___ _ __ | |_ ___
//  / /\ \| __| __/ _` |/ __| '_ \| '_ ` _ \ / _ \ '_ \| __/ __|
// / ____ \ |_| || (_| | (__| | | | | | | | |  __/ | | | |_\__ \
///_/    \_\__|\__\__,_|\___|_| |_|_| |_| |_|\___|_| |_|\__|___/
//==============================================================================
#pragma mark Attachments

void DT3OpenGL::attach_depth_stencil_state (const std::shared_ptr<DT3GLDepthStencilResource> &res)
{
    std::shared_ptr<DT3OpenGLDepthStencilResource> r = std::static_pointer_cast<DT3OpenGLDepthStencilResource>(res);
    _pending_depth_stencil_state = r;
}

void DT3OpenGL::attach_blend_state (const std::shared_ptr<DT3GLBlendResource> &res)
{
    std::shared_ptr<DT3OpenGLBlendResource> r = std::static_pointer_cast<DT3OpenGLBlendResource>(res);
    _pending_blend_state = r;
}

void DT3OpenGL::attach_rasterizer_state (const std::shared_ptr<DT3GLRasterizerResource> &res)
{
    std::shared_ptr<DT3OpenGLRasterizerResource> r = std::static_pointer_cast<DT3OpenGLRasterizerResource>(res);
    _pending_rasterizer_state = r;
}

void DT3OpenGL::attach_vertex_shader (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLVertexShaderResource> &vertex_shader)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);
    std::shared_ptr<DT3OpenGLVertexShaderResource> rv = std::static_pointer_cast<DT3OpenGLVertexShaderResource>(vertex_shader);

    ::glAttachShader(rs->name, rv->name);

    rs->vertex_shaders.push_back(rv);
}

void DT3OpenGL::attach_geometry_shader (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLGeometryShaderResource> &geometry_shader)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);
    std::shared_ptr<DT3OpenGLGeometryShaderResource> rg = std::static_pointer_cast<DT3OpenGLGeometryShaderResource>(geometry_shader);

    ::glAttachShader(rs->name, rg->name);

    rs->geometry_shaders.push_back(rg);
}

void DT3OpenGL::attach_fragment_shader (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLFragmentShaderResource> &fragment_shader)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);
    std::shared_ptr<DT3OpenGLFragmentShaderResource> rf = std::static_pointer_cast<DT3OpenGLFragmentShaderResource>(fragment_shader);

    ::glAttachShader(rs->name, rf->name);

    rs->fragment_shaders.push_back(rf);
}

void DT3OpenGL::attach_attribute_buffer (const std::shared_ptr<DT3GLShaderResource> &shader, DTuint attribute_slot, const std::shared_ptr<DT3GLAttribBufferResource> &res)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);
    std::shared_ptr<DT3OpenGLAttribBufferResource> ra = std::static_pointer_cast<DT3OpenGLAttribBufferResource>(res);

    rs->attributes[attribute_slot].attribute = ra;
}

void DT3OpenGL::attach_uniform_buffer (const std::shared_ptr<DT3GLShaderResource> &shader, DTuint uniform_slot, const std::shared_ptr<DT3GLUniformResource> &res)
{
    std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(shader);
    std::shared_ptr<DT3OpenGLUniformResource> ru = std::static_pointer_cast<DT3OpenGLUniformResource>(res);

    rs->uniforms[uniform_slot].uniform = ru;
}

//==============================================================================
//==============================================================================

void DT3OpenGL::attach_shader (std::shared_ptr<DT3GLShaderResource> shader)
{
    _pending_shader_state = shader;
}

//==============================================================================
//==============================================================================

void DT3OpenGL::attach_sampler (DTuint texture_slot, const std::shared_ptr<DT3GLSamplerResource> &res)
{
    _pending_sampler_state[texture_slot] = res;
}

void DT3OpenGL::attach_texture (DTuint texture_slot, const std::shared_ptr<DT3GLTexture2DResource> &res)
{
    _pending_texture_2d_state[texture_slot] = res;
    _pending_texture_3d_state[texture_slot].reset();
    _pending_texture_cube_state[texture_slot].reset();
}

void DT3OpenGL::attach_texture (DTuint texture_slot, const std::shared_ptr<DT3GLTexture3DResource> &res)
{
    _pending_texture_2d_state[texture_slot].reset();
    _pending_texture_3d_state[texture_slot] = res;
    _pending_texture_cube_state[texture_slot].reset();
}

void DT3OpenGL::attach_texture (DTuint texture_slot, const std::shared_ptr<DT3GLTextureCubeResource> &res)
{
    _pending_texture_2d_state[texture_slot].reset();
    _pending_texture_3d_state[texture_slot].reset();
    _pending_texture_cube_state[texture_slot] = res;
}


//==============================================================================
// _____                     _
//|  __ \                   (_)
//| |  | |_ __ __ ___      ___ _ __   __ _
//| |  | | '__/ _` \ \ /\ / / | '_ \ / _` |
//| |__| | | | (_| |\ V  V /| | | | | (_| |
//|_____/|_|  \__,_| \_/\_/ |_|_| |_|\__, |
//                                    __/ |
//                                   |___/
//==============================================================================
#pragma mark Drawing

void DT3OpenGL::draw_arrays (DT3GLPrimitiveType primitive_type, DTuint num_elements)
{
    sync_state();
    
    ::glDrawArrays(gPrimitiveType[primitive_type], 0, num_elements);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::draw_arrays_ranged (DT3GLPrimitiveType primitive_type, DTuint start_element, DTuint num_elements)
{
    sync_state();
    
    ::glDrawArrays(gPrimitiveType[primitive_type], start_element, num_elements);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
// ______                        _            __  __
//|  ____|                      | |          / _|/ _|
//| |__ _ __ __ _ _ __ ___   ___| |__  _   _| |_| |_ ___ _ __
//|  __| '__/ _` | '_ ` _ \ / _ \ '_ \| | | |  _|  _/ _ \ '__|
//| |  | | | (_| | | | | | |  __/ |_) | |_| | | | ||  __/ |
//|_|  |_|  \__,_|_| |_| |_|\___|_.__/ \__,_|_| |_| \___|_|
//==============================================================================
#pragma mark Framebuffer

std::shared_ptr<DT3GLFramebufferResource> DT3OpenGL::create_framebuffer (void)
{
    DT3OpenGLFramebufferResource *r = new DT3OpenGLFramebufferResource();

	::glGenFramebuffersM(1, &r->name);
    
    r->name_rb_color = 0;
    r->name_rb_depth = 0;
    
    ASSERT(::glGetError() == GL_NO_ERROR);

    return std::shared_ptr<DT3GLFramebufferResource>(r);
}

void DT3OpenGL::attach_framebuffer_color (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, const std::shared_ptr<DT3GLTexture2DResource> &tex)
{
    GLint save_fb;
    ::glGetIntegerv(GL_FRAMEBUFFER_BINDINGM, &save_fb);

    DT3OpenGLFramebufferResource *rfb = static_cast<DT3OpenGLFramebufferResource*>(framebuffer.get());
    DT3OpenGLTexture2DResource *rt = static_cast<DT3OpenGLTexture2DResource*>(tex.get());
    
    ::glBindFramebufferM(GL_FRAMEBUFFERM, rfb->name);
    ::glFramebufferTexture2DM(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0M, GL_TEXTURE_2D, rt->name, 0);

    ::glBindFramebufferM(GL_FRAMEBUFFERM, save_fb);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::attach_framebuffer_depth_stencil (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, const std::shared_ptr<DT3GLTexture2DResource> &tex)
{
    GLint save_fb;
    ::glGetIntegerv(GL_FRAMEBUFFER_BINDINGM, &save_fb);

    DT3OpenGLFramebufferResource *rfb = static_cast<DT3OpenGLFramebufferResource*>(framebuffer.get());
    DT3OpenGLTexture2DResource *rt = static_cast<DT3OpenGLTexture2DResource*>(tex.get());
    
    ::glBindFramebufferM(GL_FRAMEBUFFERM, rfb->name);
    ::glFramebufferTexture2DM(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt->name, 0);

    ::glBindFramebufferM(GL_FRAMEBUFFERM, save_fb);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::attach_renderbuffer_color (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, DTint width, DTint height, DT3GLRenderBufferFormat format)
{
    GLint save_fb;
    ::glGetIntegerv(GL_FRAMEBUFFER_BINDINGM, &save_fb);

    DT3OpenGLFramebufferResource *r = static_cast<DT3OpenGLFramebufferResource*>(framebuffer.get());

    GLenum color_fmt = 0;
    switch (format) {
        case DT3GL_FORMAT_RGBA:                 color_fmt = GL_RGBA;    break;
        
        default:
            break;
    }

    if (!r->name_rb_color)
        ::glGenRenderbuffersM(1, &r->name_rb_color);
    
    ::glBindRenderbufferM(GL_RENDERBUFFER, r->name_rb_color);
    ::glRenderbufferStorageM(GL_RENDERBUFFER, color_fmt, width, height);

    ::glBindFramebufferM(GL_FRAMEBUFFERM, r->name);
    ::glFramebufferRenderbufferM(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0M, GL_RENDERBUFFER, r->name_rb_color);

    ::glBindFramebufferM(GL_FRAMEBUFFERM, save_fb);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

void DT3OpenGL::attach_renderbuffer_depth_stencil (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, DTint width, DTint height, DT3GLRenderBufferFormat format)
{
    GLint save_fb;
    ::glGetIntegerv(GL_FRAMEBUFFER_BINDINGM, &save_fb);

    DT3OpenGLFramebufferResource *r = static_cast<DT3OpenGLFramebufferResource*>(framebuffer.get());

    GLenum depth_fmt = 0;
    switch (format) {
        case DT3GL_RB_FORMAT_DEPTH_16:              depth_fmt = GL_DEPTH_COMPONENT16M;  break;
        case DT3GL_RB_FORMAT_DEPTH_24:              depth_fmt = GL_DEPTH_COMPONENT24M;  break;
        case DT3GL_RB_FORMAT_DEPTH_24_STENCIL_8:    depth_fmt = GL_DEPTH24_STENCIL8M;   break;
        
        default:
            break;
    }
    
    if (!r->name_rb_depth)
        ::glGenRenderbuffersM(1, &r->name_rb_depth);
    
    ::glBindRenderbufferM(GL_RENDERBUFFER, r->name_rb_depth);
    ::glRenderbufferStorageM(GL_RENDERBUFFER, depth_fmt, width, height);        

    if (depth_fmt == DT3GL_FORMAT_DEPTH_24_STENCIL_8) {
        ::glFramebufferRenderbufferM(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, r->name_rb_depth);
        ::glFramebufferRenderbufferM(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, r->name_rb_depth);
    } else {
        ::glFramebufferRenderbufferM(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, r->name_rb_depth);
    }
    
    ::glBindFramebufferM(GL_FRAMEBUFFERM, save_fb);
    
    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
//==============================================================================

void DT3OpenGL::screenshot (const std::shared_ptr<TextureResource2D> &tex, const Rectangle &rect)
{

}

//==============================================================================
//==============================================================================

void DT3OpenGL::sync_state (void)
{

    //
    // Depth stencil State
    //
    
    ASSERT(::glGetError() == GL_NO_ERROR);

    if (_pending_depth_stencil_state && (_pending_depth_stencil_state != _current_depth_stencil_state)) {
        std::shared_ptr<DT3OpenGLDepthStencilResource> r = std::static_pointer_cast<DT3OpenGLDepthStencilResource>(_pending_depth_stencil_state);
        std::shared_ptr<DT3OpenGLDepthStencilResource> r_cur = std::static_pointer_cast<DT3OpenGLDepthStencilResource>(_current_depth_stencil_state);

        // Depth
//        if (!r_cur || (r_cur->state.depth_enable != r->state.depth_enable)) {
            if (r->state.depth_enable)          ::glEnable(GL_DEPTH_TEST);
            else                                ::glDisable(GL_DEPTH_TEST);
//        }

//        if (!r_cur || (r_cur->state.depth_write_mask != r->state.depth_write_mask)) {
            if (r->state.depth_write_mask)      ::glDepthMask(GL_TRUE);
            else                                ::glDepthMask(GL_FALSE);
//        }

//        if (!r_cur || (r_cur->state.depth_func != r->state.depth_func)) {
            ::glDepthFunc(gComparison[r->state.depth_func]);
//        }

        // Stencil
//        if (!r_cur || (r_cur->state.stencil_enable != r->state.stencil_enable)) {
            if (r->state.stencil_enable)        ::glEnable(GL_STENCIL_TEST);
            else                                ::glDisable(GL_STENCIL_TEST);
//        }

//        if (!r_cur || (r_cur->state.stencil_write_mask != r->state.stencil_write_mask)) {
            ::glStencilMask(r->state.stencil_write_mask);
//        }

//        if (!r_cur || (r_cur->state.front_face_stencil_fail_op != r->state.front_face_stencil_fail_op)
//                    || (r_cur->state.front_face_stencil_depth_fail_op != r->state.front_face_stencil_depth_fail_op)
//                    || (r_cur->state.front_face_stencil_pass_op != r->state.front_face_stencil_pass_op)) {
            ::glStencilOpSeparate(  GL_FRONT,
                                    gStencilOp[r->state.front_face_stencil_fail_op],
                                    gStencilOp[r->state.front_face_stencil_depth_fail_op],
                                    gStencilOp[r->state.front_face_stencil_pass_op]);
//        }

//        if (!r_cur || (r_cur->state.front_face_stencil_func != r->state.front_face_stencil_func)
//                    || (_current_stencil_ref != _pending_stencil_ref)
//                    || (r_cur->state.stencil_read_mask != r->state.stencil_read_mask)) {
            ::glStencilFuncSeparate(GL_FRONT,
                                    gComparison[r->state.front_face_stencil_func],
                                    _pending_stencil_ref,
                                    r->state.stencil_read_mask);
//        }

//        if (!r_cur || (r_cur->state.back_face_stencil_fail_op != r->state.back_face_stencil_fail_op)
//                    || (r_cur->state.back_face_stencil_depth_fail_op != r->state.back_face_stencil_depth_fail_op)
//                    || (r_cur->state.back_face_stencil_pass_op != r->state.back_face_stencil_pass_op)) {
            ::glStencilOpSeparate(  GL_BACK,
                                    gStencilOp[r->state.back_face_stencil_fail_op],
                                    gStencilOp[r->state.back_face_stencil_depth_fail_op],
                                    gStencilOp[r->state.back_face_stencil_pass_op]);
//        }


//        if (!r_cur || (r_cur->state.back_face_stencil_func != r->state.back_face_stencil_func)
//                    || (_current_stencil_ref != _pending_stencil_ref)
//                    || (r_cur->state.stencil_read_mask != r->state.stencil_read_mask)) {
            ::glStencilFuncSeparate(GL_BACK,
                                    gComparison[r->state.back_face_stencil_func],
                                    _pending_stencil_ref,
                                    r->state.stencil_read_mask);
//        }

        ASSERT(::glGetError() == GL_NO_ERROR);
        
        _current_depth_stencil_state = _pending_depth_stencil_state;
    }

    //
    // Blend state
    //
    
    ASSERT(::glGetError() == GL_NO_ERROR);
    
    if (_pending_blend_state && (_pending_blend_state != _current_blend_state)) {
        std::shared_ptr<DT3OpenGLBlendResource> r = std::static_pointer_cast<DT3OpenGLBlendResource>(_pending_blend_state);

        // Blend
        if (r->state.blend_enable)          ::glEnable(GL_BLEND);
        else                                ::glDisable(GL_BLEND);

        ::glBlendFuncSeparate(  gBlending[r->state.src_blend],
                                gBlending[r->state.dest_blend],
                                gBlending[r->state.src_blend_alpha],
                                gBlending[r->state.dest_blend_alpha]);

        ::glColorMask(  r->state.render_target_write_mask_r,
                        r->state.render_target_write_mask_g,
                        r->state.render_target_write_mask_b,
                        r->state.render_target_write_mask_a);
        
        ASSERT(::glGetError() == GL_NO_ERROR);

        _current_blend_state = _pending_blend_state;
    }

    //
    // Rasterizer State
    //

    ASSERT(::glGetError() == GL_NO_ERROR);

    if (_pending_rasterizer_state && (_pending_rasterizer_state != _current_rasterizer_state)) {
        std::shared_ptr<DT3OpenGLRasterizerResource> r = std::static_pointer_cast<DT3OpenGLRasterizerResource>(_pending_rasterizer_state);

        // Culling
        if (r->state.cull_mode == DT3GL_CULL_NONE) {
            ::glDisable(GL_CULL_FACE);
        } else {
            ::glEnable(GL_CULL_FACE);
            ::glCullFace(gCulling[r->state.cull_mode]);
        }

        ASSERT(::glGetError() == GL_NO_ERROR);

        // TODO Depth bias
        //::glPolygonOffset(factor, units)

        // Scissor
        if (r->state.scissor_enable)    ::glEnable(GL_SCISSOR_TEST);
        else                            ::glDisable(GL_SCISSOR_TEST);
        
        ASSERT(::glGetError() == GL_NO_ERROR);

        _current_rasterizer_state = _pending_rasterizer_state;
    }
    
    //
    // Shader state
    //
    
    ASSERT(::glGetError() == GL_NO_ERROR);

    if (_pending_shader_state && (_pending_shader_state != _current_shader_state)) {
        std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(_pending_shader_state);
    
        ::glUseProgram(rs->name);
    
        ASSERT(::glGetError() == GL_NO_ERROR);

        _current_shader_state = _pending_shader_state;
    }

    
    ASSERT(::glGetError() == GL_NO_ERROR);

    if (_pending_shader_state) {
    
        //
        // Attributes
        //

        std::shared_ptr<DT3OpenGLShaderResource> rs = std::static_pointer_cast<DT3OpenGLShaderResource>(_pending_shader_state);
    
        for (auto &i : rs->attributes) {
            const std::shared_ptr<DT3OpenGLAttribBufferResource> &attrib = i.second.attribute;
            
            ::glBindBuffer(GL_ARRAY_BUFFER, attrib->name);
            
            ::glEnableVertexAttribArray(i.first);
            
            switch (attrib->format) {
                case DT3GL_BUFFER_FORMAT_2_FLOAT:
                    ::glVertexAttribPointer(i.first, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
                    break;
                
                case DT3GL_BUFFER_FORMAT_3_FLOAT:
                    ::glVertexAttribPointer(i.first, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
                    break;
                    
                case DT3GL_BUFFER_FORMAT_4_FLOAT:
                    ::glVertexAttribPointer(i.first, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);
                    break;
                
                case DT3GL_BUFFER_FORMAT_4_UBYTE:
                    ::glVertexAttribPointer(i.first, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void *) 0);
                    break;
                    
                case DT3GL_BUFFER_FORMAT_4_USHORT:
                    ::glVertexAttribPointer(i.first, 4, GL_UNSIGNED_SHORT, GL_TRUE, 0, (void *) 0);
                    break;
            };
            
        }
        
        ASSERT(::glGetError() == GL_NO_ERROR);
    

        //
        // Uniforms
        //
        
        for (auto &i : rs->uniforms) {
            std::shared_ptr<DT3OpenGLUniformResource> uniform = i.second.uniform;

            // Check if already set
            if (uniform->use_stamp == i.second.use_stamp)
                continue;
            uniform->use_stamp = i.second.use_stamp;

            DTint index = i.first;
            
            switch(uniform->format) {
                case DT3GL_UNIFORM_FORMAT_1_INT:
                    ::glUniform1iv(index, uniform->num, (const GLint*) &(uniform->data[0]));
                    break;
                
                case DT3GL_UNIFORM_FORMAT_4_FLOAT:
                    ::glUniform4fv(index, uniform->num, (const GLfloat*) &(uniform->data[0]));
                    break;
                
                case DT3GL_UNIFORM_FORMAT_16_FLOAT:
                    ::glUniformMatrix4fv(index, uniform->num, GL_FALSE, (const GLfloat *) &(uniform->data[0]));
                    break;
                
            }

            ASSERT(::glGetError() == GL_NO_ERROR);
        }
        
        ASSERT(::glGetError() == GL_NO_ERROR);
    }
    
    //
    // Textures
    //
    
    ASSERT(::glGetError() == GL_NO_ERROR);

    for (DTint i = 0; i < 16; ++i) {
        
        //
        // Set active texture unit
        //
        
        ASSERT(::glGetError() == GL_NO_ERROR);

        // Activate texture
        if (_pending_texture_2d_state[i] && (_pending_texture_2d_state[i] != _current_texture_2d_state[i])) {
            std::shared_ptr<DT3OpenGLTexture2DResource> r = std::static_pointer_cast<DT3OpenGLTexture2DResource>(_pending_texture_2d_state[i]);

            ::glActiveTexture(GL_TEXTURE0 + i);
            ASSERT(::glGetError() == GL_NO_ERROR);

            ::glBindTexture(GL_TEXTURE_2D, r->name);
            ASSERT(::glGetError() == GL_NO_ERROR);
            
            if (r->sampler != _pending_sampler_state[i]) {
                std::shared_ptr<DT3OpenGLSamplerResource> rs = std::static_pointer_cast<DT3OpenGLSamplerResource>(_pending_sampler_state[i]);
                
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gFilterMinMode[rs->state.filter]);
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gFilterMagMode[rs->state.filter]);
                ASSERT(::glGetError() == GL_NO_ERROR);

                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gWrap[rs->state.address_u]);
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gWrap[rs->state.address_v]);
                ASSERT(::glGetError() == GL_NO_ERROR);

#if DT3_OS != DT3_ANDROID
                ::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::max(1.0F,rs->state.max_anisotropy) );
                 ASSERT(::glGetError() == GL_NO_ERROR);
#endif
                r->sampler = rs;
            }
            
            ASSERT(::glGetError() == GL_NO_ERROR);
            
            // Copy current states
            _current_texture_2d_state[i] = _pending_texture_2d_state[i];
            _current_texture_3d_state[i] = nullptr;
            _current_texture_cube_state[i] = nullptr;

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
        } else if (_pending_texture_3d_state[i] && (_pending_texture_3d_state[i] != _current_texture_3d_state[i])) {
            std::shared_ptr<DT3OpenGLTexture3DResource> r = std::static_pointer_cast<DT3OpenGLTexture3DResource>(_pending_texture_3d_state[i]);

            ::glActiveTexture(GL_TEXTURE0 + i);
            ::glBindTexture(GL_TEXTURE_3D, r->name);
            
            if (r->sampler != _pending_sampler_state[i]) {
                std::shared_ptr<DT3OpenGLSamplerResource> rs = std::static_pointer_cast<DT3OpenGLSamplerResource>(_pending_sampler_state[i]);

                ::glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, gFilterMinMode[rs->state.filter]);
                ::glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, gFilterMagMode[rs->state.filter]);

                ::glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, gWrap[rs->state.address_u]);
                ::glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, gWrap[rs->state.address_v]);
                ::glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, gWrap[rs->state.address_w]);

                ::glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::max(1.0F,rs->state.max_anisotropy) );

                r->sampler = rs;
            }

            ASSERT(::glGetError() == GL_NO_ERROR);
            
            // Copy current states
            _current_texture_2d_state[i] = nullptr;
            _current_texture_3d_state[i] = _pending_texture_3d_state[i];
            _current_texture_cube_state[i] = nullptr;

#endif
        } else if (_pending_texture_cube_state[i] && (_pending_texture_cube_state[i] != _current_texture_cube_state[i])) {
            std::shared_ptr<DT3OpenGLTextureCubeResource> r = std::static_pointer_cast<DT3OpenGLTextureCubeResource>(_pending_texture_cube_state[i]);

            ::glActiveTexture(GL_TEXTURE0 + i);
            ::glBindTexture(GL_TEXTURE_CUBE_MAPM, r->name);

            if (r->sampler != _pending_sampler_state[i]) {
                std::shared_ptr<DT3OpenGLSamplerResource> rs = std::static_pointer_cast<DT3OpenGLSamplerResource>(_pending_sampler_state[i]);

                ::glTexParameteri(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_MIN_FILTER, gFilterMinMode[rs->state.filter]);
                ::glTexParameteri(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_MAG_FILTER, gFilterMagMode[rs->state.filter]);

                ::glTexParameteri(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_WRAP_S, gWrap[rs->state.address_u]);
                ::glTexParameteri(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_WRAP_T, gWrap[rs->state.address_v]);

#if DT3_OS != DT3_IOS && DT3_OS != DT3_ANDROID
                ::glTexParameteri(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_WRAP_R, gWrap[rs->state.address_w]);
#endif
                ::glTexParameterf(GL_TEXTURE_CUBE_MAPM, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::max(1.0F,rs->state.max_anisotropy) );

                r->sampler = rs;
            }

            ASSERT(::glGetError() == GL_NO_ERROR);

            // Copy current states
            _current_texture_2d_state[i] = nullptr;
            _current_texture_3d_state[i] = nullptr;
            _current_texture_cube_state[i] = _pending_texture_cube_state[i];

        }
        


        //
        // Sampler
        //
       
        
        if (_pending_sampler_state[i] != _current_sampler_state[i]) {
            std::shared_ptr<DT3OpenGLSamplerResource> r = std::static_pointer_cast<DT3OpenGLSamplerResource>(_pending_sampler_state[i]);
            
#if DT3_OS != DT3_IOS && DT3_OS != DT3_ANDROID
            // Env
            ::glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, r->state.mip_lod_bias);
#endif

            ASSERT(::glGetError() == GL_NO_ERROR);

            // Copy current states
            _current_sampler_state[i] = _pending_sampler_state[i];
        }
        
        
        ASSERT(::glGetError() == GL_NO_ERROR);
    }

    ASSERT(::glGetError() == GL_NO_ERROR);
}

//==============================================================================
//==============================================================================

} // DT3

