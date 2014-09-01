#ifndef DT3_SCRIPTINGMUSICPLAYER
#define DT3_SCRIPTINGMUSICPLAYER
//==============================================================================
///	
///	File: ScriptingMusicPlayer.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Controls Music playback.
//==============================================================================

class ScriptingMusicPlayer: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMusicPlayer,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingMusicPlayer	(void);	
									ScriptingMusicPlayer	(const ScriptingMusicPlayer &rhs);
        ScriptingMusicPlayer &		operator =				(const ScriptingMusicPlayer &rhs);	
        virtual                     ~ScriptingMusicPlayer	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
    
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Play Event
        void                        play                    (PlugNode *sender);

		/// Stop Event
        void                        stop                    (PlugNode *sender);

	private:			        
        Event                       _play;
        Event                       _stop;
        
		Plug<FilePath>				_path;
        Plug<DTfloat>				_gain;

		Plug<DTboolean>				_is_playing;
		Plug<DTboolean>				_stop_on_destroy;

		Plug<DTboolean>				_loop;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
