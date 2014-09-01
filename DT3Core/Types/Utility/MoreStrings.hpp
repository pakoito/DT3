#ifndef DT3_MORESTRINGS
#define DT3_MORESTRINGS
//==============================================================================
///	
///	File: MoreStrings.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"

#include <string>
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class MoreStrings {
	private:
								MoreStrings                     (void);
								MoreStrings                     (const MoreStrings &rhs);
		MoreStrings &           operator =                      (const MoreStrings &rhs);
								~MoreStrings                    (void);
	
	public:

		/// Convert data to hex strings
		/// \param data pointer to data
		/// \param length length of data
		/// \return Hex string
		static std::string      to_hex_string                   (const void* data, DTsize length);
    
        /// Convert hex string to data
		/// \param s hex string
		/// \param data pointer to data
		/// \param length length of data
		static void				from_hex_string					(const std::string &s, void* data, DTsize length);

		/// Simple Obfuscator
		/// \param key key used to obfuscate data
		/// \param s data
		/// \return Obfuscated data
		static std::string      to_obfuscated					(const std::string &key, const std::string &s);

		/// Simple Unobfuscator
		/// \param key key used to obfuscate data
		/// \param s data
		/// \return Unobfuscated data
		static std::string      from_obfuscated					(const std::string &key, const std::string &s);

		/// Simple Unobfuscator for raw data
		/// \param data data to obfuscate data
		/// \param size size sof data
		/// \param salt salt obfuscate data
		/// \param offset offset for obfuscating data
		/// \return Unobfuscated data
        static void             obfuscate_raw                   (DTubyte *data, DTsize size, DTuint salt, DTsize offset);

		/// Convert string to lowercase
		/// \param s string
		/// \return lowercase string
        static std::string      lowercase                       (const std::string &s);

		/// Convert string to uppercase
		/// \param s string
		/// \return uppercase string
        static std::string      uppercase                       (const std::string &s);
    
    
        /// Find and replace strings
        /// \param s string
        /// \param f find string
        /// \param r replace string
		/// \return string
        static std::string      find_and_replace                (const std::string &s, const std::string &f, const std::string &r);
    
        /// Filter characters out of a string
        /// \param s string
        /// \param f filter characters
		/// \return string
        static std::string      filter_out                      (const std::string &s, const std::string &f);

        /// Allow certain characters in the string
        /// \param s string
        /// \param f filter characters
		/// \return string
        static std::string      filter_in                       (const std::string &s, const std::string &f);


        /// Hash a string
        /// \param s string
		/// \return hash of string
        static DTuint           hash                            (const std::string &s);


        /// Clean the control characters in a string
        /// \param s string
		/// \return cleaned string
        static std::string      clean_ctrl_characters           (const std::string &s);


        /// Split the string based on a string of delimeters
        /// \param s string to split
        /// \param delims delimeters to use to split
        /// \return Array of tokens
        static std::vector<std::string>     split               (const std::string &s, const std::string &delims);


        /// Escape a string
        /// \param s string to escape
        /// \return escaped string
        static std::string      escaped                         (const std::string &s);

        /// Unescape a string
        /// \param s string to unescape
        /// \return unescaped string
        static std::string      unescaped                       (const std::string &s);
    
        /// Remove end digits from a string
        /// \param s string to trim
        /// \return trimmed string
        static std::string      trim_end_digits                 (const std::string &s);
    
        /// Return end digits from a string
        /// \param s string to get digits from
        /// \return trimmed string
        static std::string      end_digits                      (const std::string &s);
    
        /// Extract a unicode character from a buffer
        /// \param start start of buffer
        /// \param num_bytes number of bytes consumed
        /// \param character unicode character
        static void             extract_unicode                 (DTcharacter *start, DTuint &num_bytes, DTuint &character);

        /// Trim left (i.e. front) of string
        /// \param s string to trim
        /// \return trimmed string
        static std::string      left_trim                       (const std::string &s);

        /// Trim right (i.e. end) of string
        /// \param s string to trim
        /// \return trimmed string
        static std::string      right_trim                      (const std::string &s);

        /// Trim both ends of string
        /// \param s string to trim
        /// \return trimmed string
        static std::string      trim                            (const std::string &s);

        /// Converts underscores to spaces and captializes any character after a space
        /// \param s string to format
        /// \return formatted string
        static std::string      captialize_and_format           (const std::string &s);

        /// Converts string to camelCaps
        /// \param s string to format
        /// \return formatted string
        static std::string      camel_caps                      (const std::string &s);

        /// Cast a value to a string
        /// \param t value to cast
        /// \return string
        template <class T>
        inline static std::string cast_to_string (T t) {
            TextBufferStream ss;
            ss << t;
            return ss.buffer();
        }

        /// Cast a value from a string
        /// \param s string
        /// \return value
        template <class T>
        inline static T cast_from_string (const std::string &s) {
            T temp = TypeTraits<T>::default_value();
            TextBufferStream ss(s);
            ss >> temp;
            return temp;
        }
    
    
        // Case insensitive string compare
        // \param a first string
        // \param b second string
        /// \return is equal
        static DTboolean        iequals (const std::string &a, const std::string &b);

        // Case insensitive string compare
        // \param a first string
        // \param b second string
        /// \return is equal
        static DTboolean        iequals (const std::string &a, const char *b);

        // Case insensitive string compare
        // \param a first string
        // \param b second string
        /// \return is equal
        static DTboolean        iequals (const char *a, const std::string &b);


};

//==============================================================================
// String cast specializations
//==============================================================================

template <>
inline std::string MoreStrings::cast_to_string<std::string> (std::string t) {
    return t;
}

template <>
inline std::string MoreStrings::cast_from_string<std::string> (const std::string &s) {
    return s;
}

//==============================================================================
//==============================================================================

class StringCopier {
    public:
        StringCopier (void)
        {}

        StringCopier (const StringCopier &s)
            :   _s  (s._s.data(), s._s.length())
        {}
    
        StringCopier (const std::string &s)
            :   _s  (s.data(), s.length())
        {}
    
        operator std::string()
        {
            return std::string(_s.data(), _s.length());
        }
    
        ~StringCopier (void)
        {}
    
    
        DTboolean operator < (const StringCopier &s) const {
            return _s < s._s;
        }
    
    private:
        std::string     _s;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
