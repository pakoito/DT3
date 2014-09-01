//==============================================================================
///	
///	File: DeviceGraphicsDX11Shader.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Shader.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "DeviceGraphicsDX11VertexShader.hpp"
#include "DeviceGraphicsDX11PixelShader.hpp"
#include "MoreMath.hpp"
#include "DeviceGlobalsManager.hpp"
#include "System.hpp"
#include "DeviceConsole.hpp"
#include "Factory.hpp"
#include "FilePath.hpp"
#include "ShaderResource.hpp"
#include "TextFileStream.hpp"
#include "DeviceFileManager.hpp"
#include "CheckedCast.hpp"
#include "StringCast.hpp"

#include "FragmentShaderResource.hpp"
#include "VertexShaderResource.hpp"
#include "GeometryShaderResource.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Shader)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Shader::DeviceGraphicsDX11Shader (void)
    :   _layout         (NULL),
		_pixel_shader   (NULL),
		_vertex_shader  (NULL)
{    
    for (DTuint i = 0; i < ARRAY_SIZE(_constants); ++i) {
        _constants[i] = NULL;
    }

}
					
DeviceGraphicsDX11Shader::~DeviceGraphicsDX11Shader (void)
{
	clear();

	RELEASE(_pixel_shader);
	RELEASE(_vertex_shader);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Shader::clear(void)
{
    SAFE_RELEASE(_layout);

    for (DTuint i = 0; i < ARRAY_SIZE(_constants); ++i) {
        SAFE_RELEASE(_constants[i]);
    }

    for (DTuint i = 0; i < _parameters.size(); ++i) {
        SAFE_RELEASE(_parameters[i]);
    }

    _parameters.clear();
}

//==============================================================================
//==============================================================================

#define ADD_DESC(A,F)                                                       \
    if (program->hasAttrib(A)) {                                            \
        String attrib_name = program->getAttribName(A);                     \
        String index = attrib_name.endDigits();                             \
        name_buffer[desc_index] = attrib_name.trimEndDigits();              \
        desc[desc_index].SemanticName = name_buffer[desc_index].cStr();     \
        desc[desc_index].SemanticIndex = index.size() ? castFromString<DTint>(index) : 0;      \
        desc[desc_index].Format = F;                                        \
        desc[desc_index].InputSlot = A;                                     \
        desc[desc_index].AlignedByteOffset = 0;                             \
        desc[desc_index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;      \
        desc[desc_index].InstanceDataStepRate = 0;                          \
        ++desc_index;                                                       \
    }

#define ADD_CONSTANT(A,S)                                                   \
    if (!program->getUniformName(A).empty()) {                              \

        String constant_name = program->getUniformName(A);                  \
        DTint ps_resource_index = program->getFragmentShader() ? program->getFragmentShader()->getShaderResourceIndex (constant_name) : -1;    \
        DTint vs_resource_index = program->getVertexShader() ? program->getVertexShader()->getShaderResourceIndex (constant_name) : -1;        \
                                                                            \

        D3D11_BUFFER_DESC desc;                                             \

        desc.ByteWidth = S;                                                 \

        desc.MiscFlags = 0;                                                 \

        desc.StructureByteStride = 0;                                       \

        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;                        \

        desc.Usage = D3D11_USAGE_DYNAMIC;                                   \

        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                       \

                                                                            \

        HRESULT hr = device->CreateBuffer(&desc, NULL, &_constants[A]);     \

        Assert(SUCCEEDED(hr));                                              \
                                                                            \
        if (vs_resource_index >= 0)   _vs_constants[vs_resource_index] = _constants[A];     \
        if (ps_resource_index >= 0)   _ps_constants[ps_resource_index] = _constants[A];     \
    }                                                                       


void DeviceGraphicsDX11Shader::syncToResource (ShaderResource *program)
{
	if (program->getRecacheData()) {
	
        DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

        ID3D11Device1 *device = renderer->getD3D11Device();

        ID3D11DeviceContext1 *context = renderer->getD3D11Context();


		SAFE_ASSIGN(_pixel_shader, checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getPixelShaderCached(program->getFragmentShader()));
		SAFE_ASSIGN(_vertex_shader, checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getVertexShaderCached(program->getVertexShader()));

		if (!_pixel_shader || !_vertex_shader) { // Geometry shader is optional
			program->setRecacheData(false);
			return;
		}

        // Clear previous resources
        clear();

        HRESULT hr;

        //
        // Build a layout (Input element description)
        //
		
        D3D11_INPUT_ELEMENT_DESC desc[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
        String name_buffer[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
        ::memset(desc,0,sizeof(desc));

        DTuint desc_index = 0;

        ADD_DESC(ShaderResource::ATTRIB_POSITION, DXGI_FORMAT_R32G32B32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);
        ADD_DESC(ShaderResource::ATTRIB_NORMAL, DXGI_FORMAT_R32G32B32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_TEXCOORD0, DXGI_FORMAT_R32G32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_TEXCOORD1, DXGI_FORMAT_R32G32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_TEXCOORD2, DXGI_FORMAT_R32G32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_TEXCOORD3, DXGI_FORMAT_R32G32_FLOAT);
        ADD_DESC(ShaderResource::ATTRIB_WEIGHTS_INDEX, DXGI_FORMAT_R16G16B16A16_UINT);
        ADD_DESC(ShaderResource::ATTRIB_WEIGHTS_STRENGTH, DXGI_FORMAT_R32G32B32A32_FLOAT);

        const String *bytecode = program->getVertexShader()->getShader("HLSL");

        hr = device->CreateInputLayout(desc,desc_index,&(*bytecode)[0],bytecode->size(),&_layout);
        Assert(SUCCEEDED(hr));


        //
        // Constants (Uniforms)
        //

        ::memset(_constants,0,sizeof(_constants));
        ::memset(_vs_constants,0,sizeof(_vs_constants));
        ::memset(_ps_constants,0,sizeof(_ps_constants));

        ADD_CONSTANT(ShaderResource::UNIFORM_COLOR, sizeof(float) * 4);
        ADD_CONSTANT(ShaderResource::UNIFORM_TEXTURE0, sizeof(float) * 16);
        ADD_CONSTANT(ShaderResource::UNIFORM_TEXTURE1, sizeof(float) * 16);
        ADD_CONSTANT(ShaderResource::UNIFORM_MODELVIEW, sizeof(float) * 16);
        ADD_CONSTANT(ShaderResource::UNIFORM_PROJECTION, sizeof(float) * 16);
        ADD_CONSTANT(ShaderResource::UNIFORM_CAMERA, sizeof(float) * 16);
        //ADD_CONSTANT(ShaderResource::UNIFORM_SKELETON,???);

		const Array<ShaderResource::Parameter> &parameters = program->getParameters();
		_parameters.resize(parameters.size());
		
		for (DTuint i = 0; i < parameters.size(); ++i) {
            String constant_name = parameters[i].name;
            DTint ps_resource_index = program->getFragmentShader() ? program->getFragmentShader()->getShaderResourceIndex (constant_name) : -1;
            DTint vs_resource_index = program->getVertexShader() ? program->getVertexShader()->getShaderResourceIndex (constant_name) : -1; 

            D3D11_BUFFER_DESC desc;

            desc.ByteWidth = parameters[i].data.size();

            desc.MiscFlags = 0;

            desc.StructureByteStride = 0;

            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

            desc.Usage = D3D11_USAGE_DYNAMIC;

            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;



            HRESULT hr = device->CreateBuffer(&desc, NULL, &_parameters[i]);

            Assert(SUCCEEDED(hr));

            if (vs_resource_index >= 0)   _vs_constants[vs_resource_index] = _parameters[i];
            if (ps_resource_index >= 0)   _ps_constants[ps_resource_index] = _parameters[i];
		}
		
		program->setRecacheData(false);
    }
	
	// recache all of our parameters
	if (program->getRecacheParameters()) {
    
        DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

        ID3D11DeviceContext1 *context = renderer->getD3D11Context();


        // Get Parameters. This creates a mapping from our internal index to an opengl uniform location
        const Array<ShaderResource::Parameter> &parameters = program->getParameters();
            
        for (DTuint i = 0; i < parameters.size(); ++i) {
            const ShaderResource::Parameter &p = parameters[i]; 
            context->UpdateSubresource(_parameters[i], 0, NULL, &parameters[0].data[0], parameters[i].data.size(), parameters[i].data.size());            
        }
        
		program->setRecacheParameters(false);
	}
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Shader::setConstantValue (ID3D11Buffer *buffer, DTubyte *data, DTuint size)
{    	
    if (!buffer)
        return;

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11DeviceContext1 *context = renderer->getD3D11Context();



    D3D11_MAPPED_SUBRESOURCE res;
    HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res); 

    ::memcpy(res.pData,data,size);

    context->Unmap(buffer, 0);
}

//==============================================================================
//==============================================================================

} // DT2
