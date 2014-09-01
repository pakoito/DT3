//==============================================================================
///	
///	File: World_test.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"
#include "DT3Core/System/Factory.hpp"

#if DT3_UNIT_TESTS

#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class World_test: public UnitTest {
    public:
        DEFINE_TYPE(World_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (World_test)

//==============================================================================
// Implement the test
//==============================================================================

void World_test::run_test (void)
{
/*	

	// add a bunch of objects to the world
	World *world = World::create();
	
	for (DTint i = 0; i < 100; ++i) {
		WorldNode *obj = WorldNode::create();
		
		world->addObject(obj);
		obj->expire();
		world->tick(0.0F);
	
		RELEASE(obj);
	}
	
	RELEASE(world);
*/
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS

