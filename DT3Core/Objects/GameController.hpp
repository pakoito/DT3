#ifndef DT3_GAMECONTROLLER
#define DT3_GAMECONTROLLER
//==============================================================================
///	
///	File: GameController.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ObjectBase.hpp"
#include <list>

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// The base game controller. The Game controller manages the game rules and
/// state and also manages the players.
//==============================================================================

class GameController: public ObjectBase {    
    public:
        DEFINE_TYPE(GameController,ObjectBase)

										GameController			(void);	
										GameController			(const GameController &rhs);
        GameController &				operator =				(const GameController &rhs);
        virtual							~GameController			(void);
    
        virtual void					archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		      
		/// Object was added to a world
		/// world world that object was added to
        virtual void                    add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                    remove_from_world       (void);

};

//==============================================================================
//==============================================================================

} // DT3

#endif
