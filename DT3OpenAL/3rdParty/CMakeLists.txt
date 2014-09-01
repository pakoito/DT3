#==============================================================================
#	
#	File: CMakeLists.txt	(for 3rdParty)
#
#	Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
#
#	This file is subject to the terms and conditions defined in
#	file 'LICENSE.txt', which is part of this source code package.
#	
#==============================================================================

#
# OpenAL
#

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DAL_ALEXT_PROTOTYPES)

set(PROJECT_SRCS 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alAuxEffectSlot.c 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alBuffer.c        
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alEffect.c        
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alError.c         
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alExtension.c     
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alFilter.c        
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alListener.c      
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alSource.c        
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alState.c         
    ${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/alThunk.c         
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/ALc.c                  
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/ALu.c                  
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcConfig.c            
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcDedicated.c            
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcEcho.c              
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcModulator.c         
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcReverb.c            
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcRing.c              
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/alcThread.c            
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/bs2b.c                 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/helpers.c                 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/hrtf.c                 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/mixer.c                 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/panning.c                 
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/backends/loopback.c              
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/backends/null.c              
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/backends/opensl.c              
    ${PROJECT_SOURCE_DIR}/openal-soft-android/Alc/backends/android.c              
)

set(PROJECT_INCLUDES 
	${PROJECT_SOURCE_DIR}/openal-soft-android/OpenAL32/Include
	${PROJECT_SOURCE_DIR}/openal-soft-android/include
)

include_directories("${PROJECT_INCLUDES}")
add_library(openal STATIC ${PROJECT_SRCS})


