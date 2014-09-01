#ifndef DT3_DT3OPENGLRESOURCES
#define DT3_DT3OPENGLRESOURCES
//==============================================================================
///	
///	File: 			DT3OpenGLResources.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <map>
#include <list>

#if DT3_OS == DT3_ANDROID
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#elif DT3_OS == DT3_IOS
	#include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GL/glew.h>
#endif


#if DT3_OS == DT3_ANDROID
    
    #define GL_DEPTH_COMPONENT24M           GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH_COMPONENT16M           GL_DEPTH_COMPONENT16
    #define GL_DEPTH24_STENCIL8M            GL_DEPTH24_STENCIL8_OES
    #define GL_TEXTURE_CUBE_MAPM            GL_TEXTURE_CUBE_MAP

    #define GL_TEXTURE_CUBE_MAP_POSITIVE_ZM GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_ZM GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_YM GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_YM GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_XM GL_TEXTURE_CUBE_MAP_POSITIVE_X
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_XM GL_TEXTURE_CUBE_MAP_NEGATIVE_X

    #define glGenerateMipmapM               glGenerateMipmap

    #define GL_BGRAM                        GL_BGRA_EXT
    
    
    #define glGenFramebuffersM          glGenFramebuffers
    #define glBindFramebufferM          glBindFramebuffer
    #define glGenRenderbuffersM         glGenRenderbuffers
    #define glBindRenderbufferM         glBindRenderbuffer
    #define glRenderbufferStorageM      glRenderbufferStorage
    #define glFramebufferRenderbufferM  glFramebufferRenderbuffer
    #define glFramebufferTexture2DM     glFramebufferTexture2D
    #define glCheckFramebufferStatusM   glCheckFramebufferStatus
    #define glDeleteRenderbuffersM      glDeleteRenderbuffers
    #define glDeleteFramebuffersM       glDeleteFramebuffers
    
    #define GL_COLOR_ATTACHMENT0M       GL_COLOR_ATTACHMENT0
    #define GL_DEPTH_ATTACHMENTM        GL_DEPTH_ATTACHMENT
    #define GL_FRAMEBUFFERM             GL_FRAMEBUFFER
    #define GL_FRAMEBUFFER_COMPLETEM    GL_FRAMEBUFFER_COMPLETE
    #define GL_FRAMEBUFFER_BINDINGM     GL_FRAMEBUFFER_BINDING

    #define GL_DEPTH_COMPONENT24M       GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH_COMPONENT16M       GL_DEPTH_COMPONENT16
    #define GL_DEPTH24_STENCIL8M        GL_DEPTH24_STENCIL8_OES


#elif DT3_OS == DT3_IOS

    #define GL_DEPTH_COMPONENT24M           GL_DEPTH_COMPONENT
    #define GL_DEPTH_COMPONENT16M           GL_DEPTH_COMPONENT
    #define GL_DEPTH24_STENCIL8M            GL_DEPTH24_STENCIL8_OES
    #define GL_TEXTURE_CUBE_MAPM            GL_TEXTURE_CUBE_MAP

    #define GL_TEXTURE_CUBE_MAP_POSITIVE_ZM GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_ZM GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_YM GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_YM GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_XM GL_TEXTURE_CUBE_MAP_POSITIVE_X
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_XM GL_TEXTURE_CUBE_MAP_NEGATIVE_X

    #define glGenerateMipmapM               glGenerateMipmap

    #define GL_BGRAM                        GL_BGRA

    #define glGenFramebuffersM          glGenFramebuffers
    #define glBindFramebufferM          glBindFramebuffer
    #define glGenRenderbuffersM         glGenRenderbuffers
    #define glBindRenderbufferM         glBindRenderbuffer
    #define glRenderbufferStorageM      glRenderbufferStorage
    #define glFramebufferRenderbufferM  glFramebufferRenderbuffer
    #define glFramebufferTexture2DM     glFramebufferTexture2D
    #define glCheckFramebufferStatusM   glCheckFramebufferStatus
    #define glDeleteRenderbuffersM      glDeleteRenderbuffers
    #define glDeleteFramebuffersM       glDeleteFramebuffers

    #define GL_COLOR_ATTACHMENT0M       GL_COLOR_ATTACHMENT0
    #define GL_DEPTH_ATTACHMENTM        GL_DEPTH_ATTACHMENT
    #define GL_FRAMEBUFFERM             GL_FRAMEBUFFER
    #define GL_FRAMEBUFFER_COMPLETEM    GL_FRAMEBUFFER_COMPLETE
    #define GL_FRAMEBUFFER_BINDINGM     GL_FRAMEBUFFER_BINDING

    #define GL_DEPTH_COMPONENT24M       GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH_COMPONENT16M       GL_DEPTH_COMPONENT16
    #define GL_DEPTH24_STENCIL8M        GL_DEPTH24_STENCIL8_OES

