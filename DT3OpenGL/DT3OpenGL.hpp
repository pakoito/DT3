#ifndef DT3_DT3OPENGL
#define DT3_DT3OPENGL
//==============================================================================
///	
///	File: DT3OpenGL.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3OpenGL/DT3OpenGLResources.hpp"
#include <memory>
#include <list>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class LatentCall;
class Color4f;

//==============================================================================
/// Class
//==============================================================================

class DT3OpenGL: public DeviceGraphics {
		/// Multithreaded renderer implementation.

	public:
        DEFINE_TYPE(DT3OpenGL,DeviceGraphics)
		DEFINE_CREATE

                                DT3OpenGL                       (void);
                                DT3OpenGL                       (const DT3OpenGL&);
		DT3OpenGL &             operator =                      (const DT3OpenGL&);
                                ~DT3OpenGL                      (void);
    				
	public:            
    
		/// Screen context is ready
		virtual void                                        init_display                    (void);

		/// Opens up a display or tells the Device that a new display was opened
		/// \param width width of the display
		/// \param height width of the display
		virtual void                                        open_display                    (DTint width, DTint height);

		/// Changes a display or tells the Device that a display was changed
		/// \param width width of the display
		/// \param height width of the display
		virtual void                                        change_display                  (DTuint width, DTuint height);

		/// Closes a display or tells the Device that a display was closed
		virtual void                                        close_display                   (void);
				
		/// Returns the width of the screen in pixels
		/// \return width of screen in pixels
		virtual DTuint                                      screen_width                    (void);

		/// Returns the height of the screen in pixels
		/// \return height of screen in pixels
		virtual DTuint                                      screen_height                   (void);

		/// Returns the aspect ratio of the screen in pixels
		/// \return aspect ratio of screen in pixels
		virtual DTfloat                                     screen_aspect                   (void);

		
		/// Returns the current viewport
		/// \param x x
		/// \param y y
		/// \param width width
		/// \param height height
		virtual void                                        viewport                        (DTint &x, DTint &y, DTint &width, DTint &height);

		/// Sets the current viewport
		/// \param x x
		/// \param y y
		/// \param width width
		/// \param height height
		/// \param rt_width width of render target. Needed for compatibility with D3D. 0 for screen dimensions
		/// \param rt_height height of render target. Needed for compatibility with D3D. 0 for screen dimensions
		virtual void                                        set_viewport                    (DTint x, DTint y, DTint width, DTint height, DTint rt_width, DTint rt_height);

		/// Returns the current viewport width
		/// \return viewport width
		virtual DTuint                                      viewport_width                  (void);

		/// Returns the current viewport height
		/// \return viewport height
		virtual DTuint                                      viewport_height                 (void);

		/// Returns the current viewport aspect ratio
		/// \return viewport aspect ratio
		virtual DTfloat                                     viewport_aspect                 (void);
		
    

		/// Clears the viewport
		virtual void                                        clear_viewport                  (DTboolean depth, DTboolean color, DTboolean stencil);


		/// Viewport clear values
        virtual void                                        clear_stencil_value             (DTuint s);
        virtual void                                        clear_color_value               (const Color4f &c);
        virtual void                                        clear_depth_value               (DTfloat d);


        //
        // Depth Stencil State
        //

        virtual std::shared_ptr<DT3GLDepthStencilResource>  create_depth_stencil_state      (const DT3GLDepthStencilState &state);
        virtual void                                        set_stencil_ref                 (DTuint ref);

        //
        // Blend State
        //
        
        virtual std::shared_ptr<DT3GLBlendResource>         create_blend_state              (const DT3GLBlendState &state);

        //
        // Rasterizer State
        //

        virtual std::shared_ptr<DT3GLRasterizerResource>    create_rasterizer_state         (const DT3GLRasterizerState &state);

        //
        // Sampler State
        //

        virtual std::shared_ptr<DT3GLSamplerResource>       create_sampler_state            (const DT3GLSamplerState &state);
    
        //
        // Textures
        //

        virtual std::shared_ptr<DT3GLTexture2DResource>     create_texture_2D               (DTint width, DTint height, DTubyte *textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);
        virtual void                                        update_texture_2D               (const std::shared_ptr<DT3GLTexture2DResource> &res, DTint x, DTint y, DTint width, DTint height, DTubyte *textels);
    
