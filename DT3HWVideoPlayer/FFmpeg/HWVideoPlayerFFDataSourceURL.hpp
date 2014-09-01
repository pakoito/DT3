#ifndef DT3_HWVIDEOPLAYERFFDATASOURCEURL
#define DT3_HWVIDEOPLAYERFFDATASOURCEURL
//==============================================================================
///	
///	URL: 			HWVideoPlayerFFDataSourceURL.hpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceBase.hpp"

#include "URL.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class HWVideoPlayerFFDataSourceURL: public HWVideoPlayerFFDataSourceBase {
    public:
        DEFINE_CREATE

                                        HWVideoPlayerFFDataSourceURL    (const URL &url);
    private:
                                        HWVideoPlayerFFDataSourceURL    (const HWVideoPlayerFFDataSourceURL &rhs);
        HWVideoPlayerFFDataSourceURL&   operator =                      (const HWVideoPlayerFFDataSourceURL &rhs);
    public:
        virtual                         ~HWVideoPlayerFFDataSourceURL	(void);
    
	public:
    
		/// Open a video data source
        /// \return Error code
        virtual DTerr                   open        (void);
    
		/// Closes a video source
        virtual void                    close       (void);
    
    private:
        URL                             _url;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
