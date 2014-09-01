#ifndef DT2_CAMERAMACOSX
#define DT2_CAMERAMACOSX
//==============================================================================
///	
///	File: CameraMacOSX.hpp
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

class CameraMacOSX {
    public:
        DEFINE_TYPE_SIMPLE_BASE(CameraMacOSX)

    private:
                                            CameraMacOSX    (void);
                                            CameraMacOSX    (const CameraMacOSX&);
        CameraMacOSX &                      operator =      (const CameraMacOSX&);
                                            ~CameraMacOSX   (void);
        
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

	private:
        static Callback2List<std::shared_ptr<TextureResource>, Rectangle&>      _frameCapturedCB;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
