#ifndef DT3_DRAWUTILS
#define DT3_DRAWUTILS
//==============================================================================
///	
///	File: DrawUtils.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Color4f;
class Color4b;
class Matrix4;
class Vector2;
class DrawBatcher;

class CameraObject;
class MaterialResource;
class ShaderResource;

//==============================================================================
/// Class
//==============================================================================

class DrawUtils {
 	private:
										DrawUtils					(void);	
										DrawUtils					(const DrawUtils &rhs);
        DrawUtils &						operator =					(const DrawUtils &rhs);
		virtual							~DrawUtils					(void);

	public:
		// Helper Drawing routines
				
		/// Draw a stretched quad split into 9 parts
		/// \param draw_batcher Draw batcher to use
		/// \param material Material to use
		/// \param color Color to use
		/// \param transform Transform to use
		/// \param xpos xpos of quad
		/// \param ypos ypos of quad
		/// \param xsize xsize of quad
		/// \param ysize ysize of quad
		/// \param corner_width width of the corner
		/// \param corner_height height of the corner
		static void						draw_quad_stretch_center_3x3(   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Color4f &color,
                                                                        const Matrix4 &transform,
                                                                        DTfloat xpos,
                                                                        DTfloat ypos,
                                                                        DTfloat xsize,
                                                                        DTfloat ysize,
                                                                        DTfloat corner_width,
                                                                        DTfloat corner_height);

		/// Draw a stretched quad split into 4 parts
		/// \param draw_batcher Draw batcher to use
		/// \param material Material to use
		/// \param color Color to use
		/// \param transform Transform to use
		/// \param xpos xpos of quad
		/// \param ypos ypos of quad
		/// \param xsize xsize of quad
		/// \param ysize ysize of quad
		/// \param corner_width width of the corner
		/// \param corner_height height of the corner
		static void						draw_quad_stretch_center_2x2(   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Color4f &color,
                                                                        const Matrix4 &transform,
                                                                        DTfloat xpos,
                                                                        DTfloat ypos,
                                                                        DTfloat xsize,
                                                                        DTfloat ysize,
                                                                        DTfloat corner_width,
                                                                        DTfloat corner_height);
    
		/// Draw a selection box
		/// \param draw_batcher Draw batcher to use
		/// \param camera Camera to use
		/// \param material Material to use
		/// \param shader Shader to use
		/// \param transform Transform to use
		/// \param color Color for drawing
		/// \param radius Radius of selection box
		static void						draw_selection              (   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Matrix4 &transform,
                                                                        const Color4b &color,
                                                                        DTfloat radius);

		/// Draw a box
		/// \param draw_batcher Draw batcher to use
		/// \param camera Camera to use
		/// \param material Material to use
		/// \param shader Shader to use
		/// \param transform Transform to use
		/// \param color Color for drawing
		/// \param size Size of selection box
        static void                     draw_cube                   (   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Matrix4 &transform,
                                                                        const Color4b &color,
                                                                        DTfloat size);
    
		/// Draw a cone
		/// \param draw_batcher Draw batcher to use
		/// \param camera Camera to use
		/// \param material Material to use
		/// \param shader Shader to use
		/// \param transform Transform to use
		/// \param color Color for drawing
		/// \param radius Radius of cone
		/// \param length Length of cone
        static void                     draw_cone                   (   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Matrix4 &transform,
                                                                        const Color4b &color,
                                                                        DTfloat radius,
                                                                        DTfloat length);

		/// Draw a ring
		/// \param draw_batcher Draw batcher to use
		/// \param camera Camera to use
		/// \param material Material to use
		/// \param shader Shader to use
		/// \param transform Transform to use
		/// \param color Color for drawing
		/// \param size Size of selection box
        static void                     draw_ring                   (   DrawBatcher &draw_batcher,
                                                                        const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Matrix4 &transform,
                                                                        const Color4b &color,
                                                                        DTfloat size);

		/// Draw a message with a bit font
		/// \param material Material to use
		/// \param transform Transform to use
		/// \param text Text to draw
        static void                     draw_bit_font               (   const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Vector2 &translation,
                                                                        const std::string &text);
    
        /// Activate a render state fo all of the builtin drawing
        /// \param camera current camera
        /// \param material material to activate
        /// \param shader shader to use for drawing
        static void                     activate_state              (   const std::shared_ptr<CameraObject> &camera,
                                                                        const std::shared_ptr<MaterialResource> &material,
                                                                        const std::shared_ptr<ShaderResource> &shader,
                                                                        const Matrix4 &transform);
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif
