//==============================================================================
///	
///	File: DeviceMusicMediaFoundation.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
//
// NOTE: See http://msdn.microsoft.com/en-us/library/windows/desktop/ms703190%28v=vs.85%29.aspx
//
//==============================================================================

#include "DeviceMusicMediaFoundation.hpp"

#include "Factory.hpp"
#include "FilePath.hpp"
#include "System.hpp"
#include "DeviceGlobalsManager.hpp"
#include "StringCast.hpp"
#include "DeviceConsole.hpp"
#include "DeviceInput.hpp"
#include "DeviceAudio.hpp"
#include "DeviceFileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceMusicMediaFoundation)

//==============================================================================
//==============================================================================


//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceMusicMediaFoundation::DeviceMusicMediaFoundation (void)
    :   _music_master_volume(1.0F),
		_music_volume		(0.0F),
		_music_volume_vel	(0.0F),
		_old_volume			(-1.0F),
		_tick_accum			(0.0F),
        _loop               (true),
        _mf_engine          (NULL)
{
    HRESULT hr = MFStartup(MF_VERSION,MFSTARTUP_LITE);
    Assert(SUCCEEDED(hr));

    // Build callback object
    _mf_notify = new DeviceMusicMediaFoundationNotify(this);

    // Create a class factory so we can create a media engine
    IMFMediaEngineClassFactory *factory;
    hr = CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        return;
    }

    // Create an instance of the media engine
    IMFAttributes *attributes;
    hr = MFCreateAttributes(&attributes, 1);
    if (FAILED(hr)) {
        SAFE_RELEASE(factory);
        return;
    }



    // Set up attributes for media engine

    hr = attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, _mf_notify);

    if (FAILED(hr)) {
        SAFE_RELEASE(attributes);
        SAFE_RELEASE(factory);
        return;
    }


    // Create an instance of the media engine

    hr = factory->CreateInstance(MF_MEDIA_ENGINE_AUDIOONLY, attributes, &_mf_engine);

    if (FAILED(hr)) {
        SAFE_RELEASE(attributes);
        SAFE_RELEASE(factory);
        return;
    }


    // Done with factory and attributes

    SAFE_RELEASE(attributes);
    SAFE_RELEASE(factory);
}

DeviceMusicMediaFoundation::~DeviceMusicMediaFoundation (void)
{
	stopMusic();

    SAFE_RELEASE(_mf_notify);
    SAFE_RELEASE(_mf_engine);

    HRESULT hr = MFShutdown();
    Assert(SUCCEEDED(hr));
}


//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::startMusic (const FilePath &pathname)
{		
	if (pathname.isBlank() || !_mf_engine)
		return;	
	    
    // Build the URL for the file

    HRESULT hr = MFCreateURLFromPath(pathname.getPath().cStr(), LPWSTR *ppwszFileURL);

    HRESULT hr = UrlCreateFromPathA(pathname.getPath().cStr(), url, INTERNET_MAX_URL_LENGTH, NULL);  

    if (FAILED(hr)) {

        return;

    }



    	
	// set the default volume	
	DTfloat music_gain = System::getGlobalsManager()->hasGlobal("SYS_MUSIC_GAIN") ? castFromString<DTfloat>(System::getGlobalsManager()->getGlobal("SYS_MUSIC_GAIN")) : 1.0F;
	DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
	_old_volume = new_vol;
	

	
	
	// set the current music
	_music_current = pathname;
}

//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::playMusic	(const FilePath &pathname, DTfloat transition, DTboolean loop)	
{
	_music_volume_vel = 1.0F / transition;
	_music_pending = pathname;
    _loop = loop;
}

//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::stopMusic (DTfloat transition)
{
	_music_volume_vel = -1.0F / transition;
}

void DeviceMusicMediaFoundation::stopMusic (void)	
{
		
    _file.close();
	_music_current.clear();
}

//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::setVolume (DTfloat volume)
{
	_music_master_volume = volume;
}

//==============================================================================
//==============================================================================

DTfloat DeviceMusicMediaFoundation::getProgress (void)
{
    if (getPlaying())
        return 0.5F;
    else
        return 0.0F;
        
}

DTboolean DeviceMusicMediaFoundation::getPlaying (void)
{
    return false;
}

//==============================================================================
//==============================================================================

FilePath DeviceMusicMediaFoundation::getCurrentMusic (void)
{
    return _music_current;
}

//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::tick (const DTfloat dt)
{
	// Only tick at 10 FPS
	const DTfloat TICK_PERIOD = 0.1F;
	_tick_accum += dt;
	if (_tick_accum > TICK_PERIOD) {
		_tick_accum -= TICK_PERIOD;

		//
		// Music
		//
		
		DTboolean music_on = castFromString<DTboolean>(System::getGlobalsManager()->getGlobal("SYS_MUSIC"));
		DTfloat music_gain = System::getGlobalsManager()->hasGlobal("SYS_MUSIC_GAIN") ? castFromString<DTfloat>(System::getGlobalsManager()->getGlobal("SYS_MUSIC_GAIN")) : 1.0F;

		// Check for music off
		if (!music_on || music_gain <= 0.01F) {
			if (_music_current.getPath().size() > 0)
				_music_pending = _music_current;
			_music_current.clear();
			stopMusic ();
			return;
		}

		// Trying to play song already playing so ignore it
		if (_music_pending == _music_current)
			_music_pending.clear();

		// Swapping music
		if (!_music_pending.isBlank() && !_music_current.isBlank()) {
				
			// transition to new music
			_music_volume -= _music_volume_vel * TICK_PERIOD;
			
			if (_music_volume < 0.0F) {
				stopMusic ();

				_music_volume = 0.0F;
				startMusic (_music_pending);
				_music_pending.clear();

			}
            
            DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
            if (_mf_engine && new_vol != _old_volume) {
                _mf_engine->SetVolume(new_vol);
                _old_volume = new_vol;
            }

		// Ramping music
		} else if (!_music_current.isBlank()) {
		
			_music_volume += _music_volume_vel * TICK_PERIOD;
			if (_music_volume > 1.0F) {
				_music_volume = 1.0F;
                _music_volume_vel = 0.0F;
			} else if (_music_volume < 0.0F) {
                stopMusic ();

				_music_volume = 0.0F;
                _music_volume_vel = 0.0F;
			}
			
			DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
            if (_mf_engine && new_vol != _old_volume) {
                _mf_engine->SetVolume(new_vol);
                _old_volume = new_vol;
            }
				
		// Music pending and existing music is playing
		} else if (!_music_pending.isBlank()) {
			stopMusic ();

			_music_volume = 1.0F;
			_music_volume_vel = 0.0F;
			startMusic (_music_pending);
			_music_pending.clear();

			DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
            if (_mf_engine && new_vol != _old_volume) {
                _mf_engine->SetVolume(new_vol);
                _old_volume = new_vol;
            }
		}
		
		// Loop music
		if (_mf_engine /*&& _aq_info._done*/) {
			FilePath music_current = _music_current;	// Stopping music clears this
			stopMusic ();
            
            if (_loop)
                startMusic (music_current);
		}
	}
}

//==============================================================================
//==============================================================================

void DeviceMusicMediaFoundation::emergencyFreeMemory	(void)
{
	//stopMusic();
}

//==============================================================================
//==============================================================================

} // DT2
