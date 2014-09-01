#ifndef DT3_GEOMETRYRESOURCE
#define DT3_GEOMETRYRESOURCE
//==============================================================================
///	
///	File: GeometryResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include "DT3Core/Types/Graphics/Mesh.hpp"
#include "DT3Core/Types/Animation/Skeleton.hpp"
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;

//==============================================================================
/// Geometry resource type
//==============================================================================

class GeometryResource: public Resource {
    public:
        DEFINE_TYPE(GeometryResource,Resource)
		DEFINE_CREATE
    
                                        GeometryResource		(void);	
	private:
                                        GeometryResource		(const GeometryResource &rhs);
        GeometryResource &              operator =				(const GeometryResource &rhs);	
	public:
        virtual                         ~GeometryResource		(void);
                
    public:
		/// Called to initialize the class
		static void                     initialize_static       (void);

		/// Called to uninitialize the class
		static void                     uninitialize_static     (void);

		/// Called to initialize the object
		virtual void                    initialize              (void);


		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		virtual DTerr                   import					(const FilePath &pathname, std::string args);
		
		/// Adds a mesh to the geometry
        /// \param m Mesh to add
        void                            add_mesh                (const std::shared_ptr<Mesh> &m);

		/// Removes a mesh from the geometry
        /// \param m Mesh to remove
        void                            remove_mesh             (const std::shared_ptr<Mesh> &m);

		/// Removes all meshes from the geometry
        void                            remove_all_meshes       (void);
    
		/// Returns all meshes from the geometry
        const std::vector<std::shared_ptr<Mesh>>& meshes        (void) const        {   return _meshes; }


		/// Sets the skeleton for the mesh
        /// \param skeleton new skeleton
        void                            set_skeleton            (const Skeleton &skeleton)  {   _skeleton = skeleton;   }
    
        /// Returns the skeleton for the mesh
		/// \return Skeleton
        const Skeleton&                 skeleton                (void) const        {   return _skeleton;   }


        /// Draws the resource
        /// \param camera Camera used for drawing
        void                            draw                    (const std::shared_ptr<CameraObject> &camera);

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<GeometryResource>   import_resource (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                               _geometry_map_lock;
		static std::map<std::string, std::shared_ptr<GeometryResource>> _geometry_map;
    
        static void                     reload_if_changed       (void);

        void                            screen_opened           (DTuint width, DTuint height);
        void                            screen_closed           (void);

        std::vector<std::shared_ptr<Mesh>>      _meshes;
        Skeleton                                _skeleton;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<GeometryResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<GeometryResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
