#ifndef DT3_FILEPATH
#define DT3_FILEPATH
//==============================================================================
///	
///	File: FilePath.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class FilePath {
    public:
        DEFINE_TYPE_SIMPLE_BASE(FilePath)
		//DEFINE_CREATE_AND_CLONE

                                            FilePath				(void);
		explicit                            FilePath				(const std::string &path);
                                            FilePath				(const FilePath &rhs);
                                            FilePath				(FilePath &&rhs);
        FilePath &                          operator =				(const FilePath &rhs);
        FilePath &                          operator =				(FilePath &&rhs);
                                            ~FilePath				(void);

	public:
		/// Sets the path to the file
		/// \param path path to file
		void                                set_path                 (const std::string &path);

		/// Returns the path to the file
		/// \return path to file
		std::string                         full_path               (void) const;

		/// Returns the original path that was set (before any substitutions)
		/// \return original path to file
		std::string                         original_path           (void) const;
				
		/// Returns the path separator
		/// \return path separator
		static std::string                  path_separator          (void);

		/// Returns the package separator
		/// \return package separator
		static std::string                  package_separator       (void);
		
		/// Returns name of file
		/// \return Name of file
		std::string                         file_name               (void) const;

		/// Returns enclosing directory of file
		/// \return Path to directory
        std::string                         directory_path          (void) const;

		/// Returns name of file without extension
		/// \return Name of file without extension
		std::string                         file_name_no_ext        (void) const;

		/// Returns the file extension
		/// \return extension
		std::string                         file_ext                (void) const;

		/// If the file is in a package, returns package name
		/// \return package name
		std::string                         package                 (void) const;

		/// Returns if the file is in a package
		/// \return is in package
		DTboolean                           in_package				(void) const;

		/// Returns the path to the package
		/// \return path to package
		std::string                         package_path            (void) const;

		/// Clears the path
		void                                clear                   (void);
    
		/// Tests for empty path
		/// \return Path is empty
		DTboolean                           is_blank                (void) const		{	return _original_path.empty();	}


		/// Returns modification date for file
		/// \return modification date
		DTuint64                            modification_date       (void) const;

		/// Returns wether path is a directory
		/// \return Is a directory
        DTboolean                           is_dir                  (void) const;
    
		/// Checks if the file exists
		/// \return file exists
		DTboolean                           exists					(void) const;

		/// Moves the file
		/// \param new_path new path to file
		/// \return Success
		DTboolean                           move					(const FilePath &new_path) const;

		/// Deletes the file
		/// \return Success
		DTboolean                           del                     (void) const;

        /// Checksum the file with MD5
		/// \return MD5 hash
        std::string                         checksum                (void) const;

        /// Returns size of file
		/// \return Size of file in bytes
        DTsize                              length                  (void) const;

		/// Recache the original path
		void                                recache_path            (void);
		 
	private:
		friend int operator == (const FilePath& a, const FilePath& b);
		friend int operator != (const FilePath& a, const FilePath& b);

		std::string						_original_path;
		std::string						_path;
		
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator << (Stream &s, const FilePath &v);
Stream& operator >> (Stream &s, FilePath &v);

//==============================================================================
//==============================================================================

inline int operator == (const FilePath& a, const FilePath& b)
{
	return (a._path == b._path);
}

inline int operator != (const FilePath& a, const FilePath& b)
{
	return (a._path != b._path);
}

//==============================================================================
//==============================================================================


} // DT3

#endif