        virtual std::shared_ptr<DT3GLTexture3DResource>     create_texture_3D               (DTint width, DTint height, DTint depth, DTubyte *textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);
        virtual void                                        update_texture_3D               (const std::shared_ptr<DT3GLTexture3DResource> &res, DTint x, DTint y, DTint z, DTint width, DTint height, DTint depth, DTubyte *textels);
    
        virtual std::shared_ptr<DT3GLTextureCubeResource>   create_texture_cube             (   DTint width, DTint height,
                                                                                                DTubyte *textels_front,
                                                                                                DTubyte *textels_back,
                                                                                                DTubyte *textels_left,
                                                                                                DTubyte *textels_right,
                                                                                                DTubyte *textels_top,
                                                                                                DTubyte *textels_bottom,
                                                                                                DT3GLTextelFormat format,
                                                                                                DTboolean mipmapped,
                                                                                                DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);
        virtual void                                        update_texture_cube             (   const std::shared_ptr<DT3GLTextureCubeResource> &res, DTint x, DTint y, DTint width, DTint height,
                                                                                                DTubyte *textels_front,
                                                                                                DTubyte *textels_back,
                                                                                                DTubyte *textels_left,
                                                                                                DTubyte *textels_right,
                                                                                                DTubyte *textels_top,
                                                                                                DTubyte *textels_bottom);
    
        //
        // Buffers
        //
    
        virtual std::shared_ptr<DT3GLAttribBufferResource>  create_buffer                   (DTubyte *buffer_data, DTsize buffer_size, DT3GLBufferFormat buffer_format, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);
        virtual void                                        update_buffer                   (const std::shared_ptr<DT3GLAttribBufferResource> &res, DTubyte *buffer_data, DTsize buffer_size, DTsize buffer_offset);
    
