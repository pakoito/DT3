#ifndef DT3_SOUNDRESOURCE
#define DT3_SOUNDRESOURCE
//==============================================================================
///	
///	File: SoundResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundPacket;
class ImporterSound;

//==============================================================================
/// Sound resource type
//==============================================================================

class SoundResource: public Resource {
    public:
        DEFINE_TYPE(SoundResource,Resource)
		DEFINE_CREATE

                                        SoundResource		(void);
	private:
                                        SoundResource		(const SoundResource &rhs);
        SoundResource &                 operator =			(const SoundResource &rhs);
	public:
        virtual                         ~SoundResource      (void);
                
    public:        	
		/// Called to initialize the class
		static void                     initialize_static   (void);

		/// Called to uninitialize the class
		static void                     uninitialize_static (void);

		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr                           import				(const FilePath &pathname, std::string args);
		
		enum Format {
			FORMAT_MONO16,
			FORMAT_STEREO16
		};
        
		/// Sets the format of the sound file.
		/// \param format format of sound file
		void                            set_format          (Format format)         {	_format = format;			}

		/// Returns format of the sound file
		/// \return format of sound file
		Format                          format              (void) const            {	return (Format) _format;	}

		/// Sets frequency of sound data
		/// \param frequency frequency of sound data
		void                            set_frequency       (DTuint frequency)		{	_frequency = frequency;		}

		/// Returns frequency of sound data
		/// \return frequency of sound data
		DTuint                          frequency           (void) const            {	return _frequency;			}

		/// Returns the raw sound data of streaming sound
		/// \param packet sound packet
		/// \param chunk_index index of streaming chunk
		void                            streamed_chunk		(SoundPacket &packet, DTsize chunk_index) const;
		
		/// Returns the number of streaming chunks given chunk size
		/// \return number of streaming chunks
		DTsize                          count_chunks        (void) const;

		/// Returns the number of bytes of data
		/// \return number of bytes
        DTsize                          length              (void) const;
        	
		/// Returns the number of samples
		/// \return number of samples
        DTsize                          num_samples         (void) const;
        	
		/// Returns a sample. Useful for previews, etc.
		/// \param s sample index
		/// \return sample
        DTfloat                         sample              (DTsize s) const;

		/// Returns a representative sample for a window. Useful for previews, etc.
		/// \param s sample index
		/// \return sample
        void                            sample_window       (DTsize s, DTsize num, DTfloat &min_sample, DTfloat &max_sample) const;


		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<SoundResource>  import_resource (const FilePath &pathname, std::string args = "");

    private:
        static std::mutex                                               _sound_map_lock;
		static std::map<std::string, std::shared_ptr<SoundResource>>    _sound_map;

        static void                     reload_if_changed   (void);

        DTuint                          _format;
        DTuint                          _frequency;
        				
		std::shared_ptr<ImporterSound>  _importer;
		
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<SoundResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<SoundResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
