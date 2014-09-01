#ifndef DT3_TEXTURERESOURCE3D
#define DT3_TEXTURERESOURCE3D
//==============================================================================
///	
///	File: TextureResource3D.hpp
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

class TextureResource3D: public Resource {
    public:
        DEFINE_TYPE(TextureResource3D,Resource)
		DEFINE_CREATE

									TextureResource3D   (void);
	private:
									TextureResource3D   (const TextureResource3D &rhs);
        TextureResource3D &			operator =			(const TextureResource3D &rhs);	
	public:
        virtual						~TextureResource3D	(void);
                
    public:        	
		/// Called to initialize the class
		static void                 initialize_static   (void);

		/// Called to uninitialize the class
		static void                 uninitialize_static (void);

		/// Called to initialize the object
		virtual void				initialize          (void);


		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr						import				(const FilePath &pathname, std::string args);
		
		// Texture building functions
		
		/// Allocate a 3D texture
		/// \param width width of texture
		/// \param height height of texture
		/// \param depth depth of texture
		/// \param textels textel data (can be NULL)
		/// \param format format
		/// \param mipmapped mipmapped
        void                        set_textels         (const DTint width, const DTint height, const DTint depth, std::shared_ptr<DTubyte> &textels, DT3GLTextelFormat format, DTboolean mipmapped, DTuint flags = DT3GL_ACCESS_CPU_NONE | DT3GL_ACCESS_GPU_READ);

		/// Width of the buffer
		/// \return buffer width
		DTint						width               (void) const	{	return _width;	}

		/// Height of the buffer
		/// \return buffer height
		DTint						height              (void) const	{	return _height;	}

		/// Depth of the buffer
		/// \return buffer depth
		DTint						depth               (void) const	{	return _depth;	}

		/// Returns format of the texture file
		/// \return format of texture file
		DTint						format              (void) const    {	return _format;     }

		/// Get the mipmapped flag
		/// \return mipmapped
		DTboolean					is_mipmapped        (void) const    {	return _mipmapped;  }

		/// Activate the resource
		/// \param texture_slot which slot to activate the texture in
        void                        activate            (DTuint texture_slot);

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<TextureResource3D>    import_resource (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                                   _texture_map_lock;
		static std::map<std::string, std::shared_ptr<TextureResource3D>>    _texture_map;
    
        static void                 reload_if_changed   (void);

        void                        screen_opened       (DTuint width, DTuint height);
        void                        screen_closed       (void);

        // Data for 2D and 3D textures
        std::shared_ptr<DTubyte>                _textels;
    
        DT3GLTextelFormat                       _format;
    
		DTint                                   _width;
		DTint                                   _height;
		DTint                                   _depth;
    
		DTboolean                               _mipmapped;
        DTuint                                  _flags;
    
        std::shared_ptr<DT3GLTexture3DResource> _resource_3D;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<TextureResource3D> &r);
Stream& operator >>(Stream &s, std::shared_ptr<TextureResource3D> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