        //
        // Uniforms
        //
    
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const DTint buffer_data);
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const Vector4 &buffer_data);
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const std::vector<Vector4> &buffer_data);
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const Matrix4 &buffer_data);
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const std::vector<Matrix4> &buffer_data);
        virtual std::shared_ptr<DT3GLUniformResource>       create_uniform                  (const Color4f &buffer_data);

        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const DTint buffer_data);
        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const Vector4 &buffer_data);
        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const std::vector<Vector4> &buffer_data);
        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const Matrix4 &buffer_data);
        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const std::vector<Matrix4> &buffer_data);
        virtual void                                        update_uniform                  (const std::shared_ptr<DT3GLUniformResource> &res, const Color4f &buffer_data);

        //
        // Shaders
        //
    
        virtual std::shared_ptr<DT3GLShaderResource>        create_shader                   (void);
        virtual void                                        link_shader                     (const std::shared_ptr<DT3GLShaderResource> &shader);
    
        virtual std::shared_ptr<DT3GLVertexShaderResource>  create_vertex_shader            (const std::string &source);
        virtual std::shared_ptr<DT3GLGeometryShaderResource>create_geometry_shader          (const std::string &source);
        virtual std::shared_ptr<DT3GLFragmentShaderResource>create_fragment_shader          (const std::string &source);

        virtual DTint                                       attribute_slot                  (const std::shared_ptr<DT3GLShaderResource> &shader, const std::string &name);
        virtual DTint                                       uniform_slot                    (const std::shared_ptr<DT3GLShaderResource> &shader, const std::string &name);

        //
        // Attachments
        //
    
        virtual void                                        attach_depth_stencil_state      (const std::shared_ptr<DT3GLDepthStencilResource> &res);
        virtual void                                        attach_blend_state              (const std::shared_ptr<DT3GLBlendResource> &res);
        virtual void                                        attach_rasterizer_state         (const std::shared_ptr<DT3GLRasterizerResource> &res);
    
        virtual void                                        attach_vertex_shader            (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLVertexShaderResource> &vertex_shader);
        virtual void                                        attach_geometry_shader          (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLGeometryShaderResource> &geometry_shader);
        virtual void                                        attach_fragment_shader          (const std::shared_ptr<DT3GLShaderResource> &shader, const std::shared_ptr<DT3GLFragmentShaderResource> &fragment_shader);
        virtual void                                        attach_attribute_buffer         (const std::shared_ptr<DT3GLShaderResource> &shader, DTuint attribute_slot, const std::shared_ptr<DT3GLAttribBufferResource> &res);
        virtual void                                        attach_uniform_buffer           (const std::shared_ptr<DT3GLShaderResource> &shader, DTuint uniform_slot, const std::shared_ptr<DT3GLUniformResource> &res);

        virtual void                                        attach_shader                   (std::shared_ptr<DT3GLShaderResource> shader);
    
        virtual void                                        attach_sampler                  (DTuint texture_slot, const std::shared_ptr<DT3GLSamplerResource> &res);
        virtual void                                        attach_texture                  (DTuint texture_slot, const std::shared_ptr<DT3GLTexture2DResource> &res);
        virtual void                                        attach_texture                  (DTuint texture_slot, const std::shared_ptr<DT3GLTexture3DResource> &res);
        virtual void                                        attach_texture                  (DTuint texture_slot, const std::shared_ptr<DT3GLTextureCubeResource> &res);
    
        //
        // Drawing
        //

        virtual void                                        draw_arrays                     (DT3GLPrimitiveType primitive_type, DTuint num_elements);
        virtual void                                        draw_arrays_ranged              (DT3GLPrimitiveType primitive_type, DTuint start_element, DTuint num_elements);

        //
        // Framebuffers
        //
    
        virtual std::shared_ptr<DT3GLFramebufferResource>   create_framebuffer              (void);
    
        virtual void                                        attach_framebuffer_color        (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, const std::shared_ptr<DT3GLTexture2DResource> &tex);
        virtual void                                        attach_framebuffer_depth_stencil(const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, const std::shared_ptr<DT3GLTexture2DResource> &tex);
    
        virtual void                                        attach_renderbuffer_color       (const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, DTint width, DTint height, DT3GLRenderBufferFormat format);
        virtual void                                        attach_renderbuffer_depth_stencil(const std::shared_ptr<DT3GLFramebufferResource> &framebuffer, DTint width, DTint height, DT3GLRenderBufferFormat format);

		/// Copies the current screen to a TextureResource2D
		/// \param tex destination texture
		/// \param rect source rectangle
        virtual void                                        screenshot                      (const std::shared_ptr<TextureResource2D> &tex, const Rectangle &rect);
    
    private:
        void                                                sync_state                      (void);
    
		DTuint                                              _width;
		DTuint                                              _height;
    
		DTuint                                              _viewport_x;
		DTuint                                              _viewport_y;
		DTuint                                              _viewport_width;
		DTuint                                              _viewport_height;

        DTint                                               _default_framebuffer;

        DTuint                                              _use_stamp;
    
    
        // All states
        std::map<DTuint, std::shared_ptr<DT3GLDepthStencilResource>>    _depth_stencil_states;
        std::map<DTuint, std::shared_ptr<DT3GLBlendResource>>           _blend_states;
        std::map<DTuint, std::shared_ptr<DT3GLRasterizerResource>>      _rasterizer_states;
        std::map<DTuint, std::shared_ptr<DT3GLSamplerResource>>         _sampler_states;
    
    
        // State
        std::shared_ptr<DT3GLDepthStencilResource>          _current_depth_stencil_state,
                                                            _pending_depth_stencil_state;
    
        std::shared_ptr<DT3GLBlendResource>                 _current_blend_state,
                                                            _pending_blend_state;
    
        std::shared_ptr<DT3GLRasterizerResource>            _current_rasterizer_state,
                                                            _pending_rasterizer_state;
    
        std::shared_ptr<DT3GLSamplerResource>               _current_sampler_state[16],
                                                            _pending_sampler_state[16];


        std::shared_ptr<DT3GLShaderResource>                _current_shader_state,
                                                            _pending_shader_state;
    
    
        std::shared_ptr<DT3GLTexture2DResource>             _current_texture_2d_state[16],
                                                            _pending_texture_2d_state[16];
    
        std::shared_ptr<DT3GLTexture3DResource>             _current_texture_3d_state[16],
                                                            _pending_texture_3d_state[16];
    
        std::shared_ptr<DT3GLTextureCubeResource>           _current_texture_cube_state[16],
                                                            _pending_texture_cube_state[16];

        // Not part of any state
        DTuint                                              _current_stencil_ref,
                                                            _pending_stencil_ref;
    
        Matrix4                                             _current_texture_matrix[16],
                                                            _pending_texture_matrix[16];
};

//==============================================================================
//==============================================================================

} // DT3

#endif
