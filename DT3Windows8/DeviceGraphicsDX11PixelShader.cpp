//==============================================================================
///	
///	File: DeviceGraphicsDX11PixelShader.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11PixelShader.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "MoreMath.hpp"
#include "DeviceGlobalsManager.hpp"
#include "System.hpp"
#include "DeviceConsole.hpp"
#include "Factory.hpp"
#include "FilePath.hpp"
#include "FragmentShaderResource.hpp"
#include "Assert.hpp"
#include "TextFileStream.hpp"
#include "DeviceFileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11PixelShader)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11PixelShader::DeviceGraphicsDX11PixelShader (void)
    :   _shader (NULL)
{    

}
			
DeviceGraphicsDX11PixelShader::~DeviceGraphicsDX11PixelShader (void)
{
    SAFE_RELEASE(_shader);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11PixelShader::loadShader (const FilePath &path, const String *shader)
{
    Assert(shader);

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11Device1 *device = renderer->getD3D11Device();


    SAFE_RELEASE(_shader);
    HRESULT hr = device->CreatePixelShader(&(*shader)[0], shader->size(), NULL, &_shader);
    Assert(SUCCEEDED(hr));
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11PixelShader::syncToResource (FragmentShaderResource *program)
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

