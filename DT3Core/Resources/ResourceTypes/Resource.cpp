//==============================================================================
///	
///	File: Resource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Time.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Resource::Resource (void)
	:	_time_loaded        (0)
{    

}
			
Resource::~Resource (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr Resource::import (const FilePath &pathname, std::string args)
{
	_file_path = pathname;
	_args = args;
	
	_time_loaded = Time::seconds();

	// Spew to the log
    LOG_MESSAGE << "Loading " << _file_path.full_path() << " (UID = " << unique_id() << ")";
	
    return DT3_ERR_NONE;
}

DTerr Resource::reload (void)
{
    return import (_file_path, _args);
}

//==============================================================================
//==============================================================================

DTboolean Resource::is_changed (void) const
{	
    if (_time_loaded < _file_path.modification_date()) {
        return true;
    }
        
    // Check dependencies
    for (DTuint i = 0; i < _dependencies.size(); ++i) {
        if (_time_loaded < _dependencies[i].modification_date()) {
            return true;
        }
    }
    
    return false;
}

//==============================================================================
//==============================================================================

void Resource::add_dependencies(const std::vector<FilePath> &paths)
{
    _dependencies.insert( _dependencies.end(), paths.begin(), paths.end() );
}

//==============================================================================
//==============================================================================

} // DT3

