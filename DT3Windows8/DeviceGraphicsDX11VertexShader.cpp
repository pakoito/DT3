//==============================================================================
///	
///	File: DeviceGraphicsDX11VertexShader.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11VertexShader.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "MoreMath.hpp"
#include "DeviceGlobalsManager.hpp"
#include "System.hpp"
#include "DeviceConsole.hpp"
#include "Factory.hpp"
#include "FilePath.hpp"
#include "VertexShaderResource.hpp"
#include "TextFileStream.hpp"
#include "DeviceFileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11VertexShader)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11VertexShader::DeviceGraphicsDX11VertexShader (void)
    :   _shader (NULL)
{    

}
					
DeviceGraphicsDX11VertexShader::~DeviceGraphicsDX11VertexShader (void)
{
    SAFE_RELEASE(_shader);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11VertexShader::loadShader (const FilePath &path, const String *shader)
{
    Assert(shader);

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11Device1 *device = renderer->getD3D11Device();


    SAFE_RELEASE(_shader);
    HRESULT hr = device->CreateVertexShader(&(*shader)[0], shader->size(), NULL, &_shader);
    Assert(SUCCEEDED(hr));
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11VertexShader::syncToResource		(VertexShaderResource *program)
{
	if (program->getRecacheData()) {
		loadShader(program->getPath(), program->getShader("HLSL"));
		program->setRecacheData(false);
    }
	
	program->setRecacheParameters(false);
}

//==============================================================================
//==============================================================================

} // DT2

