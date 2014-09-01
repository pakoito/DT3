//==============================================================================
///	
///	File: MoreStrings.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::string MoreStrings::to_hex_string (const void* data, DTsize length)
{
	std::string as_hex;
	as_hex.reserve(length * 2);
	
	const DTubyte *data_bytes = reinterpret_cast<const DTubyte*>(data);
	
	for (DTuint i = 0; i < length; ++i) {
		
		DTubyte nibbles[2] = {	(DTubyte)((data_bytes[i] >> 4) & 0xF),
								(DTubyte)((data_bytes[i]) & 0xF) };
		
		if (nibbles[0] <= 9)	as_hex += (DTcharacter) ('0' + nibbles[0]);
		else					as_hex += (DTcharacter) ('A' + nibbles[0] - 10);

		if (nibbles[1] <= 9)	as_hex += (DTcharacter) ('0' + nibbles[1]);
		else					as_hex += (DTcharacter) ('A' + nibbles[1] - 10);

	}
	
	return as_hex;
}

void MoreStrings::from_hex_string (const std::string &s, void* data, DTsize length)
{
    std::string ss = find_and_replace(s,"0x","");

	DTubyte *data_bytes = reinterpret_cast<DTubyte*>(data);
	
	ASSERT(length >= static_cast<DTint>(ss.size()) / 2);

	for (DTuint i = 0; i < ss.size()/2; ++i) {
		DTcharacter nibbles[2] = { ss[i*2+0], ss[i*2+1] };
		DTubyte byte;
		
		if (nibbles[0] >= '0' && nibbles[0] <= '9')		byte = static_cast<DTubyte>((nibbles[0] - '0') << 4);
		else											byte = static_cast<DTubyte>((nibbles[0] - 'A' + 10) << 4);

		if (nibbles[1] >= '0' && nibbles[1] <= '9')		byte |= static_cast<DTubyte>((nibbles[1] - '0'));
		else											byte |= static_cast<DTubyte>((nibbles[1] - 'A' + 10));
		
		data_bytes[i] = byte;
	}
}
	
//==============================================================================
//==============================================================================

void MoreStrings::obfuscate_raw (DTubyte *data, DTsize size, DTuint salt, DTsize offset)
{
	// This is just a stupid XOR of bytes. It won't fool anyone that takes
	// any time to try and break it.
	
	static DTubyte xor_table[] = {	0xf3, 0x5b, 0x47, 0x86, 0xb0, 0xd5, 0x99, 0x4e, 0xce, 0xe1, 0x3c, 0xe9, 0x06, 0x71, 0x9f, 0xcf, 
									0x0b, 0xe6, 0x31, 0x9b, 0x81, 0x5f, 0x3b, 0xdd, 0x14, 0xac, 0x9b, 0xd7, 0x19, 0xd2, 0xfa, 0xf3, 
									0xe3, 0x31, 0xca, 0xc8, 0x93, 0x9d, 0xa3, 0x03, 0x0c, 0x93, 0x7f, 0xaf, 0x4b, 0x50, 0xac, 0x80, 
									0x8f, 0x81, 0x44, 0xb1, 0x8a, 0x43, 0xc5, 0x79, 0x72, 0xa6, 0x85, 0x94, 0x52, 0x97, 0xf4, 0xa5, 
									0x76, 0xfc, 0x04, 0x65, 0xee, 0x84, 0x17, 0xa0, 0xab, 0x84, 0xe7, 0x00, 0xa6, 0xba, 0xb7, 0x38, 
									0xc5, 0x67, 0xdf, 0x92, 0x22, 0x1b, 0xe1, 0x60, 0xce, 0x24, 0x32, 0x9c, 0x8c, 0xa8, 0xcb, 0x14, 
									0x08, 0x3e, 0x73, 0x52, 0x37, 0x41, 0x5c, 0xd9, 0xb5, 0x90, 0xa1, 0x1e, 0xde, 0x30, 0x52, 0xcd, 
									0x46, 0x46, 0xe8, 0xef, 0x0e, 0x29, 0x69, 0xd6, 0x93, 0x45, 0xab, 0xf5, 0xe3, 0x13, 0x6b, 0x41	};
	
	// Old method
	if (salt == 0xFFFFFFFF) {
		for (DTint i = 0; i < size; ++i) {
			data[i] ^= xor_table[(i + offset) % sizeof(xor_table)];
		}
		
	// New method, if turned on
	} else if (salt != 0) {
	
		for (DTsize i = 0; i < size; ++i) {
			DTubyte s = (salt >> ((i + offset) % 24)) & 0xFF;
			data[i] ^= xor_table[(i + offset) % sizeof(xor_table)] ^ s;
		}	
	}
}

