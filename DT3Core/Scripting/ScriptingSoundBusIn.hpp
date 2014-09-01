#ifndef DT3_SCRIPTINGSOUNDBUSIN
#define DT3_SCRIPTINGSOUNDBUSIN
//==============================================================================
///	
///	File: ScriptingSoundBusIn.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBase.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundResource;
class SoundBusSourceObject;

//==============================================================================
/// Audio bus input.
//==============================================================================

class ScriptingSoundBusIn: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundBusIn,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundBusIn     (void);	
									ScriptingSoundBusIn     (const ScriptingSoundBusIn &rhs);
        ScriptingSoundBusIn &		operator =				(const ScriptingSoundBusIn &rhs);	
        virtual                     ~ScriptingSoundBusIn    (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
        
        /// Adds a bus sound source
		/// \param source bus sound source
        void                        add_source              (const std::shared_ptr<SoundBusSourceObject> &source);
        
        /// Aemoves a bus sound source
		/// \param source bus sound source
        void                        remove_source           (const std::shared_ptr<SoundBusSourceObject> &source);
        
	private:		
		Plug<SoundPacket>     _sound_packet_out;
                
        struct BusSource {
            std::shared_ptr<SoundBusSourceObject>   _bus_source;
            SoundPacket                             _buffer;
        };
        
        std::list<BusSource>                        _bus_sources;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
