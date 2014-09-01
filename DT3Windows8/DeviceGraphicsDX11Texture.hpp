#ifndef DT2_DEVICEGRAPHICSDX11TEXTURE
#define DT2_DEVICEGRAPHICSDX11TEXTURE
//==============================================================================
///	
///	File: DeviceGraphicsDX11Texture.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "WinUtils.hpp"
#include "BaseClass.hpp"
#include "Vector.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Color;
class TextureResource;

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11Texture: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11Texture,BaseClass)
		DEFINE_CREATE
         
                                    DeviceGraphicsDX11Texture		(void);	
	private:
                                    DeviceGraphicsDX11Texture		(const DeviceGraphicsDX11Texture &rhs);
        DeviceGraphicsDX11Texture &			operator =				(const DeviceGraphicsDX11Texture &rhs);	
	public:
        virtual                     ~DeviceGraphicsDX11Texture		(void);

	public:

		/// Description
		/// \param param description
		/// \return description
		void                        buildTexture2D			(DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size = 0, DTboolean mipmapped = false);

		/// Description
		/// \param param description
		/// \return description
		void                        updateTexture2D			(DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size = 0, DTboolean mipmapped = false);

		/// Description
		/// \param param description
		/// \return description
		void                        buildTexture2D			(DTuint width, DTuint height, DTint format, DTuint compressed_size = 0, DTboolean mipmapped = false);

		/// Description
		/// \param param description
		/// \return description
		void                        buildTexture3D          (DTubyte *buffer, DTuint width, DTuint height, DTuint depth, DTint format, DTuint compressed_size = 0, DTboolean mipmapped = false);

		/// Description
		/// \param param description
		/// \return description
		void                        buildTextureCube		(DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size = 0, DTboolean mipmapped = false);

		/// Description
		/// \param param description
		/// \return description
		void						syncToResource          (TextureResource *texture);
		
		/// Description
		/// \param param description
		/// \return description
		ID3D11Texture2D*            get2DTexture            (void)          {   return _texture_2D;     }

        /// Description
		/// \param param description
		/// \return description
		ID3D11Texture3D*            get3DTexture            (void)          {   return _texture_3D;     }

        /// Description
		/// \param param description
		/// \return description
        ID3D11ShaderResourceView*   getTextureView          (void)          {   return _texture_view;   }

		/// Description
		/// \param param description
		/// \return description
		DTuint						size					(void) const    {	return _size;	        }

		/// Description
		/// \param param description
		/// \return description
		DTboolean					isResident				(void) const    {	return _resident;   	}


		/// Description
		/// \param param description
		/// \return description
		DTboolean					is2D				(void)          {	return (_type == TYPE_2D);	}

		/// Description
		/// \param param description
		/// \return description
		DTboolean					is3D				(void)          {	return (_type == TYPE_3D);	}

		/// Description
		/// \param param description
		/// \return description
		DTboolean					isCube				(void)          {	return (_type == TYPE_CUBE);}

	private:

        enum {
            TYPE_2D,
            TYPE_3D,
            TYPE_CUBE
        } _type;

		ID3D11Texture2D 			*_texture_2D;
		ID3D11Texture3D 			*_texture_3D;

        ID3D11ShaderResourceView    *_texture_view;

        DTuint                      _width;
        DTuint                      _height;
        DTuint                      _depth;
        
        DTboolean                   _is_created;
		DTuint						_size;
		DTboolean					_resident;
};

//==============================================================================
//==============================================================================

} // DT2

#endif

