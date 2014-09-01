//==============================================================================
///	
///	File: PlugNode_test.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"

#if DT3_UNIT_TESTS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include <thread>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class PlugNode_test: public UnitTest {
    public:
        DEFINE_TYPE(PlugNode_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (PlugNode_test)

//==============================================================================
// Implement the test
//==============================================================================

IMPLEMENT_PLUG_INFO_INDEX(_a_int_1)
IMPLEMENT_PLUG_INFO_INDEX(_a_int_2)
IMPLEMENT_PLUG_INFO_INDEX(_a_int_out_1)
IMPLEMENT_PLUG_INFO_INDEX(_a_int_out_2)

IMPLEMENT_PLUG_INFO_INDEX(_b_int_1)
IMPLEMENT_PLUG_INFO_INDEX(_b_int_2)
IMPLEMENT_PLUG_INFO_INDEX(_b_int_out_1)
IMPLEMENT_PLUG_INFO_INDEX(_b_int_out_2)

IMPLEMENT_PLUG_INFO_INDEX(_c_int_1)
IMPLEMENT_PLUG_INFO_INDEX(_c_int_2)
IMPLEMENT_PLUG_INFO_INDEX(_c_int_out_1)
IMPLEMENT_PLUG_INFO_INDEX(_c_int_out_2)

//==============================================================================
//==============================================================================

struct A: public PlugNode {
	DEFINE_TYPE(A,PlugNode)
	DEFINE_PLUG_NODE

	A () 
		:	_a_int_out_1(PLUG_INFO_INDEX(_a_int_out_1),0),
			_a_int_out_2(PLUG_INFO_INDEX(_a_int_out_2),0),
            _a_int_1	(PLUG_INFO_INDEX(_a_int_1),0),
			_a_int_2	(PLUG_INFO_INDEX(_a_int_2),0)
	{

	}
	
	virtual DTboolean compute (const PlugBase *plug)
	{
		if (plug == &_a_int_out_1) {
			LOG_MESSAGE << "_a_int_out_1: Computing " << (_a_int_1) << " + " << (_a_int_2);
			_a_int_out_1 = (_a_int_1) + (_a_int_2);
			_a_int_out_1.set_clean();
			return true;
		} else if (plug == &_a_int_out_2) {
			LOG_MESSAGE << "_a_int_out_2: Computing " << (_a_int_1) << " * " << (_a_int_2);
			_a_int_out_2 = (_a_int_1) * (_a_int_2);
			_a_int_out_2.set_clean();
			return true;
		}
		return false;
	
	}

	Plug<DTint>		_a_int_1;
	Plug<DTint>		_a_int_2;
	Plug<DTint>		_a_int_out_1;
	Plug<DTint>		_a_int_out_2;
};

BEGIN_IMPLEMENT_PLUGS(A)

	PLUG_INIT(_a_int_1,"_a_int_1")
        .affects(PLUG_INFO_INDEX(_a_int_out_1))
        .affects(PLUG_INFO_INDEX(_a_int_out_2));

	PLUG_INIT(_a_int_2,"_a_int_2")
        .affects(PLUG_INFO_INDEX(_a_int_out_1))
        .affects(PLUG_INFO_INDEX(_a_int_out_2));
    
	PLUG_INIT(_a_int_out_1,"_a_int_out_1");
	PLUG_INIT(_a_int_out_2,"_a_int_out_2");

END_IMPLEMENT_PLUGS

struct B: public PlugNode {
	DEFINE_TYPE(B,PlugNode)
	DEFINE_PLUG_NODE

	B () 
		:	_b_int_out_1(PLUG_INFO_INDEX(_b_int_out_1),0),
			_b_int_out_2(PLUG_INFO_INDEX(_b_int_out_2),0),
            _b_int_1	(PLUG_INFO_INDEX(_b_int_1),0),
			_b_int_2	(PLUG_INFO_INDEX(_b_int_2),0)
	{

	}
		
	virtual DTboolean compute (const PlugBase *plug)
	{
		if (plug == &_b_int_out_1) {
			LOG_MESSAGE << "_b_int_out_1: Computing " << (_b_int_1) << " + " << (_b_int_2);
			_b_int_out_1 = (_b_int_1) + (_b_int_2);
			_b_int_out_1.set_clean();
			return true;
		} else if (plug == &_b_int_out_2) {
			LOG_MESSAGE << "_b_int_out_2: Computing " << (_b_int_1) << " * " << (_b_int_2);
			_b_int_out_2 = (_b_int_1) * (_b_int_2);
			_b_int_out_2.set_clean();
			return true;
		}
		return false;
	
	}

	Plug<DTint>		_b_int_1;
	Plug<DTint>		_b_int_2;
	Plug<DTint>		_b_int_out_1;
	Plug<DTint>		_b_int_out_2;
};

BEGIN_IMPLEMENT_PLUGS(B)
	PLUG_INIT(_b_int_1,"_b_int_1")
        .affects(PLUG_INFO_INDEX(_b_int_out_1))
        .affects(PLUG_INFO_INDEX(_b_int_out_2));

	PLUG_INIT(_b_int_2,"_b_int_2")
        .affects(PLUG_INFO_INDEX(_b_int_out_1))
        .affects(PLUG_INFO_INDEX(_b_int_out_2));
                
	PLUG_INIT(_b_int_out_1,"_b_int_out_1");
	PLUG_INIT(_b_int_out_2,"_b_int_out_2");

END_IMPLEMENT_PLUGS


struct C: public PlugNode {
	DEFINE_TYPE(C,PlugNode)
	DEFINE_PLUG_NODE

	C () 
		:	_c_int_out_1(PLUG_INFO_INDEX(_c_int_out_1),0),
			_c_int_out_2(PLUG_INFO_INDEX(_c_int_out_2),0),
            _c_int_1	(PLUG_INFO_INDEX(_c_int_1),0),
			_c_int_2	(PLUG_INFO_INDEX(_c_int_2),0)
	{

	}
	
	virtual DTboolean compute (const PlugBase *plug)
	{
		if (plug == &_c_int_out_1) {
			LOG_MESSAGE << "_c_int_out_1: Computing " << (_c_int_1) << " + " << (_c_int_2);
			_c_int_out_1 = (_c_int_1) + (_c_int_2);
			_c_int_out_1.set_clean();
			return true;
		} else if (plug == &_c_int_out_2) {
			LOG_MESSAGE << "_c_int_out_2: Computing " << (_c_int_1) << " * " << (_c_int_2);
			_c_int_out_2 = (_c_int_1) * (_c_int_2);
			_c_int_out_2.set_clean();
			return true;
		}	
		return false;
	}

	Plug<DTint>		_c_int_1;
	Plug<DTint>		_c_int_2;
	Plug<DTint>		_c_int_out_1;
	Plug<DTint>		_c_int_out_2;
};

BEGIN_IMPLEMENT_PLUGS(C)
    // Set up plugs
	PLUG_INIT(_c_int_1,"_c_int_1")
        .affects(PLUG_INFO_INDEX(_c_int_out_1))
        .affects(PLUG_INFO_INDEX(_c_int_out_2));

	PLUG_INIT(_c_int_2,"_c_int_2")
        .affects(PLUG_INFO_INDEX(_c_int_out_1))
        .affects(PLUG_INFO_INDEX(_c_int_out_2));
    
	PLUG_INIT(_c_int_out_1,"_c_int_out_1");
	PLUG_INIT(_c_int_out_2,"_c_int_out_2");
END_IMPLEMENT_PLUGS

IMPLEMENT_PLUG_NODE(A)
IMPLEMENT_PLUG_NODE(B)
IMPLEMENT_PLUG_NODE(C)

//==============================================================================
//==============================================================================

IMPLEMENT_EVENT_INFO_INDEX(_e1)
IMPLEMENT_EVENT_INFO_INDEX(_e2)

struct E1: public PlugNode {
	DEFINE_TYPE(E1,PlugNode)
	DEFINE_PLUG_NODE

	E1 () 
		:	_e1	(EVENT_INFO_INDEX(_e1))
    {

	}
    
    void e1_trigger(PlugNode *sender)
    {
        LOG_MESSAGE << "e1_trigger called";
        
        _e1.send();
    }
	   
	Event	_e1;
};

BEGIN_IMPLEMENT_PLUGS(E1)

    // Set up plugs
	EVENT_INIT(_e1,"_e1")
        .set_output(true)
        .set_event(&E1::e1_trigger);

END_IMPLEMENT_PLUGS


struct E2: public PlugNode {
	DEFINE_TYPE(E2,PlugNode)
	DEFINE_PLUG_NODE

	E2 () 
		:	_e2	(EVENT_INFO_INDEX(_e2))
    {

	}
    
    void e2_trigger(PlugNode *sender)
    {
        LOG_MESSAGE << "e2_trigger called";
    }
	
	Event	_e2;
};

BEGIN_IMPLEMENT_PLUGS(E2)
    // Set up plugs
	EVENT_INIT(_e2,"_e2")
        .set_input(true)
        .set_event(&E2::e2_trigger);

END_IMPLEMENT_PLUGS


IMPLEMENT_PLUG_NODE(E1)
IMPLEMENT_PLUG_NODE(E2)

//==============================================================================
//==============================================================================

namespace {

void do_thread (A *a, B *b, C *c)
{
    
    for (DTuint i = 0; i < 1000; ++i) {
    
        DTuint r = MoreMath::random_int() % 3;
        switch (r) {
            case 0:
                a->_a_int_1 = MoreMath::random_int() % 10;
                break;
            case 1:
                b->_b_int_2 = MoreMath::random_int() % 10;
                break;
            case 2:
                DTint val = c->_c_int_out_2;
                ++val;
                break;
        }
        
    }
}

}

void PlugNode_test::run_test (void)
{
    LOG_MESSAGE << "Sizeof Plug<DTuint>: " << (DTsize) sizeof(Plug<DTuint>);
    LOG_MESSAGE << "Sizeof Event: " << (DTsize) sizeof(Event);
    
    TEST_ASSERTION(sizeof(Plug<DTuint>) == 8);
    TEST_ASSERTION(sizeof(Event) == 4);
    
    LOG_MESSAGE << "Testing Events";
    
    E1 obj_e1;
    E2 obj_e2;

    obj_e2._e2.add_incoming_connection(&obj_e1._e1);
    obj_e1._e1.send(NULL);

    LOG_MESSAGE << "Testing Plugs";

	A obj_A;
	B obj_B;
	C obj_C;

	LOG_MESSAGE << "Plug Size " << static_cast<DTuint>(sizeof(obj_A._a_int_1));
	
	obj_A._a_int_1 = 2;
	obj_A._a_int_2 = 3;
    
    TEST_ASSERTION(obj_A._a_int_out_1 == 5);
    TEST_ASSERTION(obj_A._a_int_out_2 == 6);
	
	LOG_MESSAGE << "----Reading _a_int_out_1";
	LOG_MESSAGE << (obj_A._a_int_out_1);
	LOG_MESSAGE << "----Reading _a_int_out_2";
	LOG_MESSAGE << (obj_A._a_int_out_2);
	
	LOG_MESSAGE << "----Reading _a_int_out_1";
	LOG_MESSAGE << (obj_A._a_int_out_1);
	LOG_MESSAGE << "----Reading _a_int_out_2";
	LOG_MESSAGE << (obj_A._a_int_out_2);
	
	obj_A._a_int_1 = 10;
	obj_A._a_int_2 = 20;
	
	obj_B._b_int_1.set_incoming_connection(&obj_A._a_int_out_1);
	obj_B._b_int_2.set_incoming_connection(&obj_A._a_int_out_2);

	LOG_MESSAGE << "----Reading _b_int_out_1";
	LOG_MESSAGE << (obj_B._b_int_out_1);
	LOG_MESSAGE << "----Reading _b_int_out_2";
	LOG_MESSAGE << (obj_B._b_int_out_2);
	
	LOG_MESSAGE << "----Reading _b_int_out_1";
	LOG_MESSAGE << (obj_B._b_int_out_1);
	LOG_MESSAGE << "----Reading _b_int_out_2";
	LOG_MESSAGE << (obj_B._b_int_out_2);
	
	obj_C._c_int_1.set_incoming_connection(&obj_B._b_int_out_1);
	obj_C._c_int_2.set_incoming_connection(&obj_B._b_int_out_2);

	LOG_MESSAGE << "----Reading _c_int_out_1";
	LOG_MESSAGE << (obj_C._c_int_out_1);
	LOG_MESSAGE << "----Reading _c_int_out_2";
	LOG_MESSAGE << (obj_C._c_int_out_2);
	
	LOG_MESSAGE << "----Reading _c_int_out_1";
	LOG_MESSAGE << (obj_C._c_int_out_1);
	LOG_MESSAGE << "----Reading _c_int_out_2";
	LOG_MESSAGE << (obj_C._c_int_out_2);
	
	
	obj_A._a_int_1 = 1;
	obj_A._a_int_2 = 2;
	
	LOG_MESSAGE << "----Reading _c_int_out_1";
	LOG_MESSAGE << (obj_C._c_int_out_1);
	LOG_MESSAGE << "----Reading _c_int_out_2";
	LOG_MESSAGE << (obj_C._c_int_out_2);
	
	LOG_MESSAGE << "----Reading _c_int_out_1";
	LOG_MESSAGE << (obj_C._c_int_out_1);
	LOG_MESSAGE << "----Reading _c_int_out_2";
	LOG_MESSAGE << (obj_C._c_int_out_2);
    
    
    //
    // Threading Test
    //
    
    // Test thread safety
    std::thread t1(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t2(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t3(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t4(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t5(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t6(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t7(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t8(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t9(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
    std::thread t10(std::bind(do_thread, &obj_A, &obj_B, &obj_C));
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT_UNIT_TESTS

