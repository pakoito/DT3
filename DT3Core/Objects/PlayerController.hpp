#ifndef DT3_PLAYERCONTROLLER
#define DT3_PLAYERCONTROLLER
//==============================================================================
///	
///	File: PlayerController.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Types/Network/NetworkAddress.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class DynamicObject;
class NetworkConnection;

//==============================================================================
/// Representation of a player in the game. This player could be local or
/// remote.
//==============================================================================

class PlayerController: public ObjectBase {    
    public:
        DEFINE_TYPE(PlayerController,ObjectBase)
		DEFINE_CREATE_AND_CLONE

                                    PlayerController	(void);	
                                    PlayerController	(const PlayerController &rhs);
        PlayerController &			operator =          (const PlayerController &rhs);
        virtual                     ~PlayerController	(void);
        
        virtual void                archive             (const std::shared_ptr<Archive> &archive);
	
	public:
};

//==============================================================================
//==============================================================================

} // DT3

#endif
