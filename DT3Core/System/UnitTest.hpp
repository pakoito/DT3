#ifndef DT3_UNITTEST
#define DT3_UNITTEST
//==============================================================================
///	
///	File: UnitTest.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Defines a unit testing interface.
//==============================================================================

class UnitTest: public BaseClass {
    public:
        DEFINE_TYPE(UnitTest,BaseClass)
		
                                    UnitTest            (void);
									UnitTest            (const UnitTest &rhs);
        UnitTest &					operator =          (const UnitTest &rhs);
        virtual                     ~UnitTest           (void);
    
	public:
		/// Pure virtual function that contains the test in the subclass of this object.        
		virtual void				run_test            (void) = 0;


		/// Runs all of the tests registered in the object factory.        
		static void					run_all_tests		(void);


		/// Runs the test specified by "test".        
		/// \param test name of test class to instantiate and run. First the
		/// string is used as is, if it fails then "_test" is appended and tried
		/// again.
		static void					run_one_test		(std::string test);
};

//==============================================================================
//==============================================================================	
	
#define TEST_ASSERTION(cond) \
	{ \
		DTboolean correct = false; \
		correct = (cond); \
		if (!correct) { \
			LOG_MESSAGE << "*** Failed assertion: " << __FILE__ << " " << __LINE__; \
		} \
	}
	
} // DT3

#endif
