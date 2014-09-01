//==============================================================================
///	
///	File: ExpressionParser_test.cpp
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
#include "DT3Core/Types/Math/ExpressionParser.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class ExpressionParser_test: public UnitTest {
    public:
        DEFINE_TYPE(ExpressionParser_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (ExpressionParser_test)

//==============================================================================
// Implement the test
//==============================================================================

void ExpressionParser_test::run_test (void)
{
    ExpressionParser p;
    DTfloat result;
    
    p.parse("1+6/2");
    p.eval(result);
    LOG_MESSAGE << "Final result (4): " << result;

}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_UNIT_TESTS

