#ifndef DT3_KEYEDSPRITEANIMATIONRESOURCE
#define DT3_KEYEDSPRITEANIMATIONRESOURCE
//==============================================================================
///	
///	File: KeyedSpriteAnimationResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "Resource.hpp"
#include <map>

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class KeyedSpriteAnimation;

//==============================================================================
/// Class
//==============================================================================

class KeyedSpriteAnimationResource: public Resource {
    public:
        DEFINE_TYPE(KeyedSpriteAnimationResource,Resource)
		DEFINE_CREATE
        DEFINE_STANDARD_RESOURCE_HANDLERS
        
                                                    KeyedSpriteAnimationResource	(void);	
	private:
                                                    KeyedSpriteAnimationResource	(const KeyedSpriteAnimationResource &rhs);
        KeyedSpriteAnimationResource &              operator =                      (const KeyedSpriteAnimationResource &rhs);	
	public:
		virtual                                     ~KeyedSpriteAnimationResource	(void);
                
        virtual void                                archive_read        (Archive *archive);
        virtual void                                archive_write		(Archive *archive);

    public:        	
	
		/// import the resource with an importer into a platform independent format
		/// \param param description
		/// \return description
		DTerr										import				(const FilePath &pathname, std::string args);

		/// Description
		/// \param param description
		/// \return description
		DTerr										recache				(void);

		/// Description
		/// \param param description
		/// \return description
        void                                        copy                (const KeyedSpriteAnimationResource &copy);

		/// Description
		/// \param param description
		/// \return description
		void										add_animation       (KeyedSpriteAnimation *animation);

		/// Description
		/// \param param description
		/// \return description
		void										delete_animation    (KeyedSpriteAnimation *animation);

		/// Description
		/// \param param description
		/// \return description
		KeyedSpriteAnimation*                       animation_by_name   (const std::string &animation_name);

		/// Returns all of the tracks
		/// \param param description
		/// \return description
        std::list<KeyedSpriteAnimation*>&           animations          (void)									{   return _animations;		}

		/// Description
		/// \param param description
		/// \return description
		void										clear_animations    (void);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<KeyedSpriteAnimationResource>   keyed_sprite_animation   (const FilePath &pathname, std::string args = "");

	private:
        static SpinLock                             _keyed_sprite_animation_map_lock;
		static std::map<std::string, std::shared_ptr<KeyedSpriteAnimationResource> >	_keyed_sprite_animation_map;

		std::list<KeyedSpriteAnimation*>            _animations;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, KeyedSpriteAnimationResource* &r);
Stream& operator >>(Stream &s, KeyedSpriteAnimationResource* &r);

Stream& operator <<(Stream &s, const std::shared_ptr<KeyedSpriteAnimationResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<KeyedSpriteAnimationResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
