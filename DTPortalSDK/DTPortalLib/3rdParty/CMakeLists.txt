#==============================================================================
#	
#	File: CMakeLists.txt	(for DTPortal)
#
#	Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
#
#	This file is subject to the terms and conditions defined in
#	file 'LICENSE.txt', which is part of this source code package.
#	
#==============================================================================



#
# jansson-2.5
#

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE PROJECT_SRCS 
	"${PROJECT_SOURCE_DIR}/jansson-2.5/src/*.c"
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/jansson-2.5/src
)

include_directories("${PROJECT_INCLUDES}")
add_library(jansson STATIC ${PROJECT_SRCS})

#
# libtomcrypt
#

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/ciphers/aes/aes.c
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/hashes/sha2/sha256.c
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/misc/crypt/crypt_argchk.c
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/misc/base64/base64_decode.c
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/misc/base64/base64_encode.c
	${PROJECT_SOURCE_DIR}/libtomcrypt-1.17/src/misc/zeromem.c
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/libtomcrypt-1.17/src/headers
)

include_directories(SYSTEM "${PROJECT_INCLUDES}")
add_library(libtomcrypt STATIC ${PROJECT_SRCS})