//==============================================================================
//==============================================================================

std::string MoreStrings::to_obfuscated (const std::string &key, const std::string &s)
{
	std::vector<DTubyte>	buffer;
	buffer.reserve( (size_t) (s.size() + 4) );
	
	DTuint salt = MoreMath::calc_crc32(key.c_str(), key.size());
	DTuint crc = MoreMath::calc_crc32(s.c_str(), s.size());
	buffer.push_back( (DTcharacter) ((crc >> 24) & 0xFF));
	buffer.push_back( (DTcharacter) ((crc >> 16) & 0xFF));
	buffer.push_back( (DTcharacter) ((crc >> 8) & 0xFF));
	buffer.push_back( (DTcharacter) ((crc >> 0) & 0xFF));
	
    buffer.insert(buffer.end(), s.begin(), s.end());
		
	obfuscate_raw(&buffer[0], buffer.size(), salt, 0);
	
	return to_hex_string(&buffer[0], buffer.size());
}

std::string MoreStrings::from_obfuscated (const std::string &key, const std::string &s)
{
	if (s.size() < 8)	// There has to be at least 8 characters for a checksum
		return "";

	DTuint salt = MoreMath::calc_crc32(key.c_str(), key.size());

	std::vector<DTubyte>	buffer;
	buffer.resize( (size_t) (s.size() / 2) );
	
	from_hex_string	(s, &buffer[0], buffer.size());
	obfuscate_raw(&buffer[0], buffer.size(), salt, 0);
	
	std::string s_out;
	s_out.reserve(buffer.size() - 4);
	
	for (DTuint i = 4; i < buffer.size(); ++i)
		s_out += buffer[i];

	DTuint crc = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3] << 0;
	DTuint crc_out = MoreMath::calc_crc32(s_out.c_str(), s_out.size());
		
	if (crc == crc_out)
		return s_out;
	else 
		return "";
}

//==============================================================================
//==============================================================================

std::string MoreStrings::lowercase (const std::string &s)
{
    std::string ss(s);
    std::transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
    return ss;
}

std::string MoreStrings::uppercase (const std::string &s)
{
    std::string ss(s);
    std::transform(ss.begin(), ss.end(), ss.begin(), ::toupper);
    return ss;
}

//==============================================================================
//==============================================================================

std::string MoreStrings::find_and_replace (const std::string &s, const std::string &f, const std::string &r)
{
    std::string ss(s);
    size_t pos = 0;
    
    while((pos = ss.find(f, pos)) != std::string::npos) {
        ss.replace(pos, f.length(), r);
        pos += r.length();
    }
    
    return ss;
}

//==============================================================================
//==============================================================================

std::string MoreStrings::filter_out (const std::string &s, const std::string &f)
{
    std::string ss(s);
    
    auto e = std::remove_if(ss.begin(), ss.end(), [&f](const char c) { return f.find_first_of(c) != std::string::npos; } );
    ss.erase(e, ss.end());
    
    return ss;
}

std::string MoreStrings::filter_in (const std::string &s, const std::string &f)
{
    std::string ss(s);
    
    auto e = std::remove_if(ss.begin(), ss.end(), [&f](const char c) { return f.find_first_of(c) == std::string::npos; } );
    ss.erase(e, ss.end());
    
    return ss;
}

//==============================================================================
//==============================================================================

