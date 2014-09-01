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
# Freetype
#

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/autofit/autofit.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftbase.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftinit.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftsystem.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftglyph.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftbitmap.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/base/ftstroke.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/bdf/bdf.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/cache/ftcache.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/cff/cff.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/cid/type1cid.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/gzip/ftgzip.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/lzw/ftlzw.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/otvalid/otvalid.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/pcf/pcf.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/pfr/pfr.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/psaux/psaux.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/pshinter/pshinter.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/psnames/psnames.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/raster/ftraster.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/raster/ftrend1.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/sfnt/sfnt.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/smooth/ftsmooth.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/smooth/ftgrays.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/truetype/truetype.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/type1/type1.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/type42/type42.c
	${PROJECT_SOURCE_DIR}/freetype-2.4.7/src/winfonts/winfnt.c
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/include/freetype
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/include
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/autofit
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/base
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/bdf
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/cache
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/cff
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/cid
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/gzip
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/lzw
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/otvalid
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/pcf
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/pfr
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/psaux
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/pshinter
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/psnames
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/raster
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/sfnt
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/smooth
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/truetype
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/type1
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/type42
	${PROJECT_INCLUDE_DIR}/freetype-2.4.7/src/winfonts
)

include_directories("${PROJECT_INCLUDES}")
add_library(freetype STATIC ${PROJECT_SRCS})

#
# zlib
#

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/inftrees.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/infback.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/inffast.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/adler32.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/gzlib.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/gzread.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/gzwrite.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/crc32.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/inflate.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/compress.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/deflate.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/uncompr.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/trees.c
	${PROJECT_SOURCE_DIR}/zlib-1.2.5/zutil.c
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/zlib-1.2.5
)

include_directories("${PROJECT_INCLUDES}")
add_library(zlib STATIC ${PROJECT_SRCS})

#
# lpng
#

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/lpng151/png.c
	${PROJECT_SOURCE_DIR}/lpng151/pngerror.c
	${PROJECT_SOURCE_DIR}/lpng151/pngget.c
	${PROJECT_SOURCE_DIR}/lpng151/pngmem.c
	${PROJECT_SOURCE_DIR}/lpng151/pngread.c
	${PROJECT_SOURCE_DIR}/lpng151/pngpread.c
	${PROJECT_SOURCE_DIR}/lpng151/pngrio.c
	${PROJECT_SOURCE_DIR}/lpng151/pngrtran.c
	${PROJECT_SOURCE_DIR}/lpng151/pngrutil.c
	${PROJECT_SOURCE_DIR}/lpng151/pngset.c
	${PROJECT_SOURCE_DIR}/lpng151/pngtrans.c
	${PROJECT_SOURCE_DIR}/lpng151/pngwio.c
	${PROJECT_SOURCE_DIR}/lpng151/pngwrite.c
	${PROJECT_SOURCE_DIR}/lpng151/pngwtran.c
	${PROJECT_SOURCE_DIR}/lpng151/pngwutil.c
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/lpng151
)

include_directories("${PROJECT_INCLUDES}")
add_library(lpng STATIC ${PROJECT_SRCS})

#
# md5
#

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/md5-cc/md5.cc
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/md5-cc
)

include_directories("${PROJECT_INCLUDES}")
add_library(md5 STATIC ${PROJECT_SRCS})

#
# jenkins
#

set(PROJECT_SRCS 
	${PROJECT_SOURCE_DIR}/JenkinsHash/lookup3.c
)

set(PROJECT_INCLUDES 
	${PROJECT_INCLUDE_DIR}/JenkinsHash
)

include_directories("${PROJECT_INCLUDES}")
add_library(jenkins STATIC ${PROJECT_SRCS})

