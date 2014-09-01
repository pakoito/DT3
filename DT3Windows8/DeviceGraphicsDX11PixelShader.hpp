#ifndef DT2_DEVICEGRAPHICSDX11PIXELSHADER
#define DT2_DEVICEGRAPHICSDX11PIXELSHADER
//==============================================================================
///	
///	File: DeviceGraphicsDX11PixelShader.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "Config.hpp"
#include "Memory.hpp"
#include <sstream>
#include "BaseClass.hpp"
#include "String.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FragmentShaderResource;
class FilePath;

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11PixelShader: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11PixelShader,BaseClass)
		DEFINE_CREATE
         
										DeviceGraphicsDX11PixelShader	(void);	
    
    private:	
										    DeviceGraphicsDX11PixelShader	(const DeviceGraphicsDX11PixelShader &rhs);
        DeviceGraphicsDX11PixelShader&	operator =					        (const DeviceGraphicsDX11PixelShader &rhs);	
        
    public:
        virtual							    ~DeviceGraphicsDX11PixelShader	(void);
             
    public:
		/// Description
		/// \param param description
		/// \return description
        void						    	syncToResource			    (FragmentShaderResource *program);
		
		/// Description
		/// \param param description
		/// \return description
		ID3D11PixelShader*				    getShader					(void)	{	return _shader;	}
	
    private:
  		void							    loadShader					(const FilePath &path, const String *shader);

        ID3D11PixelShader                  *_shader;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
