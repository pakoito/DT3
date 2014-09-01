#ifndef DT3_SOUNDSOURCE
#define DT3_SOUNDSOURCE
//==============================================================================
///	
///	File: SoundSource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundPacket;

//==============================================================================
/// Class
//==============================================================================

class SoundSource: public PlugNode {
    public:
        DEFINE_TYPE(SoundSource,PlugNode)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    SoundSource				(void);
                                    SoundSource             (const SoundSource &rhs);
        SoundSource &               operator =              (const SoundSource &rhs);
        virtual                     ~SoundSource            (void);
                
	public:
		/// Called to initialize the object
		virtual void				initialize              (void);

    public:        	
		/// Returns the velocity of the source.
		/// \return velocity
		const Vector3 &				velocity                (void) const                {   return _velocity.as_ref();		}

		/// Sets the velocity of the source.
		/// \param velocity velocity
        void						set_velocity            (const Vector3 &velocity)	{	_velocity = velocity;			}
        
	
		/// Returns the translation of the source.
		/// \return translation
		const Vector3 &				translation             (void) const				{   return _translation.as_ref();   }

		/// Sets the translation of the source.
		/// \param translation translation
        void						set_translation         (const Vector3 &translation){	_translation = translation;     }
		

		/// Returns the pitch of the source.
		/// \return pitch
		DTfloat						pitch                   (void) const				{   return _pitch;					}

		/// Sets the pitch of the source.
		/// \param pitch pitch
        void						set_pitch               (const DTfloat pitch)		{	_pitch = pitch;					}


		/// Returns the gain of the source.
		/// \return gain
		DTfloat						gain                    (void) const				{   return _gain;					}

		/// Sets the gain of the source.
		/// \param gain gain
        void						set_gain                (const DTfloat gain)		{	_gain = gain;					}


		/// Returns the Rolloff of the source.
		/// \return rolloff
		DTfloat						rolloff                 (void) const                {   return _rolloff;				}

		/// Sets the Rolloff of the source.
		/// \param rolloff rolloff
        void						set_rolloff             (const DTfloat rolloff)		{	_rolloff = rolloff;				}


		/// Returns wether the sound source is empty when the buffer is empty.
		/// \return ends on empty
		DTboolean					ends_on_empty           (void) const                    {   return _ends_on_empty;          }

		/// Returns wether the sound source is empty when the buffer is empty.
		/// \param ends_on_empty flag
        void						set_ends_on_empty       (const DTboolean ends_on_empty) {	_ends_on_empty = ends_on_empty; }


		/// Sends the event that the sound has started
        void                        start_sound             (void);
        
		/// Sends the event that the sound has stopped
        void                        stop_sound              (void);
        
    
		/// Returns the next sound packet for playing from this source
        virtual const SoundPacket&  next_sound_packet       (void);
        
	protected:
        Plug<SoundPacket>    _sound_packet;

		Plug<Vector3>		_translation;
		Plug<Vector3>		_velocity;
		Plug<DTfloat>		_pitch;
		Plug<DTfloat>		_gain;
		Plug<DTfloat>		_rolloff;
    
        DTboolean           _ends_on_empty;
        
        Event               _started;
        Event               _stopped;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
