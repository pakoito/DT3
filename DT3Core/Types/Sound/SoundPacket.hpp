#ifndef DT3_SOUNDPACKET
#define DT3_SOUNDPACKET
//==============================================================================
///	
///	File: SoundPacket.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class SoundPacket: public BaseClass {
    public:
        DEFINE_TYPE(SoundPacket,BaseClass)
		DEFINE_CREATE_AND_CLONE

							SoundPacket				(void);	
							SoundPacket				(const SoundPacket &rhs);
        SoundPacket &		operator =				(const SoundPacket &rhs);
        virtual				~SoundPacket			(void);
                
    public: 
        /// Copies the Soundpacket
        void                copy                    (const SoundPacket &rhs);

		/// Gets Buffer for streaming data
		/// \return Buffer for streaming data
        DTubyte*            buffer                  (void)                  {   return _buffer ? &(*_buffer)[0] : NULL;        }
        const DTubyte*      buffer                  (void) const            {   return _buffer ? &(*_buffer)[0] : NULL;        }
        
        DTsize              num_bytes               (void) const			{	return _logical_size;           }
        void                set_num_bytes           (DTsize bytes);

        DTsize              num_samples             (void) const;
        void                set_num_samples         (DTsize samples);

        DEFINE_ACCESSORS	(format, set_format, SoundResource::Format, _format);
        DEFINE_ACCESSORS	(frequency, set_frequency, DTuint, _frequency);
    
        // Merges sound packets
        void                append_packet           (const SoundPacket &rhs);

		/// Keeps reading a plug until "num_bytes" bytes are in the packet. Note: this is meant to be
        /// called from within "compute"
		/// \param plug plug to read
		/// \param num_bytes target size of data
        void                append_bytes            (Plug<SoundPacket> &plug, DTsize num_bytes);

		/// Keeps reading a plug until "num_samples" samples are in the packet. Note: this is meant to be
        /// called from within "compute"
		/// \param plug plug to read
		/// \param num_bytes target size of data
        void                append_samples          (Plug<SoundPacket> &plug, DTsize num_samples);
        
        /// Removes "num_bytes" bytes from the beginning of the buffer
        /// \param num_bytes number of bytes to remove from the buffer
        void                remove_bytes            (DTsize num_bytes);
            
        /// Removes "num_samples" samples from the beginning of the buffer
        /// \param num_samples number of samples to remove from the buffer
        void                remove_samples          (DTsize num_samples);
    
    
        // Always different
        DTboolean           operator ==             (const SoundPacket& rhs) const  {   return false;   }
		DTboolean           operator !=             (const SoundPacket& rhs) const  {   return true;    }

    private:
        std::shared_ptr<std::vector<DTubyte>>   _buffer;
        
        DTsize                                  _logical_size;
    
        SoundResource::Format                   _format;
        DTuint                                  _frequency;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const SoundPacket&v);
Stream& operator >>(Stream &s, SoundPacket&v);

//==============================================================================
//==============================================================================

namespace TypeTraitsInfo {

template <> struct Info<SoundPacket> {
	static SoundPacket				default_value(void)	{	return SoundPacket();	}
	static const DTcharacter*       name(void)          {	return "SoundPacket";   }
	static const DTcharacter*       name_caps(void)     {	return "SoundPacket";}
    enum { isFundamental = 0 };
};

} // TypeTraitsDefaults

//==============================================================================
//==============================================================================

} // DT3

#endif
