#ifndef DT2_DEVICEMUSICMEDIAFOUNDATION
#define DT2_DEVICEMUSICMEDIAFOUNDATION
//==============================================================================
///	
///	File: DeviceMusicMediaFoundation.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
/// NOTES: based on http://developer.apple.com/samplecode/AudioQueueTest/listing1.html
///
//==============================================================================

#include "pch.h"

#include "WinUtils.hpp"
#include "DeviceMusic.hpp"
#include "Array.hpp"
#include "BinaryFileStream.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
// Forward Declarations
//==============================================================================

class DeviceMusicMediaFoundation;

//==============================================================================
//==============================================================================

class DeviceMusicMediaFoundationNotify : public IMFMediaEngineNotify

{

    public:

        DeviceMusicMediaFoundationNotify (DeviceMusicMediaFoundation *c) 

            :   _num_refs   (1)

        {

            _class = c;

        }



        STDMETHODIMP QueryInterface(REFIID riid, void** ppv)

        {

            if(__uuidof(IMFMediaEngineNotify) == riid) {

                *ppv = static_cast<IMFMediaEngineNotify*>(this);

            } else {

                *ppv = nullptr;

                return E_NOINTERFACE;

            }



            AddRef();



            return S_OK;

        }      



        STDMETHODIMP_(ULONG) AddRef()

        {

            return InterlockedIncrement(&_num_refs);

        }



        STDMETHODIMP_(ULONG) Release()

        {

            LONG num_refs = InterlockedDecrement(&_num_refs);

            if (num_refs == 0) {

                delete this;

            }

            return num_refs;

        }



        // EventNotify is called when the Media Engine sends an event.

        STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD param2)

        {

            return S_OK;

        }



        /*void MediaEngineNotifyCallback(MediaEngineNotifyCallback^ pCB)

        {

            m_pCB = pCB;

        }

        */



    private:

        long                        _num_refs;

        DeviceMusicMediaFoundation  *_class;

};

//==============================================================================
//==============================================================================

class DeviceMusicMediaFoundation: public DeviceMusic {
		/// Mac and iOS music driver.
    public:
        DEFINE_TYPE(DeviceMusicMediaFoundation,DeviceMusic)
		DEFINE_CREATE

									DeviceMusicMediaFoundation		(void);
	private:
									DeviceMusicMediaFoundation		(const DeviceMusicMediaFoundation&);
        DeviceMusicMediaFoundation& operator =		        		(const DeviceMusicMediaFoundation&);
	public:
        virtual						~DeviceMusicMediaFoundation		(void);
        
	public:
		/// Shuts down the Music system
		virtual void				shutdownAudio			(void) {}

		/// Shuts down the Music system
		virtual void				restartAudio			(void) {}
       
		/// Plays the music file at the specified path
		/// \param pathname path to music file
        virtual void				playMusic				(const FilePath &pathname, DTfloat transition = 1.0F, DTboolean loop = true);	

		/// Stops the music
		/// \param pathname path to music file
        virtual void				stopMusic               (DTfloat transition);
        virtual void				stopMusic				(void);	
     
		/// Sets the music volume
		/// \param volume music volume
        virtual void				setVolume				(DTfloat volume);

		/// Returns how far the music has progressed
        /// \return progress
        virtual DTfloat             getProgress             (void);

		/// Returns whether a song is playing or not
        /// \return progress
        virtual DTboolean           getPlaying              (void);

		/// Returns the currently playing song
        /// \return Song
        virtual FilePath            getCurrentMusic         (void);

		/// Ticks the sound manager
		/// \param dt time step
        virtual void				tick					(const DTfloat dt);

		/// Called when an allocation has failed
		virtual void				emergencyFreeMemory     (void);

    private:
        void						startMusic				(const FilePath &pathname);	
        	
        //BinaryFileStream        _file;

		DTfloat					_music_master_volume;
		DTfloat					_music_volume;
		DTfloat					_music_volume_vel;
		
		DTfloat					_old_volume;
		
		FilePath				_music_current;
		FilePath				_music_pending;

		DTfloat					_tick_accum;
        DTboolean               _loop;

        IMFMediaEngine                      *_mf_engine;
        DeviceMusicMediaFoundationNotify    *_mf_notify;
		
};

//==============================================================================
//==============================================================================

} // DT2

#endif
