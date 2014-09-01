#ifndef DT3_FILEMANAGER
#define DT3_FILEMANAGER
//==============================================================================
///	
///	File: FileManager.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include <string>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================

class TextFileStream;
class BinaryFileStream;
class FileHandle;
class FilePath;
class Package;
class Progress;

//==============================================================================
//==============================================================================

class FileManager {
		/// File Manager for managing packages and file opening.

	private:
									FileManager         (void);	
									FileManager         (const FileManager &rhs);
        FileManager &               operator =          (const FileManager &rhs);	
        virtual						~FileManager		(void);
    
	public:

		/// Initialize the file manager system
        static void                 initialize          (void);

		/// Destroy the file manager system
        static void                 destroy             (void);

		/// Scans and build a list of all of the files in the data directory
		static void                 scan_files			(void);

		/// Registers and scans a package file
		/// \param pathname Path to the package
		/// \return Error code
		static DTerr                register_package    (const FilePath &packagename);

		/// Registers and scans a package file
		/// \param pathname Path to the package
		/// \return Error code
		static void                 unregister_packages (void);
    
		/// Looks up a file in a package and returns its start position and length
		/// \param pathname Path to the file
		/// \param packagename Path to the package
		/// \param start Start position of file in package
		/// \param length Length of file in package
		/// \return Error code
        static DTerr                start_and_length    (const FilePath &pathname, FilePath &packagename, DTsize &start, DTsize &length, DTsize &uncompressed_length);
		
		/// Opens a text file in the package
		/// \param file Resulting file stream
		/// \param pathname Path to file
		/// \param read Reading or writing
		/// \param progress Progress object to implement progress bars
		/// \return Error code
		static DTerr				open				(TextFileStream &file, const FilePath &pathname, DTboolean read = true, std::shared_ptr<Progress> progress = NULL);

		/// Opens a binary file in the package
		/// \param file Resulting file stream
		/// \param pathname Path to file
		/// \param read Reading or writing
		/// \param progress Progress object to implement progress bars
		/// \return Error code
		static DTerr				open				(BinaryFileStream &file, const FilePath &pathname, DTboolean read = true, std::shared_ptr<Progress> progress = NULL);
    
    
		/// Sets a callback to be called when the file manager is trying to Open a FileHandle
        /// It is used to intercept the opens and redirect them to anywhere else such
        /// as an asset in Android.
		/// \param cb File Open Callback
		/// \param data User data passed to callback
        static void                 set_file_open_cb    (std::shared_ptr<Callback<const FilePath &, std::shared_ptr<FileHandle> &, DTsize &, DTsize &, void *>> cb, void *data);
    
	private:
        static DTerr				open_from_fragment  (TextFileStream &file, const FilePath &pathname, DTsize start, DTsize length, DTsize uncompressed_length, DTboolean read, std::shared_ptr<Progress> progress = NULL);
        static DTerr				open_from_fragment  (BinaryFileStream &file, const FilePath &pathname, DTsize start, DTsize length, DTsize uncompressed_length, DTboolean read, std::shared_ptr<Progress> progress = NULL);
    
		static std::map<std::string, std::shared_ptr<Package>>                                         _packages;
        static std::shared_ptr<Callback<const FilePath &, std::shared_ptr<FileHandle> &, DTsize &, DTsize &, void *>> _cb;
        static void                                                                                     *_data;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
