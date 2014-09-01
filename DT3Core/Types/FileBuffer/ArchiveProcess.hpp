#ifndef DT3_ARCHIVEPROCESS
#define DT3_ARCHIVEPROCESS
//==============================================================================
///	
///	File: ArchiveProcess.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Archive;

//==============================================================================
/// Class
//==============================================================================

class ArchiveProcess: public BaseClass {
    public:
        DEFINE_TYPE(ArchiveProcess,BaseClass)
         
							ArchiveProcess	(void);	
    
							ArchiveProcess	(const ArchiveProcess &rhs);
        ArchiveProcess &	operator =		(const ArchiveProcess &rhs);	
    
		virtual				~ArchiveProcess	(void);
								
	public:
		/// Post process for archiving
		/// \param archive Archive that owns the post process
		/// \param archive Objects Map of objects that have been archived
		virtual void		process			(const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects) = 0;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
