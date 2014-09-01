#ifndef DT3_DIRECTORYLISTER
#define DT3_DIRECTORYLISTER
//==============================================================================
///	
///	File: DirectoryLister.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <vector>
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

class FilePath;

//==============================================================================
/// Class
//==============================================================================

class DirectoryLister: public BaseClass {
    public:
        DEFINE_TYPE(DirectoryLister,BaseClass)
		DEFINE_CREATE_AND_CLONE

									DirectoryLister			(void);	
		explicit					DirectoryLister			(const FilePath &pathname, DTboolean recursive = true);	
									DirectoryLister			(const DirectoryLister &rhs);
									DirectoryLister			(DirectoryLister &&rhs);
        DirectoryLister &			operator =				(const DirectoryLister &rhs);
        DirectoryLister &			operator =				(DirectoryLister &&rhs);
        virtual						~DirectoryLister		(void);

	public:
		/// Scan a directory at a path
		/// \param pathname path to directory
		/// \param recursive scan recursively
		void						scan_path				(const FilePath &pathname, DTboolean recursive = true);

		/// Clear all of the files that have been found
		void						clear_files             (void)					{	_paths.clear();		}
		
		/// Return number of files that have been found
		/// \return number of files
		DTuint						num_files               (void);
		
		/// Retrieve file name
		/// \param index Index to entry
		/// \return name
		std::string                 file_name               (DTuint index);

		/// Retrieve file name
		/// \param index Index to entry
		/// \return name without extension
		std::string                 file_name_no_ext        (DTuint index);

		/// Retrieve file path
		/// \param index Index to entry
		/// \return path to file
		FilePath					file_path               (DTuint index);
				
		/// Returns wehter index is a directory
		/// \param index Index to entry
		/// \return is a directory
		DTboolean					is_directory            (DTuint index) const;

	private:	
		std::vector<FilePath>       _paths;
		
};

//==============================================================================
//==============================================================================


} // DT3

#endif
