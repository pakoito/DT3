#ifndef DT3_HWVIDEOPLAYERFFDATASOURCEFILE
#define DT3_HWVIDEOPLAYERFFDATASOURCEFILE
//==============================================================================
///	
///	File: 			HWVideoPlayerFFDataSourceFile.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceBase.hpp"

#include "FilePath.hpp"
#include "BinaryFileStream.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class HWVideoPlayerFFDataSourceFile: public HWVideoPlayerFFDataSourceBase {
    public:
        DEFINE_CREATE

                                        HWVideoPlayerFFDataSourceFile   (const FilePath &path);
    private:
                                        HWVideoPlayerFFDataSourceFile   (const HWVideoPlayerFFDataSourceFile &rhs);
        HWVideoPlayerFFDataSourceFile&  operator =                      (const HWVideoPlayerFFDataSourceFile &rhs);
    public:
        virtual                         ~HWVideoPlayerFFDataSourceFile	(void);
    
	public:
    
		/// Open a video data source
        /// \return Error code
        virtual DTerr                   open        (void);
    
		/// Closes a video source
        virtual void                    close       (void);
    
    private:
        // This function must read “buf_size” number of bytes from your
        // file handle (which is the “opaque” parameter) and store the data
        // into “buf”. The return value is the number of bytes actually read /
        // from your file handle. If the function fails you must return <0.
        static int      readFunc    (void *opaque, uint8_t *buf, int buf_size);
    
        // This function is like the fseek() C stdio function. “whence” can
        // be either one of the standard C values (SEEK_SET, SEEK_CUR, SEEK_END)
        // or one more value: AVSEEK_SIZE.
        static int64_t  seekFunc    (void *opaque, int64_t offset, int whence);


        FilePath                _path;
        BinaryFileStream        _file;
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif
