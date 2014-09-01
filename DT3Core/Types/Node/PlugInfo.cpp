//==============================================================================
///	
///	File: PlugInfo.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

PlugInfo    PlugInfo::_pool[DT3_PLUG_POOL_SIZE];

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

PlugInfo::PlugInfo (void)
	:	_offset			(0),
		_next			(NULL),
		_is_input		(false),
		_is_output		(false),
		_is_compute		(true),
		_is_no_draw		(false),
        _is_always_dirty(false)
{  

}
					
PlugInfo::~PlugInfo (void)
{

}

//==============================================================================
//==============================================================================

PlugInfo& PlugInfo::init (  std::string name,
                            DTsize offset,
                            DTnodekind *plug_node_info,
                            const DTcharacter* (*type_fn)(void),
                            void (*copy_fn)(PlugBase *dst, const PlugBase *src),
                            std::string (*internal_value_as_string_fn)(const PlugBase *p))
{
	// This assert will trigger if this PlugInfo is added more than once. The easiest way for
	// this to happen is if you have a DEFINE_PLUG_NODE macro without a Initialize_Plugs static 
	// function in your class.
	ASSERT(_next == NULL);
							
	_name = name;
	_offset = offset;

    
    // Save copy function so we done need virtual functions in "Plug"
    _copy_fn = copy_fn;
    _type_fn = type_fn;
    _internal_value_as_string_fn = internal_value_as_string_fn;
	
	// Add to linked list
	_next = plug_node_info->_plugs;
	plug_node_info->_plugs = this;
	
	return *this;
}

//==============================================================================
//==============================================================================

PlugInfo& PlugInfo::affects (DTushort other)
{
    PlugInfo &i = PlugInfo::get_info(other);
    
	_affects.push_back(&i);
	i._affected_by.push_back(this);
	return *this;	
}

PlugInfo& PlugInfo::affected_by (DTushort other)
{
    PlugInfo &i = PlugInfo::get_info(other);

	i._affects.push_back(this);
	_affected_by.push_back(&i);
	return *this;	
}

//==============================================================================	
//==============================================================================	

DTushort PlugInfo::get_free_index (void)
{
    static DTushort pool_index = 0;
    
    DTint r = pool_index++;
    ASSERT(r < DT3_PLUG_POOL_SIZE);
    
    return static_cast<DTushort>(r);
}

PlugInfo& PlugInfo::get_info (DTushort i)
{
    ASSERT(i != 0xFFFF);
    return _pool[i];
}

//==============================================================================
//==============================================================================	

} // DT3

