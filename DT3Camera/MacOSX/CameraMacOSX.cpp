//==============================================================================
///	
///	File: 			CameraMacOSX.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	11/27/2009
///	Changes:		-none-
///	
//==============================================================================

#include 	"CameraMacOSX.hpp"

#include	"Factory.hpp"
#include	"System.hpp"
#include	"ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

Callback2List<std::shared_ptr<TextureResource>, Rectangle&>      CameraMacOSX::_frameCapturedCB;

//==============================================================================
//==============================================================================

DTboolean CameraMacOSX::hasVideoInput (void)
{
    return false;
}

void CameraMacOSX::startRecording (void)
{

}

void CameraMacOSX::stopRecording (void)
{

}

DTboolean CameraMacOSX::isRecording (void)
{
    return false;
}
      
//==============================================================================
//==============================================================================

} // DT2
