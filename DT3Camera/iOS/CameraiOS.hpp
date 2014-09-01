#ifndef DT2_CAMERAIOS
#define DT2_CAMERAIOS
//==============================================================================
///	
///	File: CameraiOS.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "BaseInclude.hpp"
#include "Callback.hpp"
#include "Color.hpp"
#include "TextureResource.hpp"
#include "Rectangle.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
// Forward Declarations
//==============================================================================

//==============================================================================
//==============================================================================

class CameraiOS {
    public:
        DEFINE_TYPE_SIMPLE_BASE(CameraiOS)

    private:
                                            CameraiOS		(void);
                                            CameraiOS		(const CameraiOS&);
        CameraiOS &                         operator =      (const CameraiOS&);
                                            ~CameraiOS		(void);
        
	public:

		/// Checks if video recording is supported
		/// \result has audio recording
		static DTboolean                    hasVideoInput   (void);

		/// Starts recording video
		static void                         startRecording  (void);

		/// Stops video recording
		static void                         stopRecording   (void);
        
		/// Returns true if recording
        static DTboolean                    isRecording     (void);
		      
        // Callbacks
        static Callback2List<std::shared_ptr<TextureResource>, Rectangle&>&    getFrameCapturedCB      (void)  {  return _frameCapturedCB;     }

	public:
        static std::shared_ptr<TextureResource>                                _texture;
        static Callback2List<std::shared_ptr<TextureResource>, Rectangle&>     _frameCapturedCB;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
