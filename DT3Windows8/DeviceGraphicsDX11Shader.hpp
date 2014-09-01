#ifndef DT2_DEVICEGRAPHICSDX11SHADER
#define DT2_DEVICEGRAPHICSDX11SHADER
//==============================================================================
///	
///	File: DeviceGraphicsDX11Shader.hpp
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
#include "FilePath.hpp"
#include <sstream>
#include "BaseClass.hpp"
#include "Array.hpp"
#include "ShaderResource.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class DeviceGraphicsDX11PixelShader;
class DeviceGraphicsDX11VertexShader;

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11Shader: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11Shader,BaseClass)
		DEFINE_CREATE
         
										    DeviceGraphicsDX11Shader			(void);	
    
    private:	
										    DeviceGraphicsDX11Shader			(const DeviceGraphicsDX11Shader &rhs);
        DeviceGraphicsDX11Shader &			operator =				(const DeviceGraphicsDX11Shader &rhs);	
    
    public:
        virtual						    	~DeviceGraphicsDX11Shader		(void);
        
    public:
		/// Description
		/// \param param description
		/// \return description
		void                                syncToResource	    (ShaderResource *program);

		/// Description
		/// \param param description
		/// \return description
        void							    import			    (const FilePath &pathname);


		/// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11PixelShader*      getPixelShader      (void)	    {	return _pixel_shader;	}

        /// Description
		/// \param param description
		/// \return description
		DeviceGraphicsDX11VertexShader*     getVertexShader     (void)	    {	return _vertex_shader;  }


        /// Description
		/// \param param description
		/// \return description
        ID3D11InputLayout*                  getLayout           (void)      {   return _layout;         }

        /// Description
		/// \param param description
		/// \return description
        ID3D11Buffer*                       getConstant         (DTuint i)  {   return _constants[i];   }

        /// Description
		/// \param param description
		/// \return description
        ID3D11Buffer**                      getVSConstants      (void)      {   return _vs_constants;     }
        ID3D11Buffer**                      getPSConstants      (void)      {   return _ps_constants;     }


        /// Description
		/// \param param description
		/// \return description
        static void                         setConstantValue    (ID3D11Buffer *buffer, DTubyte *data, DTuint size);

    private:
        friend class DeviceGraphicsDX11Material;

        void                                clear               (void);

        ID3D11Buffer                        *_constants[ShaderResource::NUM_UNIFORMS];
		Array<ID3D11Buffer*>			    _parameters;

        ID3D11InputLayout                   *_layout;

        ID3D11Buffer                        *_vs_constants[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        ID3D11Buffer                        *_ps_constants[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		
		DeviceGraphicsDX11PixelShader	    *_pixel_shader;
		DeviceGraphicsDX11VertexShader		*_vertex_shader;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
