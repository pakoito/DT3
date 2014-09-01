#ifndef DT3_ASSETDOWNLOADER
#define DT3_ASSETDOWNLOADER
//==============================================================================
///	
///	File: AssetDownloader.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class URL;
class FilePath;

//==============================================================================
/// Class
//==============================================================================

class AssetDownloader:public BaseClass {
    public:
        DEFINE_TYPE(AssetDownloader, BaseClass)
         
									AssetDownloader         (void);	
	private:
									AssetDownloader         (const AssetDownloader &rhs);
        AssetDownloader &			operator =				(const AssetDownloader &rhs);
        
	public:
        virtual						~AssetDownloader        (void);
		
	public:
    
        enum Status {
            STATUS_IDLE,
            STATUS_CONNECTING,
            STATUS_DOWNLOADING,
            
            STATUS_COMPLETE,
            STATUS_CANCELLED,
            STATUS_ERROR
        };
    
		/// Description
		/// \param param description
		/// \return description
		void                        asset_at_url            (   const URL &url,
                                                                const FilePath &save_path,
                                                                const std::shared_ptr<Callback<Status, DTsize, DTsize> > &status_cb,
                                                                DTfloat timeout = 60.0F);
    
		/// Description
		/// \param param description
		/// \return description
        void                        cancel                  (void);
    
    
		/// Description
		/// \param param description
		/// \return description
        Status                      status                  (void) const  {   return _status; }

		/// Description
		/// \param param description
		/// \return description
        DTsize                      current_size            (void) const  {   return _current_size;   }

		/// Description
		/// \param param description
		/// \return description
        DTsize                      total_size              (void) const  {   return _total_size;   }
    

		/// Description
		/// \param param description
		/// \return description
        DTboolean                   is_done                 (void) const;

		/// Description
		/// \param param description
		/// \return description
        DTboolean                   is_idle                 (void) const;

    private:
        void                        update_status           (Status status, DTsize size, DTsize total_size);
    
        void                        append_data             (std::string &data, BinaryFileStream &temp_file);
        void                        finalize_data           (std::string &data, BinaryFileStream &temp_file);

        void                        downloader_task         (   const URL url,
                                                                const FilePath save_path,
                                                                DTfloat timeout);
    
        std::shared_ptr<Callback<Status, DTsize, DTsize> >  _status_cb;     // Status, size, total size
    
        // Control flags
        DTboolean                                       _cancelled;
    
        // Current state of downloading
        Status                                          _status;
        DTsize                                          _current_size;
        DTsize                                          _total_size;

};

//==============================================================================
//==============================================================================


} // DT3

#endif