#else

    #define GL_DEPTH_COMPONENT24M           GL_DEPTH_COMPONENT24
    #define GL_DEPTH_COMPONENT16M           GL_DEPTH_COMPONENT16
    #define GL_DEPTH24_STENCIL8M            GL_DEPTH24_STENCIL8
    #define GL_TEXTURE_CUBE_MAPM            GL_TEXTURE_CUBE_MAP_ARB

    #define GL_TEXTURE_CUBE_MAP_POSITIVE_ZM GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_ZM GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_YM GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_YM GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_XM GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_XM GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT

    #define glGenerateMipmapM               glGenerateMipmapEXT

    #define GL_BGRAM                        GL_BGRA

    #define glGenFramebuffersM          glGenFramebuffersEXT
    #define glBindFramebufferM          glBindFramebufferEXT
    #define glGenRenderbuffersM         glGenRenderbuffersEXT
    #define glBindRenderbufferM         glBindRenderbufferEXT
    #define glRenderbufferStorageM      glRenderbufferStorageEXT
    #define glFramebufferRenderbufferM  glFramebufferRenderbufferEXT
    #define glFramebufferTexture2DM     glFramebufferTexture2DEXT
    #define glCheckFramebufferStatusM   glCheckFramebufferStatusEXT
    #define glDeleteRenderbuffersM      glDeleteRenderbuffersEXT
    #define glDeleteFramebuffersM       glDeleteFramebuffersEXT

    #define GL_COLOR_ATTACHMENT0M       GL_COLOR_ATTACHMENT0_EXT
    #define GL_DEPTH_ATTACHMENTM        GL_DEPTH_ATTACHMENT_EXT
    #define GL_FRAMEBUFFERM             GL_FRAMEBUFFER_EXT
    #define GL_FRAMEBUFFER_COMPLETEM    GL_FRAMEBUFFER_COMPLETE_EXT
    #define GL_FRAMEBUFFER_BINDINGM     GL_FRAMEBUFFER_BINDING_EXT

    #define GL_DEPTH_COMPONENT24M       GL_DEPTH_COMPONENT24
    #define GL_DEPTH_COMPONENT16M       GL_DEPTH_COMPONENT16
    #define GL_DEPTH24_STENCIL8M        GL_DEPTH24_STENCIL8

#endif


//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

struct DT3OpenGLDepthStencilResource: public DT3GLDepthStencilResource {
    public:
                    DT3OpenGLDepthStencilResource     (void)  {}
        virtual     ~DT3OpenGLDepthStencilResource    (void)  {}

