//==============================================================================
///	
///	File: Globals.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::mutex                                      Globals::_globals_lock;
std::map<StringCopier, Globals::GlobalsEntry>   Globals::_globals;

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,Globals::initialize())
GLOBAL_STATIC_DESTRUCTION(0,Globals::destroy())

//==============================================================================
//==============================================================================

void Globals::initialize (void)
{
    load();
}

void Globals::destroy (void)
{
    save();
}

//==============================================================================
//==============================================================================

DTboolean Globals::has_global (const std::string &name_with_case)
{
	std::string name = MoreStrings::lowercase(name_with_case);
    
    std::unique_lock<std::mutex> lock(_globals_lock);
    
    if (_globals.find(name) == _globals.end())
        return false;
	else
		return true;
}

//==============================================================================
//==============================================================================

std::string Globals::global (const std::string &name_with_case)
{
	// Convert name to lowercase
	std::string name = MoreStrings::lowercase(name_with_case);

    std::unique_lock<std::mutex> lock(_globals_lock);

    auto i = _globals.find(name);

    if (i != _globals.end())
        return	_globals[name].value;
    else
        return "";
}

DTboolean Globals::global (const std::string &name_with_case, std::string &value)
{
	// Convert name to lowercase
	std::string name = MoreStrings::lowercase(name_with_case);

    std::unique_lock<std::mutex> lock(_globals_lock);

    auto i = _globals.find(name);

    if (i != _globals.end()) {
        value =	_globals[name].value;
		return true;
    } else {
        value = "";
		return false;
	}
	
}

//==============================================================================
//==============================================================================

void Globals::set_global (const std::string &name_with_case, const std::string &value, const DTint lifetime)
{
	// Convert name to lowercase
	std::string name = MoreStrings::lowercase(name_with_case);

    std::unique_lock<std::mutex> lock(_globals_lock);

    auto i = _globals.find(name);

    if ((i != _globals.end() && i->second.lifetime != READ_ONLY) || (i == _globals.end())) {
        GlobalsEntry entry;

        entry.lifetime = lifetime;
        entry.name = name;
        entry.value = value;

        _globals[name] = entry;
    }

}

//==============================================================================
//==============================================================================

void Globals::set_global_default (const std::string &name_with_case, const std::string &value, const DTint lifetime)
{
	// Convert name to lowercase
	std::string name = MoreStrings::lowercase(name_with_case);

    std::unique_lock<std::mutex> lock(_globals_lock);

    auto i = _globals.find(name);

    if (i == _globals.end()) {
        GlobalsEntry entry;

        entry.lifetime = lifetime;
        entry.name = name;
        entry.value = value;

        _globals[name] = entry;
    }
}

//==============================================================================
//==============================================================================

std::string Globals::substitute_global	(const std::string &s)
{
	std::string value;
    DTboolean success = substitute_global (s, value);
	
	if (success)	return value;
	else			return s;
}

DTboolean Globals::substitute_global (const std::string &s, std::string &value)
{
    value = s;

    for (DTuint i = 0; i < 100; ++i) {
        std::string::size_type first = value.find_last_of('{');
        std::string::size_type last = value.find_first_of('}',first);

        // Check to see if a replacement has to be made
        if (first == std::string::npos || last == std::string::npos || last <= first)
            return true;

        std::string key = MoreStrings::lowercase(value.substr(first + 1, last - first - 1));
        std::string replacement;
        
		// if global not found then return fail
		if (!global(key,replacement))
			return false;
        
        // Do replacement
        value.replace(first, last-first+1, replacement);
	}

	WARNINGMSG("Exceeded maximum iterations. Infinite loop?");
	
	return false;
}

//==============================================================================
//==============================================================================

void Globals::load (void)
{
    std::unique_lock<std::mutex> lock(_globals_lock);
    auto globals = HAL::load_globals();

    // Note: This doesn't overwrite existing elements so we have to manually copy below
    //_globals.insert(globals.begin(), globals.end());

    for(auto& i : globals)
        _globals[i.first] = i.second;

}

void Globals::save (void)
{
    std::unique_lock<std::mutex> lock(_globals_lock);
    HAL::save_globals(_globals);
}

//==============================================================================
//==============================================================================

} // DT3
