//==============================================================================
///	
///	File: StaticInitializer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::list<StaticInitializerCallback*>&	initializer_callbacks_list	(void)
{
	static std::list<StaticInitializerCallback*>	callbacks;
	return callbacks;
}

//==============================================================================
//==============================================================================

void	StaticInitializer::register_initializer (StaticInitializerCallback *initializer)
{
	std::list<StaticInitializerCallback*>& callbacks = initializer_callbacks_list();
	callbacks.push_back(initializer);
}

void	StaticInitializer::initialize (void)
{
	std::list<StaticInitializerCallback*> callbacks = initializer_callbacks_list();
	
	// Keep iterating through list until all initializers are called
	DTuint current_priority = 0;
	
	while (callbacks.size() > 0) {
    
		DTuint pending_priority = DTUINT_MAX;

		for (auto iter = callbacks.begin(); iter != callbacks.end();) {
			StaticInitializerCallback *cb = *iter;
			
			// initialize if priority matches
			if (cb->priority() == current_priority) {
				cb->initialize();
				iter = callbacks.erase(iter);
				continue;	// Skip the "++iter" below
			}
			
			// update pending priority
			if (cb->priority() > current_priority && cb->priority() < pending_priority) {
				pending_priority = cb->priority();	// We'll get you next round
			}
			
			++iter;
		}
		
		current_priority = pending_priority;
	}
	
}

void	StaticInitializer::destroy (void)
{
	std::list<StaticInitializerCallback*> callbacks = initializer_callbacks_list();
	
	// Keep iterating through list until all initializers are called
	DTuint current_priority = 0;
	
	while (callbacks.size() > 0) {
    
		DTuint pending_priority = DTUINT_MAX;

		for (auto iter = callbacks.begin(); iter != callbacks.end();) {
			StaticInitializerCallback *cb = *iter;
			
			// initialize if priority matches
			if (cb->priority() == current_priority) {
				cb->destroy();
				iter = callbacks.erase(iter);
				continue;	// Skip the "++iter" below
			}
			
			// update pending priority
			if (cb->priority() > current_priority && cb->priority() < pending_priority) {
				pending_priority = cb->priority();	// We'll get you next round
			}
			
			++iter;
		}
		
		current_priority = pending_priority;
	}
	
}

//==============================================================================
//==============================================================================

} // DT3

