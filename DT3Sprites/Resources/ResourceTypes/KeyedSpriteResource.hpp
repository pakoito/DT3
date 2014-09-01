#ifndef DT3_KEYEDSPRITERESOURCE
#define DT3_KEYEDSPRITERESOURCE
//==============================================================================
///	
///	File: KeyedSpriteResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "Resource.hpp"
#include "MaterialResource.hpp"
#include "Matrix.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include <map>

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class KeyedSpriteResource: public Resource {
    public:
        DEFINE_TYPE(KeyedSpriteResource,Resource)
		DEFINE_CREATE
        DEFINE_STANDARD_RESOURCE_HANDLERS
        
                                                KeyedSpriteResource	(void);	
	private:    
                                                KeyedSpriteResource	(const KeyedSpriteResource &rhs);
        KeyedSpriteResource &                   operator =          (const KeyedSpriteResource &rhs);	
	public:
		virtual                                 ~KeyedSpriteResource(void);
                
        virtual void                            archive_read        (Archive *archive);
        virtual void                            archive_write		(Archive *archive);

    public:        	
	
		/// import the resource with an importer into a platform independent format
		/// \param param description
		/// \return description
		DTerr                                   import				(const FilePath &pathname, String args);

		/// Description
		/// \param param description
		/// \return description
		DTerr                                   recache				(void);


		/// Description
		/// \param param description
		/// \return description
        void                                    copy                (const KeyedSpriteResource &copy);


		/// Description
		/// \param param description
		/// \return description
		std::vector<KeyedSpriteAnimationPoseJoint*>&	children            (void)		{	return _children;	}

		/// Description
		/// \param param description
		/// \return description
		std::vector<KeyedSpriteAnimationPoseJoint*>&	joints              (void)		{	return _joints;		}
		
		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimationPoseJoint*			joint_by_name       (const String &joint_name);

		/// Description
		/// \param param description
		/// \return description
		void                                    clear_joints        (void);


		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimationPoseJoint*			parent_of_joint     (KeyedSpriteAnimationPoseJoint *joint);
		
		/// Description
		/// \param param description
		/// \return description
		std::vector<KeyedSpriteAnimationPoseJoint*>&	children_of_joint   (KeyedSpriteAnimationPoseJoint *joint);
		
		
		/// Description
		/// \param param description
		/// \return description
		void                                    reparent_joint      (KeyedSpriteAnimationPoseJoint *joint, KeyedSpriteAnimationPoseJoint *parent);

		/// Description
		/// \param param description
		/// \return description
		void                                    delete_joint        (KeyedSpriteAnimationPoseJoint *joint);

		/// Description
		/// \param param description
		/// \return description
		void                                    add_joint			(KeyedSpriteAnimationPoseJoint *joint, KeyedSpriteAnimationPoseJoint *parent = NULL);
		
		/// Description
		/// \param param description
		/// \return description
		void                                    move_to_front       (KeyedSpriteAnimationPoseJoint *joint);

		/// Description
		/// \param param description
		/// \return description
		void                                    move_to_back        (KeyedSpriteAnimationPoseJoint *joint);

		/// Description
		/// \param param description
		/// \return description
		void                                    update_transforms	(void);
    
		/// Description
		/// \param param description
		/// \return description
        void                                    update_order        (void);
		


		DEFINE_ACCESSORS_REF_COUNTED(material_property, set_material_property, MaterialResource*, _material);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<KeyedSpriteResource>    keyed_sprite (const FilePath &pathname, String args = "");

	private:
        void                                    recalculate_order   (void);
        static DTint                            comparison          (const KeyedSpriteAnimationPoseJoint *a, const KeyedSpriteAnimationPoseJoint *b);

        static SpinLock                                                 _keyed_sprite_map_lock;
		static std::map<String, std::shared_ptr<KeyedSpriteResource> >  _keyed_sprite_map;

		void                                    update_transforms_recursive (const Matrix3 *parent_transform, KeyedSpriteAnimationPoseJoint *joint);
				
		MaterialResource                        *_material;

		std::vector<KeyedSpriteAnimationPoseJoint*>	_children;
		std::vector<KeyedSpriteAnimationPoseJoint*>	_joints;

};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, KeyedSpriteResource* &r);
Stream& operator >>(Stream &s, KeyedSpriteResource* &r);

Stream& operator <<(Stream &s, const std::shared_ptr<KeyedSpriteResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<KeyedSpriteResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
