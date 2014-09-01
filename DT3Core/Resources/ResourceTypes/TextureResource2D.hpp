#ifndef DT3_TEXTURERESOURCE2D
#define DT3_TEXTURERESOURCE2D
//==============================================================================
///	
///	File: TextureResource2D.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include <vector>
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================



//==============================================================================
/// Texture resource type
//==============================================================================

class TextureResource2D: public Resource {
    public:
        DEFINE_TYPE(TextureResource2D,Resource)
		DEFINE_CREATE

									TextureResource2D       (void);
	private:
									TextureResource2D       (const TextureResource2D &rhs);
        TextureResource2D &			operator =              (const TextureResource2D &rhs);
	public:
        virtual						~TextureResource2D      (void);
                
    public:        	
		/// Called to initialize the class
		static void                 initialize_static       (void);

		/// Called to uninitialize the class
		static void                 uninitialize_static     (void);

		/// Called to initialize the object
		virtual void				initialize              (void);


		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr						import                  (const FilePath &pathname, std::string args);
		
		// Texture building functions
		
		/// Convenience funtion to allocate a blank 2D RGBA8 texture for building from scratch
		/// \param width width of texture
		/// \param height height of texture
		/// \param mipmapped mipmapped
        void                        allocate_rgba_textels   (const DTint width, const DTint height, DTboolean mipmapped);

		/// Allocate a 2D texture
		/// \param width width of texture
		/// \param height height of texture
		/// \param textels textel data (can be NULL)
		/// \param format format
		/// \param mipmapped mipmapped
        void                        set_textels             (const DTint width, const DTint height, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);

		/// Allocate a compressed 2D texture
		/// \param width width of texture
		/// \param height height of texture
		/// \param textels textel data
		/// \param format format
		/// \param mipmapped mipmapped
        void                        set_textels_compressed  (const DTint width, const DTint height, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);

		/// Width of the buffer
		/// \return buffer width
		DTint						width                   (void) const	{	return _width;	}

		/// Height of the buffer
		/// \return buffer height
		DTint						height                  (void) const	{	return _height;	}

		/// Returns the buffer (if available)
		/// \return buffer
		DTubyte*                    buffer                  (void)          {	_dirty_rectangle.set(0, _width, 0, _height);    return _textels.get();  }
				
    
		/// Returns format of the texture file
		/// \return format of texture file
		DTint						format                  (void) const    {	return _format;     }

		/// Get the mipmapped flag
		/// \return mipmapped
		DTboolean					is_mipmapped            (void) const    {	return _mipmapped;  }

    
		/// Gets a pixel from the buffer
		/// \param x x coord
		/// \param y y coord
		/// \return color
        Color4b                     pixel                   (DTint x, DTint y);

		/// Gets a pixel address from the buffer
		/// \param x x coord
		/// \param y y coord
		/// \return address
        DTubyte*                    pixel_addr              (DTint x, DTint y);

		/// Sets a pixel in the buffer
		/// \param x x coord
		/// \param y y coord
		/// \param pixel color
        void                        set_pixel               (DTint x, DTint y, const Color4b &pixel);

		/// Updates texture data
		/// \param dst_x x position to copy the data to
		/// \param dst_y y position to copy the data to
		/// \param buffer data
		/// \param src_width width of data
		/// \param src_height height of data
		/// \param rowbytes bytes per row
        void                        replace_sub_rectangle   (DTint dst_x, DTint dst_y, const DTubyte *buffer, DTint src_width, DTint src_height, DTint rowbytes = 0);

		/// Activate the resource
		/// \param texture_slot which slot to activate the texture in
        void                        activate                (DTuint texture_slot);

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<TextureResource2D>    import_resource (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                                   _texture_map_lock;
		static std::map<std::string, std::shared_ptr<TextureResource2D>>    _texture_map;
    
        static void                 reload_if_changed       (void);

        void                        screen_opened           (DTuint width, DTuint height);
        void                        screen_closed           (void);

        // Data for 2D and 3D textures
        std::shared_ptr<DTubyte>                _textels;
    
		DT3GLTextelFormat                        _format;
    
		DTint                                   _width;
		DTint                                   _height;
		DTint                                   _depth;
    
		DTboolean                               _mipmapped;
        DTuint                                  _flags;
    
        std::shared_ptr<DT3GLTexture2DResource> _resource_2D;

        Rectangle                               _dirty_rectangle;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<TextureResource2D> &r);
Stream& operator >>(Stream &s, std::shared_ptr<TextureResource2D> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
