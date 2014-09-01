#ifndef DT3_IMPORTERANIMATIONTWA
#define DT3_IMPORTERANIMATIONTWA
//==============================================================================
///	
///	File: ImporterAnimationTWA.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterAnimation.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include <string>
#include <vector>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class BinaryFileStream;

//==============================================================================
/// Class
//==============================================================================

class ImporterAnimationTWA: public ImporterAnimation {
    public:
        DEFINE_TYPE(ImporterAnimationTWA,ImporterAnimation)
		DEFINE_CREATE
         
										ImporterAnimationTWA	(void);	
    
	private:
										ImporterAnimationTWA	(const ImporterAnimationTWA &rhs);
        ImporterAnimationTWA &			operator =				(const ImporterAnimationTWA &rhs);
    
    public:
        virtual							~ImporterAnimationTWA	(void);
    
    public:        	
		/// Imports an animation into an AnimationResource
		/// \param target object to import animation into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import					(AnimationResource *target, std::string args);
		
	private:
		enum {
			MAGIC = 0x5E11E70E,
			
			FILE = 0,
				ANIMATION = 1,
					ANIMATION_TRACKS = 2,
						ANIMATION_TRACK = 3
		};
		
		
		//
		// Animation
		//
		
		struct KeyFrame {
			DTfloat							_time;
			Quaternion						_rotation;
            Vector3                         _translation;
		};
		
		struct Track {
			std::vector<KeyFrame>           _keyframes;
		};
		
		struct Animation {
			DTuint							_fps;
			DTfloat							_start;
			DTfloat							_end;
		
			std::map<std::string,Track>     _tracks;
		};
		
		Animation							_animation;
		
		void		read_animation_keyframes(BinaryFileStream &file, DTuint remaining_size, std::vector<KeyFrame> &keyframes);
		void		read_animation_tracks   (BinaryFileStream &file, DTuint remaining_size, std::map<std::string,Track> &tracks);
		void		read_animation			(BinaryFileStream &file, DTuint remaining_size, Animation &animation);
		void		read_file				(BinaryFileStream &file, DTuint remaining_size);

};

//==============================================================================
//==============================================================================

} // DT3

#endif
