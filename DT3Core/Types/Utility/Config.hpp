#ifndef DT3_CONFIG
#define DT3_CONFIG
//==============================================================================
///	
///	File: Config.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Config {
    public:        
                                    Config				(void);	
									Config				(const Config &rhs);
        Config &                    operator =			(const Config &rhs);	
        virtual                     ~Config				(void);

    public:        
			
		/// Returns the engine version
		/// \return engine version
		static DTuint				engine_version      (void);
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
