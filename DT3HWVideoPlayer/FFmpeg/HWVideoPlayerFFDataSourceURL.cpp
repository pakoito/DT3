//==============================================================================
///	
///	File: 			HWVideoPlayerFFDataSourceURL.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceURL.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFDataSourceURL::HWVideoPlayerFFDataSourceURL (const URL &url)
    :   _url    (url)
{
    
}

HWVideoPlayerFFDataSourceURL::~HWVideoPlayerFFDataSourceURL (void)
{
    close();
}

//==============================================================================
//==============================================================================

DTerr HWVideoPlayerFFDataSourceURL::open   (void)
{
    return DT3_ERR_NONE;
}

void HWVideoPlayerFFDataSourceURL::close   (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

