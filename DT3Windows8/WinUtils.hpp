#ifndef DT2_WINUTILS
#define DT2_WINUTILS
//==============================================================================
///	
///	File: WinUtils.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#define SAFE_RELEASE(p) {   \
    if (p)                  \
        (p)->Release();     \
    (p) = NULL;             \
}

//==============================================================================
//==============================================================================
#endif
