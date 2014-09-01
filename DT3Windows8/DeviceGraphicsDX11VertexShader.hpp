#ifndef DT2_DEVICEGRAPHICSDX11VERTEXSHADER
#define DT2_DEVICEGRAPHICSDX11VERTEXSHADER
//==============================================================================
///	
///	File: DeviceGraphicsDX11VertexShader.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "WinUtils.hpp"
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

class VertexShaderResource;
class FilePath;

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11VertexShader: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11VertexShader,BaseClass)
		DEFINE_CREATE
         
										DeviceGraphicsDX11VertexShader		(void);	
    
    private:	
										DeviceGraphicsDX11VertexShader		(const DeviceGraphicsDX11VertexShader &rhs);
        DeviceGraphicsDX11VertexShader&	operator =				        	(const DeviceGraphicsDX11VertexShader &rhs);	
    
    public:
        virtual							~DeviceGraphicsDX11VertexShader		(void);
        
    public:
		/// Description
		/// \param param description
		/// \return description
		void                            syncToResource			    (VertexShaderResource *program);

		/// Description
		/// \param param description
		/// \return description
		ID3D11VertexShader*             getShader					(void)	{	return _shader;	}

    private:
  		void						    loadShader					(const FilePath &path, const String *shader);

        ID3D11VertexShader              *_shader;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
