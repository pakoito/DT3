#ifndef DT3_URL
#define DT3_URL
//==============================================================================
///	
///	File: URL.hpp
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

class URL {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(URL)
    
								URL					(void);	
		explicit				URL					(const std::string &url) {   set_full_url(url);    }
								URL                 (const URL &rhs);
								URL                 (URL &&rhs);
        URL &					operator =          (const URL &rhs);
        URL &					operator =          (URL &&rhs);
								~URL                (void);

	public:
		/// Set the full URL
		/// \param url full URL
		void					set_full_url        (const std::string &url);

		/// Gets the full URL
		/// \return full URL
		const std::string&      full_url            (void) const        {   return _url;		}

		/// Returns hostname
		/// \return hostname
		std::string             protocol            (void) const;

		/// Returns hostname
		/// \return hostname
		std::string             hostname            (void) const;

		/// Returns Path
		/// \return path
		std::string             path                (void) const;

		/// Returns URL without protocol
		/// \return URL without protocol
		std::string             strip_protocol		(void) const;
		
		/// Returns URL without protocol or parameters
		/// \return URL without protocol or parameters
		std::string             strip_protocol_and_parameters		(void) const;
		
		/// Specified port
		/// \return Returns port if specified or 0 if none
		DTushort				port                (void) const;

		/// Returns parameters from URL
		/// \return Parameters
		std::string             parameters          (void) const;

        /// Encode a URL using URL encoding standards
		/// \return Encoded URL
        static std::string      encode_URL          (const std::string &s);

        /// Decode a URL using URL encoding standards
		/// \return Decoded URL
        static std::string      decode_URL          (const std::string &s);


        /// Checks to see if it is a valid URL
		/// \return valid URL
        static DTboolean        is_URL              (const std::string &s);

	private:
		std::string             _url;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator << (Stream &s, const URL &v);
Stream& operator >> (Stream &s, URL &v);

//==============================================================================
//==============================================================================

} // DT3

#endif
