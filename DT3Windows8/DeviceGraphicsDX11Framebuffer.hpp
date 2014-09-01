#ifndef DT2_DEVICEGRAPHICSDX11FRAMEBUFFER
#define DT2_DEVICEGRAPHICSDX11FRAMEBUFFER
//==============================================================================
///	
///	File: DeviceGraphicsDX11Framebuffer.hpp
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

class Framebuffer;
class DeviceGraphicsDX11Texture;

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11Framebuffer: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11Framebuffer,BaseClass)
		DEFINE_CREATE
         
                                                DeviceGraphicsDX11Framebuffer  (void);	
	private:
                                                DeviceGraphicsDX11Framebuffer  (const DeviceGraphicsDX11Framebuffer &rhs);
        DeviceGraphicsDX11Framebuffer&     operator =                          (const DeviceGraphicsDX11Framebuffer &rhs);	
	public:
        virtual                                 ~DeviceGraphicsDX11Framebuffer (void);


    public:
		/// Description
		/// \param param description
		/// \return description
		void                                    buildFramebuffer                (Framebuffer *fb);

		/// Description
		/// \param param description
		/// \return description
		ID3D11RenderTargetView*                 getRenderTargetView             (void)          {   return _color_view_rb;       }

		/// Description
		/// \param param description
		/// \return description
		ID3D11DepthStencilView*                 getDepthStencilView             (void)          {   return _depth_view_rb;       }

	public:
        DeviceGraphicsDX11Texture*          _color_texture;
        DeviceGraphicsDX11Texture*          _depth_texture;
        

        // Buffers and views for rendering
	    ID3D11RenderTargetView*             _color_view_rb;

	    ID3D11DepthStencilView*             _depth_view_rb;



        ID3D11Texture2D*                    _color_texture_rb;

        ID3D11Texture2D*                    _depth_texture_rb;


};

//==============================================================================
//==============================================================================

} // DT2

#endif
