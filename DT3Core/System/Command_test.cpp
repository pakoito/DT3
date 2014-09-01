//==============================================================================
///	
///	File: Command_test.cpp
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

#include "DT3Core/System/Command.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class Command_test: public UnitTest {
    public:
        DEFINE_TYPE(Command_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (Command_test)

//==============================================================================
// Implement the test
//==============================================================================

class CommandStub: public Command {
    public:
        DEFINE_TYPE(CommandStub,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
        }
};

void Command_test::run_test (void)
{
    CommandStub c;
    
    std::string s("NODE:COMPONENT.EVENTORPLUG");
    std::string node, component, param;
    
    c.node_name(s,node);
    c.component_name(s,component);
    c.param_name(s,param);
    
	TEST_ASSERTION(node == "NODE");
	TEST_ASSERTION(component == "COMPONENT");
	TEST_ASSERTION(param == "EVENTORPLUG");
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS

