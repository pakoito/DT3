#ifndef DT3_SHADERRESOURCE
#define DT3_SHADERRESOURCE
//==============================================================================
///	
///	File: ShaderResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include <string>
#include <vector>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Shader resource type
//==============================================================================

class ShaderResource: public Resource {
    public:
        DEFINE_TYPE(ShaderResource,Resource)
		DEFINE_CREATE
         
                                                ShaderResource          (void);
	private:
                                                ShaderResource          (const ShaderResource &rhs);
        ShaderResource &                        operator =              (const ShaderResource &rhs);
	public:
        virtual                                 ~ShaderResource         (void);

    public:
		/// Called to initialize the class
		static void                             initialize_static       (void);

		/// Called to uninitialize the class
		static void                             uninitialize_static     (void);

		/// Called to initialize the object
		virtual void                            initialize              (void);


		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr                                   import					(const FilePath &pathname, std::string args);
       

		/// Sets the fragment shader
		/// \param p shader
        void                                    add_fragment_shader     (const std::string &standard, const std::string &program);
    
		/// Sets the fragment shader
		/// \param p shader
        void                                    add_geometry_shader     (const std::string &standard, const std::string &program);

		/// Sets the vertex shader
		/// \param p shader
        void                                    add_vertex_shader       (const std::string &standard, const std::string &program);


		//
		// Mappings for standard, engine recognized attributes
		//
		
		/// Sets the name of an attribute
		/// \param name attribute name
		void                                    add_attrib              (const std::string &name, DT3GLStandardAttrib standard_attrib = DT3GL_ATTRIB_NONE);

		/// Get attribute slot
		/// \param name attribute name
		/// \return slot
        DTint                                   attrib_slot             (const std::string &name);

		/// Get attribute slot
		/// \param standard_attrib attribute id
		/// \return slot
        DTint                                   attrib_slot             (DT3GLStandardAttrib standard_attrib);
    
        /// Convenience function to set vertex attribute
		/// \param attribute_slot index
		/// \param res Attribute buffer to bind
        void                                    set_attribute_buffer    (DTint attribute_slot, std::shared_ptr<DT3GLAttribBufferResource> res);

		//
		// Mappings for uniforms
		//
    
		/// Add an entry for the uniform value so that it can be accessed in this shader.
		/// \param name name of uniform
		/// \param type Type of uniform
		/// \param array_size number of elements in the unform array
		void                                    add_uniform             (   const std::string &name,
                                                                            DT3GLUniformFormat type,
                                                                            DTint array_size,
                                                                            DT3GLStandardUniform standard_uniform = DT3GL_UNIFORM_NONE);

		/// Gets the index of a uniform
		/// \param name name of uniform
		/// \return index of uniform
		DTint                                   uniform_slot            (const std::string &name);

		/// Gets the index of a standard uniform
		/// \param standard_uniform uniform id
		/// \return index of uniform
		DTint                                   uniform_slot            (DT3GLStandardUniform standard_uniform);

        /// Convenience function to set uniform values
		/// \param uniform_slot index
        void                                    set_uniform_value       (DTint uniform_slot, DTint buffer_data);
        void                                    set_uniform_value       (DTint uniform_slot, const Vector4 &buffer_data);
        void                                    set_uniform_value       (DTint uniform_slot, const std::vector<Vector4> &buffer_data);
        void                                    set_uniform_value       (DTint uniform_slot, const Matrix4 &buffer_data);
        void                                    set_uniform_value       (DTint uniform_slot, const std::vector<Matrix4> &buffer_data);
        void                                    set_uniform_value       (DTint uniform_slot, const Color4f &buffer_data);

		/// Activate the resource
        void                                    activate                (void);

        /// Attach attribute to the shader
        /// \param attribute_slot attribute slot
        /// \param res attribute buffer
        void                                    attach_attribute_buffer (DTint attribute_slot, std::shared_ptr<DT3GLAttribBufferResource> res);
    
        /// Attach uniform to the shader
        /// \param uniform_slot uniform slot
        /// \param res uniform buffer
        void                                    attach_uniform_buffer   (DTint uniform_slot, std::shared_ptr<DT3GLUniformResource> res);

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<ShaderResource>  import_resource         (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                               _shader_map_lock;
		static std::map<std::string, std::shared_ptr<ShaderResource>>   _shader_map;

        static void                             reload_if_changed       (void);
        void                                    recompile_if_needed     (void);

        void                                    screen_opened           (DTuint width, DTuint height);
        void                                    screen_closed           (void);
    
    
        struct MappingUniform {
            std::string                                 _name;
            DTint                                       _index;
            DTint                                       _array_size;
            DT3GLUniformFormat                          _type;
            DT3GLStandardUniform                        _standard_uniform;

            std::shared_ptr<DT3GLUniformResource>       _uniform_resource;
        };
		std::vector<MappingUniform>                     _uniforms;
        DTint                                           _standard_uniforms[DT3GL_UNIFORM_NUM];


        struct MappingAttribute {
            std::string                                 _name;
            DTint                                       _index;
            DT3GLStandardAttrib                         _standard_attrib;
            
            std::shared_ptr<DT3GLAttribBufferResource>  _attrib_buffer_resource;
        };
		std::vector<MappingAttribute>                   _attribs;
        DTint                                           _standard_attribs[DT3GL_ATTRIB_NUM];

		std::map<std::string,std::vector<std::string>>  _fragment_shaders;
		std::map<std::string,std::vector<std::string>>  _geometry_shaders;
		std::map<std::string,std::vector<std::string>>  _vertex_shaders;
    
        DTboolean                                       _needs_recompile;
    
        std::shared_ptr<DT3GLShaderResource>            _shader;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<ShaderResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<ShaderResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
