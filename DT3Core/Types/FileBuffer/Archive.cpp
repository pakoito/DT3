//==============================================================================
///	
///	File: Archive.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Archive::Archive (void)
    :   _ignore_streamable_flag (false),
		_recursive				(true)
{

}
			
Archive::~Archive (void)
{
    std::shared_ptr<ArchiveProcess> process = pop_post_process();
	while (process) {
		process = pop_post_process();
	}
}

//==============================================================================
//==============================================================================

Archive& Archive::add_post_process		(const std::shared_ptr<ArchiveProcess> &process)
{
	_processes.push_back(process);
	
	return (*this);
}

std::shared_ptr<ArchiveProcess> Archive::pop_post_process (void)
{
	if (_processes.size() <= 0) {
		return std::shared_ptr<ArchiveProcess>(NULL);
	} else {
		std::shared_ptr<ArchiveProcess> process(_processes.front());
		_processes.pop_front();
		
		return process;
	}
}
//==============================================================================
//==============================================================================


} // DT3

