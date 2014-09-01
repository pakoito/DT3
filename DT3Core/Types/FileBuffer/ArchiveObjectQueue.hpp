#ifndef DT3_ARCHIVEOBJECTQUEUE
#define DT3_ARCHIVEOBJECTQUEUE
//==============================================================================
///	
///	File: ArchiveObjectQueue.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ArchiveObjectQueue {
    private:
         
                                            ArchiveObjectQueue      (void);
    
                                            ArchiveObjectQueue      (const ArchiveObjectQueue &rhs);
        ArchiveObjectQueue &                operator =              (const ArchiveObjectQueue &rhs);
    
		virtual                             ~ArchiveObjectQueue     (void);
								
	public:		
    
		/// Loads the entire tree from an archive and returns the first object
		/// \param archive archive to read from
		/// \param cb callback that is called for every created object
		/// \return first object
        static std::shared_ptr<BaseClass>   queue_in_tree           (   const std::shared_ptr<Archive> &archive,
                                                                        std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> obj_loaded_cb = NULL);

		/// Saves the entire tree to an archive
		/// \param archive archive to save to
		/// \param ptr base object to save
        static void                         queue_out_tree          (const std::shared_ptr<Archive> &archive, BaseClass *ptr);
        static void                         queue_out_tree          (const std::shared_ptr<Archive> &archive, const std::shared_ptr<BaseClass> &ptr);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