    public:
        DT3GLDepthStencilState          state;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLBlendResource: public DT3GLBlendResource {
    public:
                    DT3OpenGLBlendResource            (void)  {}
        virtual     ~DT3OpenGLBlendResource           (void)  {}

    public:
        DT3GLBlendState                 state;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLRasterizerResource: public DT3GLRasterizerResource {
    public:
                    DT3OpenGLRasterizerResource       (void)  {}
        virtual     ~DT3OpenGLRasterizerResource      (void)  {}

    public:
        DT3GLRasterizerState            state;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLSamplerResource: public DT3GLSamplerResource {
    public:
                    DT3OpenGLSamplerResource          (void)  {}
        virtual     ~DT3OpenGLSamplerResource         (void)  {}

    public:
        DT3GLSamplerState               state;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLTexture2DResource: public DT3GLTexture2DResource {
    public:
                    DT3OpenGLTexture2DResource        (void)    {}
        virtual     ~DT3OpenGLTexture2DResource       (void)    {   ::glDeleteTextures (1, &name);  }

    public:
        DTuint                          flags;
        GLuint                          name;
        DT3GLTextelFormat               format;
        DTboolean                       mipmapped;
    
        std::shared_ptr<DT3OpenGLSamplerResource>   sampler;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLTexture3DResource: public DT3GLTexture3DResource {
    public:
                    DT3OpenGLTexture3DResource        (void)    {}
        virtual     ~DT3OpenGLTexture3DResource       (void)    {   ::glDeleteTextures (1, &name);  }

    public:
        DTuint                          flags;
        GLuint                          name;
        DT3GLTextelFormat               format;
        DTboolean                       mipmapped;

        std::shared_ptr<DT3OpenGLSamplerResource>   sampler;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLTextureCubeResource: public DT3GLTextureCubeResource {
    public:
                    DT3OpenGLTextureCubeResource    (void)      {}
        virtual     ~DT3OpenGLTextureCubeResource   (void)      {   ::glDeleteTextures (1, &name);    }

    public:
        DTuint                          flags;
        GLuint                          name;
        DT3GLTextelFormat               format;
        DTboolean                       mipmapped;

        std::shared_ptr<DT3OpenGLSamplerResource>   sampler;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLAttribBufferResource: public DT3GLAttribBufferResource {
    public:
                    DT3OpenGLAttribBufferResource   (void)  {}
        virtual     ~DT3OpenGLAttribBufferResource  (void)  {   ::glDeleteBuffers(1, &name); }

    public:
        DTuint                          flags;
        DT3GLBufferFormat               format;
        GLuint                          name;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLUniformResource: public DT3GLUniformResource {
    public:
                    DT3OpenGLUniformResource        (void)  {}
        virtual     ~DT3OpenGLUniformResource       (void)  {}

    public:
        DT3GLUniformFormat              format;
        DTint                           num;

        DTuint                          use_stamp;  // For tracking changes
    
        std::vector<DTubyte>            data;

};

//==============================================================================
//==============================================================================

struct DT3OpenGLVertexShaderResource: public DT3GLVertexShaderResource {
    public:
                    DT3OpenGLVertexShaderResource     (void)  {}
        virtual     ~DT3OpenGLVertexShaderResource    (void)  {   ::glDeleteShader(name);  }

    public:
        GLuint                          name;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLGeometryShaderResource: public DT3GLGeometryShaderResource {
    public:
                    DT3OpenGLGeometryShaderResource   (void)  {}
        virtual     ~DT3OpenGLGeometryShaderResource  (void)  {   ::glDeleteShader(name);  }

    public:
        GLuint                          name;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLFragmentShaderResource: public DT3GLFragmentShaderResource {
    public:
                    DT3OpenGLFragmentShaderResource   (void)  {}
        virtual     ~DT3OpenGLFragmentShaderResource  (void)  {   ::glDeleteShader(name);  }

    public:
        GLuint                          name;
};

//==============================================================================
//==============================================================================

struct DT3OpenGLShaderResource: public DT3GLShaderResource {
    public:
                    DT3OpenGLShaderResource           (void)  {}
        virtual     ~DT3OpenGLShaderResource          (void)  {   ::glDeleteShader(name);  }

    public:
        GLuint                          name;
    
        std::list<std::shared_ptr<DT3OpenGLVertexShaderResource>>           vertex_shaders;
        std::list<std::shared_ptr<DT3OpenGLGeometryShaderResource>>         geometry_shaders;
        std::list<std::shared_ptr<DT3OpenGLFragmentShaderResource>>         fragment_shaders;

        struct Uniform {
            Uniform()
                :   use_stamp(0)
            {}

            std::shared_ptr<DT3OpenGLUniformResource>       uniform;
            DTuint                                          use_stamp;
        };

        std::map<DTint, Uniform>                            uniforms;

        struct Attribute {
            Attribute()
            {}

            std::shared_ptr<DT3OpenGLAttribBufferResource>  attribute;
        };

        std::map<DTint, Attribute>                          attributes;


};

//==============================================================================
//==============================================================================

struct DT3OpenGLFramebufferResource: public DT3GLFramebufferResource {
    public:
                    DT3OpenGLFramebufferResource      (void)  {}
        virtual     ~DT3OpenGLFramebufferResource     (void)  {
            ::glDeleteFramebuffersM(1, &name);
            ::glDeleteRenderbuffersM(1, &name_rb_color);
            ::glDeleteRenderbuffersM(1, &name_rb_depth);
        }

    public:
        GLuint                          name;
        GLuint                          name_rb_color;
        GLuint                          name_rb_depth;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
