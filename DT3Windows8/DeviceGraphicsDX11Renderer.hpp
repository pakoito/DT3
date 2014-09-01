#ifndef DT2_DEVICEGRAPHICSDX11RENDERER
#define DT2_DEVICEGRAPHICSDX11RENDERER
//==============================================================================
///	
///	File: DeviceGraphicsDX11Renderer.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "DeviceGraphics.hpp"
#include "Cache.hpp"
#include "Array.hpp"
#include "DisplayMode.hpp"
#include "Matrix.hpp"
#include "Plane.hpp"

#include "DeviceGraphicsDX11Geometry.hpp"
#include "DeviceGraphicsDX11Texture.hpp"
#include "DeviceGraphicsDX11Material.hpp"
#include "DeviceGraphicsDX11Particles.hpp"
#include "DeviceGraphicsDX11PixelShader.hpp"
#include "DeviceGraphicsDX11VertexShader.hpp"
#include "DeviceGraphicsDX11Shader.hpp"
#include "DeviceGraphicsDX11Framebuffer.hpp"

class ID3D11Device1;
class ID3D11DeviceContext1;
class IDXGISwapChain1;

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;

class GeometryResource;
class TextureResource;
class MaterialResource;
class VertexShaderResource;
class FragmentShaderResource;
class GeometryShaderResource;
class ShaderResource;

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11Renderer: public DeviceGraphics {
		/// OpenGL based renderer.

    public:
        DEFINE_TYPE(DeviceGraphicsDX11Renderer,DeviceGraphics)
		DEFINE_CREATE

									DeviceGraphicsDX11Renderer	(void);
	private:
									DeviceGraphicsDX11Renderer	(const DeviceGraphicsDX11Renderer&);
		DeviceGraphicsDX11Renderer&	operator =			        (const DeviceGraphicsDX11Renderer&);
	public:
        virtual						~DeviceGraphicsDX11Renderer	(void);
    				
	public:			
	
		// Gets the list of display modes for this renderer
		/// \param modes returns the available display modes
		virtual void				getDisplayModes             (std::vector<DisplayMode> &modes);

        struct OpenDisplayData {
            Microsoft::WRL::ComPtr<ID3D11Device1>           d3dDevice;
	        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    d3dContext;
	        Microsoft::WRL::ComPtr<IDXGISwapChain1>         swapChain;
	        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
	        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;
            Callback0                                       *swapCallback;
        };

		/// Opens up a display or tells the Device that a new display was opened
		/// \param width width of the display
		/// \param height width of the display
		virtual void                openDisplay				(DTuint width, DTuint height, void *data);

		/// Changes a display or tells the Device that a display was changed
		/// \param width width of the display
		/// \param height width of the display
		virtual void                changeDisplay           (DTuint width, DTuint height);

		/// Closes a display or tells the Device that a display was closed
		virtual void				closeDisplay            (void);	

		/// Swaps the Back and fron buffer
		virtual void				swapDisplay				(void);
				
		/// Returns the width of the screen in pixels
		/// \return width of screen in pixels
		virtual DTuint				getScreenWidth			(void);

		/// Returns the height of the screen in pixels
		/// \return height of screen in pixels
		virtual DTuint				getScreenHeight			(void);

		/// Returns the aspect ratio of the screen in pixels
		/// \return aspect ratio of screen in pixels
		virtual DTfloat				getScreenAspect			(void);

		
		/// Returns the current viewport
		/// \param x x
		/// \param y y
		/// \param width width
		/// \param height height
		virtual void				getViewport				(DTint &x, DTint &y, DTint &width, DTint &height);

		/// Sets the current viewport
		/// \param x x
		/// \param y y
		/// \param width width
		/// \param height height
		/// \param rt_width width of render target. Needed for compatibility with D3D. 0 for screen dimensions
		/// \param rt_height height of render target. Needed for compatibility with D3D. 0 for screen dimensions
		virtual void				setViewport				(DTint x, DTint y, DTint width, DTint height, DTint rt_width = 0, DTint rt_height = 0);

		/// Returns the current viewport width
		/// \return viewport width
		virtual DTuint				getViewportWidth        (void);

		/// Returns the current viewport height
		/// \return viewport height
		virtual DTuint				getViewportHeight		(void);

		/// Returns the current viewport aspect ratio
		/// \return viewport aspect ratio
		virtual DTfloat				getViewportAspect		(void);
		
		
        // Activate a Framebuffer
		/// \param fb framebuffer to activate or NULL
        virtual void                activateFramebuffer     (Framebuffer *fb);

		// Queries

		/// Returns the number of texture units supported by the device
		/// \return number of texture units supported by the device
		virtual DTint				getNumTextureUnits		(void);

		/// Returns the maximum texture size supported by the device
		/// \return maximum texture size supported by the device
		virtual DTint				getMaxTextureSize		(void);

		/// Returns whether the device supports cube maps or not
		/// \return supports cube maps
		virtual DTboolean			hasCubeMaps				(void);
		
		/// Sets the device clear color
		/// \param c clear color
		virtual void				glClearColor				(const Color &c);

		/// Sets the device clear depth
		/// \param d depth
		virtual void				glClearDepth				(DTfloat d);

		/// Sets the device clear stencil
		/// \param d depth
		virtual void				glClearStencil				(DTint s);

		/// Sets the stencil test reference value
		/// \param ref reference value
        virtual void                glStencilRef                (DTint ref)     {   _stencil_ref = ref;     }

		/// Returns the stencil test reference value
        virtual DTint               glGetStencilRef             (void) const    {   return _stencil_ref;    }

		/// Clears the device
		/// \param depth Clear the depth buffer
		/// \param color Clear the color buffer
		virtual void				glClear						(DTboolean depth, DTboolean color, DTboolean stencil);

		/// Sets the shading model for the device
		/// \param s renderer specific shading model (TODO: This should be abstracted)
		virtual void				glShadeModel				(DTuint s);

		/// Sets the polygon offset for the device
		/// \param factor See OpenGL documentation
		/// \param offset See OpenGL documentation
		virtual void				glPolygonOffset				(DTfloat factor, DTfloat units);
		 					
		/// Sets the scissor rect for the renderer (TODO: Need to set mode too)
		/// \param x x
		/// \param y y
		/// \param width width
		/// \param height height
		virtual void				glScissor					(DTint x, DTint y, DTint width, DTint height);
	
						
		/// Activate a camera
		/// \param camera Camera to activate
		virtual void				activateCamera				(   CameraObject *camera	);
    
		
		/// Activate a material
		/// \param material Material to activate
        virtual void                activateMaterial            (   MaterialResource *material );


		/// Draws a GeometryResource
		/// \param transform Transform for the geometry
		/// \param geometry The actual geometry to be drawn
		/// \param material Material to be used
		/// \param skeleton Array of transforms, if any. Can be NULL
		virtual void                drawBatch                   (   const Matrix4 &transform, 
																	GeometryResource *geometry, 
																	MaterialResource *material = NULL,
                                                                    Array<Matrix4> *skeleton = NULL   );
																																						
        /// Draw a ParticlesResource
		/// \param transform Transform for the geometry
		/// \param particles The actual particles to be drawn
		/// \param material Material to be used
		/// \param first First index
		/// \param num Number of particles
		virtual void                drawParticles				(	const Matrix4 &transform,
																	ParticlesResource *particles,
																	MaterialResource *material,
																	DTuint first, DTuint num	);

		/// Draws a Bath of raw geometry in world space
		/// \param v Array of vertices
		/// \param n Array of normals. Can be NULL
		/// \param t1 Array of texture coordinates. Can be NULL
		/// \param t2 Array of texture coordinates. Can be NULL
		/// \param c Array of colors. Can be NULL
		/// \param num Number of elements in each array
		/// \param material Material to be used
		/// \param batch_type Batch type (see above)
		virtual void				drawRawBatch				(	const Matrix4 &transform, 
																	Vector3 *v, 
																	Vector3 *n, 
																	Texcoord2 *t1, 
																	Texcoord2 *t2, 
																	Color *c,
																	DTuint num,
																	MaterialResource *material,
																	BatchType batch_type);

		/// Draws a Bath of raw geometry in screen space
		/// \param v Array of vertices
		/// \param n Array of normals. Can be NULL
		/// \param t1 Array of texture coordinates. Can be NULL
		/// \param t2 Array of texture coordinates. Can be NULL
		/// \param c Array of colors. Can be NULL
		/// \param num Number of elements in each array
		/// \param material Material to be used
		/// \param batch_type Batch type (see above)
		virtual void				drawRawBatchScreenSpace	(       const Matrix4 &transform, 
																	Vector3 *v, 
																	Vector3 *n, 
																	Texcoord2 *t1, 
																	Texcoord2 *t2, 
																	Color *c,
																	DTuint num,
																	MaterialResource *material,
																	BatchType batch_type);
																        
	
		/// Blits an image to the screen
		/// \param x_pos x position of blitted image
		/// \param y_pos y position of blitted image
		/// \param width of blitted image
		/// \param height of blitted image
		/// \param data_width of source image
		/// \param data_height of source image
		/// \param buffer data of source image
        virtual void                blitImage					(DTfloat x_pos, DTfloat y_pos, DTfloat width, DTfloat height, DTuint data_width, DTuint data_height, Color *buffer) {}

		/// Blits an image to the screen
		/// \param x_pos x position of blitted image
		/// \param y_pos y position of blitted image
		/// \param width of blitted image
		/// \param height of blitted image
		/// \param data_width of source image
		/// \param data_height of source image
		/// \param buffer data of source image
        virtual void                blitImageToTexture          (TextureResource *texture, DTfloat x_pos, DTfloat y_pos, DTfloat width, DTfloat height, DTuint data_width, DTuint data_height, Color *buffer);
	
		
		/// Give file manager some time
		/// \param dt time step
		virtual void				tick						(const DTfloat dt);
	
		/// Copies the current screen to a TextureResource
		/// \param tex destination texture
		/// \param rect source rectangle
        virtual void                screenshot                  (TextureResource *tex, const Rectangle &rect) {}
		
        		
		/// Called when an allocation has failed
		virtual void				emergencyFreeMemory         (void);
        
		/// Called when an allocation has failed
		//const Matrix4 &		    getModelview				(void) const	{	return _modelview;	}
		

        ID3D11Device1*              getD3D11Device              (void)  {    return _d3dDevice.Get();   }
        ID3D11DeviceContext1*       getD3D11Context             (void)  {    return _d3dContext.Get();   }

    private:
		friend class DeviceGraphicsDX11Framebuffer;
		friend class DeviceGraphicsDX11Material;
		friend class DeviceGraphicsDX11Shader;
	
		DeviceGraphicsDX11Particles*        getParticlesCached		(ParticlesResource *particles);
		DeviceGraphicsDX11Geometry*         getGeometryCached       (GeometryResource *geometry);
		DeviceGraphicsDX11Texture*		    getTextureCached        (TextureResource *texture);
		DeviceGraphicsDX11Material*         getMaterialCached		(MaterialResource *shader);
		DeviceGraphicsDX11PixelShader*	    getPixelShaderCached	(FragmentShaderResource *shader);
		DeviceGraphicsDX11VertexShader*     getVertexShaderCached	(VertexShaderResource *shader);
		DeviceGraphicsDX11Shader*			getShaderCached			(ShaderResource *shader);
    
		DeviceGraphicsDX11Framebuffer*      getFramebufferCached    (Framebuffer *fb);

		void						        getRenderParameters		(void);
				
		DTuint					        	getTexturesSize			(void);
		DTuint						        getGeometrySize			(void);
				
		// Caches
		Cache<DeviceGraphicsDX11Particles>          _particles_cache;
		Cache<DeviceGraphicsDX11Geometry>           _geometry_cache;
		Cache<DeviceGraphicsDX11Texture>			_texture_cache;
		Cache<DeviceGraphicsDX11Material>           _material_cache;
		Cache<DeviceGraphicsDX11PixelShader>	    _pixel_shader_cache;
		Cache<DeviceGraphicsDX11VertexShader>       _vertex_shader_cache;
		Cache<DeviceGraphicsDX11Shader>			    _shader_cache;
    
        Cache<DeviceGraphicsDX11Framebuffer>        _framebuffer_cache;

		DTuint						_width;
		DTuint						_height;
		DTfloat						_aspect;
		
		DTuint						_viewport_x;
		DTuint						_viewport_y;
		DTuint						_viewport_width;
		DTuint						_viewport_height;
		DTfloat						_viewport_aspect;

		// Gl environment params
		DTint						_num_tex_units;
		DTint						_max_texture_size;
		DTboolean					_has_cube_maps;
		
        DTint                       _stencil_ref;

		// OpenGL specific
		std::vector<String>	_extensions;
		DTboolean					hasExtension			(const String ext);
		
		DTfloat						_tick_accum;
				
		Matrix4						_modelview;
		Matrix4						_projection;
		Matrix4						_camera;
		
		ID3D11Buffer				*_modelview_uniform;
		ID3D11Buffer				*_projection_uniform;
		ID3D11Buffer				*_camera_uniform;
    
        DTint                       _default_framebuffer;
	
        // Working buffers
        void                        reallocBuffers          (DTuint num);

        DTuint                      _num_elements;
        ID3D11Buffer                *_vertices;
        ID3D11Buffer                *_normals;
        ID3D11Buffer                *_texcoords1;
        ID3D11Buffer                *_texcoords2;
        ID3D11Buffer                *_colors;

        // D3D state
        Color                                               _clear_color;
        DTfloat                                             _clear_depth;
        DTubyte                                             _clear_stencil;



        Microsoft::WRL::ComPtr<ID3D11Device1>               _d3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>        _d3dContext;
	    Microsoft::WRL::ComPtr<IDXGISwapChain1>             _swapChain;
	    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      _renderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      _depthStencilView;
        Callback0                                           *_swapCallback;

        ID3D11RenderTargetView                              *_current_render_target_view;
        ID3D11DepthStencilView                              *_current_depth_stencil_view;


};

//==============================================================================
//==============================================================================

} // DT2

#endif
