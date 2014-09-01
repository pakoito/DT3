#ifndef DT3_PACKAGE
#define DT3_PACKAGE
//==============================================================================
///	
///	File: Package.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include <string>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class Package: public BaseClass {
    public:
        DEFINE_TYPE(Package,BaseClass)
		DEFINE_CREATE
         
                                        Package				(void);	
    private:
                                        Package				(const Package &rhs);
        Package &                       operator =			(const Package &rhs);
        
    public:
        virtual                         ~Package			(void);

	public:
		/// Loads a package and indexes it
		/// \param pathname path to package
		/// \return Error
		DTerr                           load_package        (const FilePath &pathname);

		/// Retrieve information about a file from the package
		/// \param pathname path to file
		/// \param start start of file
		/// \param length length of file
		/// \param uncompressed_length uncompressed length of file
		/// \return description
        DTerr                           start_and_length    (const FilePath &pathname, DTsize &start, DTsize &length, DTsize &uncompressed_length) const;

        /// Get Package Start
//        DTsize                          offset              (void) const    {   return _offset;  }

	private:
	
		struct Entry {
			std::string                 _name;
			DTsize                      _start;
			DTsize                      _length;
            DTsize                      _uncompressed_length;
		};
	
        DTsize                          _offset;

		std::map<std::string, Entry>    _entries;
};

//==============================================================================
//==============================================================================


} // DT3

#endif
