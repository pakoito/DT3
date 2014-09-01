//==============================================================================
///	
///	File:        	HLSLShaderCompiler.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================



#include <Windows.h>

#include <D3Dcompiler.h>

#include <D3D11Shader.h>



#include <fstream>

#include <iostream>

#include <string>



//==============================================================================

//==============================================================================



std::string toHexString (const void* data, unsigned int length)
{
	std::string as_hex;
	as_hex.reserve(length * 2);
	
	const unsigned char *data_bytes = reinterpret_cast<const unsigned char*>(data);
	
	for (unsigned int i = 0; i < length; ++i) {
		
		unsigned char nibbles[2] = {	(data_bytes[i] >> 4) & 0xF, 
								        (data_bytes[i]) & 0xF };
		
		if (nibbles[0] <= 9)	as_hex += (char) ('0' + nibbles[0]);
		else					as_hex += (char) ('A' + nibbles[0] - 10);

		if (nibbles[1] <= 9)	as_hex += (char) ('0' + nibbles[1]);
		else					as_hex += (char) ('A' + nibbles[1] - 10);

	}
	
	return as_hex;
}



//==============================================================================

//==============================================================================



int main(int argc, char* argv[])

{

    if (argc != 5) {

        std::cout << "Usage: " << argv[0] << " entryPoint target outfile infile" << std::endl;

        std::cout << " Target can be something like ps_4_0, vs_4_0" << std::endl;

        return 1;

    }



    //

    // Read the entire shader source file

    //



    std::cout << "Reading File..." << std::endl;



    // Open the file

    std::ifstream infile(argv[4], std::ios::in | std::ios::binary);

    if (!infile) {

        std::cout << "Unable to open input file " << argv[4] << std::endl;

        return 1;

    }



    // Read the actual data

    std::string shader_source; 

    infile.seekg(0, std::ios::end);

    unsigned int length = infile.tellg();

    shader_source.resize(length);

    infile.seekg(0, std::ios::beg);

    infile.read(&shader_source[0], shader_source.size());

    infile.close();



    //

    // Compile the source code

    //



    std::cout << "Compiling..." << std::endl;



    ID3DBlob *code;

    ID3DBlob *errors;



    HRESULT hr = D3DCompile(

      &shader_source[0],                    //in       LPCVOID pSrcData,

      shader_source.size(),                 //in       SIZE_T SrcDataSize,

      argv[4],                              //in_opt   LPCSTR pSourceName,

      NULL,                                 //in_opt   const D3D_SHADER_MACRO *pDefines,

      D3D_COMPILE_STANDARD_FILE_INCLUDE,    //in_opt   ID3DInclude *pInclude,

      argv[1],                              //in       LPCSTR pEntrypoint,

      argv[2],                              //in       LPCSTR pTarget,

      D3DCOMPILE_OPTIMIZATION_LEVEL3 | 

      D3DCOMPILE_WARNINGS_ARE_ERRORS,       //in       UINT Flags1,

      0,                                    //in       UINT Flags2,

      &code,                                //out      ID3DBlob **ppCode,

      &errors                               //out_opt  ID3DBlob **ppErrorMsgs

    );



    // Check for compilation errors

    if (FAILED(hr)) {

        if (errors) {

            std::cout << "D3DCompile failed!" << std::endl;

            std::cout << (char*) errors->GetBufferPointer() << std::endl;

        } else {

            std::cout << "D3DCompile failed!" << std::endl;

        }

       

        return 1;

    }



    //

    // Open the output file

    //



    std::ofstream outfile(argv[3]);

    if (!outfile) {

        std::cout << "Unable to open output file " << argv[3] << std::endl;

        return 1;

    }



    const char *INDENT = "    ";



    outfile << "shader {" << std::endl;



    //

    // Shader Reflection information

    //



    std::cout << "Analyzing..." << std::endl;



    ID3D11ShaderReflection *reflection = NULL;

    hr = D3DReflect(code->GetBufferPointer(), code->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &reflection);

    if (SUCCEEDED(hr)) {

        D3D11_SHADER_DESC desc;

        reflection->GetDesc(&desc);



        std::cout << std::endl;

        std::cout << "Required Resources" << std::endl;

        std::cout << "------------------" << std::endl;



        outfile << INDENT << "resources {" << std::endl;



        for (int i = 0; i < desc.BoundResources; ++i) {

            D3D11_SHADER_INPUT_BIND_DESC rDesc;

        

            reflection->GetResourceBindingDesc(i,&rDesc);



            std::cout << i << " " << rDesc.Name << " " << rDesc.BindPoint << std::endl;



            outfile << INDENT << INDENT << "resource = \"" << rDesc.Name << "\" " << rDesc.BindPoint << std::endl;

        }



        outfile << INDENT << "}" << std::endl;

    } else {

        std::string target(argv[2]);



        if (    target.find("_3_") != std::string::npos ||

                target.find("_2_") != std::string::npos ||

                target.find("_1_") != std::string::npos) {



            // Build our own resource table





        }





    }



    // Output compiled shader bytecode

    std::string shader_compiled = toHexString (code->GetBufferPointer(), code->GetBufferSize());
    outfile << INDENT << "shader = \"" << shader_compiled << "\"" << std::endl;



    // Finish off the file

    outfile << "}" << std::endl;



	return 0;

}