DTuint MoreStrings::hash (const std::string &s)
{
    DTuint  h = 5381;

    FOR_EACH(i, s)
        h = ((h << 5) + h) + (*i); /* h * 33 + c */

    return h;
}

//==============================================================================
//==============================================================================

std::string MoreStrings::clean_ctrl_characters (const std::string &s)
{
    std::string ss(s);
    
    auto e = std::remove_if(ss.begin(), ss.end(),
        [](const char c) {
            return (c >= 0x00 && c <= 0x1f) || c == 0x7f;
        }
    );
    ss.erase(e, ss.end());
    
    return ss;
}

//==============================================================================
//==============================================================================

std::vector<std::string> MoreStrings::split (const std::string &s, const std::string &delims)
{
    std::vector<std::string> tokens;
    
    std::string::size_type start = 0, end = 0;
    while ((end = s.find_first_of(delims, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(s.substr(start));

    return tokens;
}

//==============================================================================
//==============================================================================

std::string MoreStrings::escaped (const std::string &s)
{
    std::string ss(s);
    
    ss = find_and_replace(ss, "\\", "\\\\");
    ss = find_and_replace(ss, "\\", "\\\\");
    ss = find_and_replace(ss, "\n", "\\n");
    ss = find_and_replace(ss, "\t", "\\t");
    ss = find_and_replace(ss, "\"", "\\\"");
    
    return ss;
}

std::string MoreStrings::unescaped (const std::string &s)
{
    std::string ss;
    DTboolean escaped = false;
    
    for (auto &c : s) {
    
        if (escaped) {
            if (c == 'n')       ss += '\n';
            else if (c == 't')  ss += '\t';
            else if (c == '\\') ss += '\\';
            else if (c == '\"') ss += '\"';
        
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else {
            ss += c;
        }
    }
    
    return ss;
}

//==============================================================================
//==============================================================================

std::string MoreStrings::trim_end_digits (const std::string &s)
{
    std::string::size_type end = s.find_last_not_of("0123456789");
    if (end == std::string::npos)
        return s;

    std::string ss = s.substr(0,end+1);
    return ss;
}

std::string MoreStrings::end_digits (const std::string &s)
{
    std::string::size_type end = s.find_last_not_of("0123456789");
    if (end == std::string::npos)
        return s;
    
    return s.substr(end+1);
}

//==============================================================================
//==============================================================================

void MoreStrings::extract_unicode (DTcharacter *start, DTuint &num_bytes, DTuint &character)
{
    DTuint64 c = 0;
    
    DTubyte b0 = *start;
    
    // 1 byte long
    if ( (b0 & 0x80 /*10000000*/) == 0x00) {
        c = b0;
        num_bytes = 1;
        
    // 2 bytes long
    } else if ( (b0 & 0xE0 /*11100000*/) == 0xC0) {
        DTubyte b1 = *(start+1);
        
        c = (( (DTuint64) b0 & (~0xE0LL)) << (6*1)) |
            (( (DTuint64) b1 & (0x3FLL)) << (6*0));
 
        num_bytes = 2;
   
    // 3 bytes long
    } else if ( (b0 & 0xF0 /*11110000*/) == 0xE0) {
        DTubyte b1 = *(start+1);
        DTubyte b2 = *(start+2);
        
        c = (( (DTuint64) b0 & (~0xF0LL)) <<(6*2)) |
            (( (DTuint64) b1 & (0x3FLL)) << (6*1)) |
            (( (DTuint64) b2 & (0x3FLL)) << (6*0));

        num_bytes = 3;

    // 4 bytes long
    } else if ( (b0 & 0xF8 /*11111000*/) == 0xF0) {
        DTubyte b1 = *(start+1);
        DTubyte b2 = *(start+2);
        DTubyte b3 = *(start+3);
        
        c = (( (DTuint64) b0 & (~0xF8LL)) <<(6*3)) |
            (( (DTuint64) b1 & (0x3FLL)) << (6*2)) |
            (( (DTuint64) b2 & (0x3FLL)) << (6*1)) |
            (( (DTuint64) b3 & (0x3FLL)) << (6*0));

        num_bytes = 4;

    // 5 bytes long
    } else if ( (b0 & 0xFC /*11111100*/) == 0xF8) {
        DTubyte b1 = *(start+1);
        DTubyte b2 = *(start+2);
        DTubyte b3 = *(start+3);
        DTubyte b4 = *(start+4);
        
        c = (( (DTuint64) b0 & (~0xFCLL)) <<(6*4)) |
            (( (DTuint64) b1 & (0x3FLL)) << (6*3)) |
            (( (DTuint64) b2 & (0x3FLL)) << (6*2)) |
            (( (DTuint64) b3 & (0x3FLL)) << (6*1)) |
            (( (DTuint64) b4 & (0x3FLL)) << (6*0));

        num_bytes = 5;

    // 6 bytes long
    } else if ( (b0 & 0xFE /*11111110*/) == 0xFC) {
        DTubyte b1 = *(start+1);
        DTubyte b2 = *(start+2);
        DTubyte b3 = *(start+3);
        DTubyte b4 = *(start+4);
        DTubyte b5 = *(start+5);
        
        c = (( (DTuint64) b0 & (~0xFELL)) <<(6*5)) |
            (( (DTuint64) b1 & (0x3FLL)) << (6*4)) |
            (( (DTuint64) b2 & (0x3FLL)) << (6*3)) |
            (( (DTuint64) b3 & (0x3FLL)) << (6*2)) |
            (( (DTuint64) b4 & (0x3FLL)) << (6*1)) |
            (( (DTuint64) b5 & (0x3FLL)) << (6*0));
            
        num_bytes = 6;

    } else {
        c = *start;
        num_bytes = 1;
    }
    
    character = static_cast<DTuint>(c);
}

//==============================================================================
//==============================================================================

std::string MoreStrings::left_trim(const std::string &s) {
    std::string ss(s);

    ss.erase(ss.begin(), std::find_if(ss.begin(), ss.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return ss;
}

std::string MoreStrings::right_trim(const std::string &s) {
    std::string ss(s);

    ss.erase(std::find_if(ss.rbegin(), ss.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), ss.end());
    return s;
}

std::string MoreStrings::trim(const std::string &s) {
    return left_trim(right_trim(s));
}

//==============================================================================
//==============================================================================

std::string MoreStrings::captialize_and_format (const std::string &s)
{
    std::string ss = find_and_replace(s, "_", " ");
    
    if (ss.size() > 0) {
        ss[0] = static_cast<DTcharacter>(::toupper(ss[0]));   // First character
        
        for (std::string::size_type i = 1; i < ss.size(); ++i) {
            if (::isspace(ss[i-1]))
                ss[i] = static_cast<DTcharacter>(::toupper(ss[i]));
        }
    }

    return trim(ss);
}

std::string MoreStrings::camel_caps (const std::string &s)
{
    std::string ss = captialize_and_format(s);
    ss = find_and_replace(ss, " ", "");
    
    if (ss.size() > 0) {
        ss[0] = static_cast<DTcharacter>(::tolower(ss[0]));
    }
    
    return ss;
}

//==============================================================================
//==============================================================================

DTboolean MoreStrings::iequals (const std::string &a, const std::string &b)
{
#ifdef _WIN32
	return stricmp (a.c_str(), b.c_str()) == 0;
#else
	return strcasecmp (a.c_str(), b.c_str()) == 0;
#endif
}

DTboolean MoreStrings::iequals (const std::string &a, const char *b)
{
#ifdef _WIN32
	return stricmp (a.c_str(), b) == 0;
#else
	return strcasecmp (a.c_str(), b) == 0;
#endif
}

DTboolean MoreStrings::iequals (const char *a, const std::string &b)
{
#ifdef _WIN32
	return stricmp (a, b.c_str()) == 0;
#else
	return strcasecmp (a, b.c_str()) == 0;
#endif
}


//==============================================================================
//==============================================================================

} // DT3
