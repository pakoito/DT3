//==============================================================================
///	
///	File: Tokenizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Tokenizer.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/ExpressionParser.hpp"
#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Globals.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Tokenizer::Tokenizer (void)
{

}
		
Tokenizer::Tokenizer (const Tokenizer &rhs)
    :   _pending_tokens (rhs._pending_tokens),
        _defines        (rhs._defines),
        _dependencies   (rhs._dependencies)
{

}

Tokenizer::Tokenizer (Tokenizer &&rhs)
    :   _pending_tokens (std::move(rhs._pending_tokens)),
        _defines        (std::move(rhs._defines)),
        _dependencies   (std::move(rhs._dependencies))
{

}

Tokenizer & Tokenizer::operator = (const Tokenizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	
        _pending_tokens = rhs._pending_tokens;
        _defines = rhs._defines;
        _dependencies = rhs._dependencies;
    }
    return (*this);
}

Tokenizer & Tokenizer::operator = (Tokenizer &&rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {	
        _pending_tokens = std::move(rhs._pending_tokens);
        _defines = std::move(rhs._defines);
        _dependencies = std::move(rhs._dependencies);
    }
    return (*this);
}
	
Tokenizer::~Tokenizer (void)
{

}

//==============================================================================
//==============================================================================

void Tokenizer::syntax_error(std::string err)
{
    std::string error_message = "Error parsing: " + err;
    ERRORMSG(error_message.c_str());
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_whitespace(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	DTboolean had_space = false;
	
	while ( (pos < (DTint) buffer.size()) && (::isspace(buffer[pos]) || buffer[pos] == ';') ) {
		++pos;
		had_space = true;
	}
	
	return had_space;
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_symbol(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	std::string token;
	token.reserve(128);
	
	while (	(pos < (DTint) buffer.size()) && (isalpha(buffer[pos]) || isdigit(buffer[pos]) || buffer[pos]=='#' || buffer[pos]=='_' || buffer[pos]=='.' || buffer[pos]=='-' || buffer[pos]=='+' || buffer[pos]==':')) {
		token += buffer[pos];
		++pos;
	}
	
	if (token.size() > 0) {
		tokens.push_back(token);
		return true;
	} else {
		return false;
	}
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_equals(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	std::string token;
	token.reserve(128);

	while (	(pos < (DTint) buffer.size()) && buffer[pos] == '=') {
		token += buffer[pos];
		++pos;
	}
	
	if (token.size() > 0) {
		tokens.push_back(token);
		return true;
	} else {
		return false;
	}
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_braces(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	std::string token;
	token.reserve(128);
	
	while (	(pos < (DTint) buffer.size()) && (buffer[pos] == '{' || buffer[pos] == '}')) {
		token += buffer[pos];
		++pos;
	}
	
	if (token.size() > 0) {
		tokens.push_back(token);
		return true;
	} else {
		return false;
	}
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_string(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	if (buffer[pos] != '\"')	return false;
	++pos;
	
	DTboolean escaped = false;
	std::string token;
	token.reserve(128);
	
	while ( (pos < (DTint) buffer.size()) && (escaped || buffer[pos] != '\"') ) {
	
		if (!escaped && buffer[pos] == '\\') {
			escaped = true;
		} else {
			token += buffer[pos];
			escaped = false;
		}
	
		++pos;
	}
	
	++pos;
	
	// add the token
	tokens.push_back(token);
	return true;
}

//==============================================================================
//==============================================================================

DTboolean Tokenizer::parse_multi_param(const std::string &buffer, DTint &pos, std::list<std::string> &tokens)
{
	if (buffer[pos] != '(')	return false;
	++pos;
	
	std::string token;
	token.reserve(128);
	
	while ( (pos < (DTint) buffer.size()) && buffer[pos] != ')' ) {
		token += buffer[pos];
		++pos;
	}
	
	++pos;
	
	// add the token
	tokens.push_back(token);
	return true;
}

//==============================================================================
//==============================================================================

DTerr Tokenizer::load_token_stream (const FilePath &pathname, DTboolean append)
{
	TextFileStream infile;
	DTerr err;
	if ((err = FileManager::open(infile, pathname, true)) != DT3_ERR_NONE) {
		LOG_MESSAGE << "Unable to open " << pathname.full_path();
		return err;
	}

    LOG_MESSAGE << "Parsing " << pathname.full_path();
		
	std::string contents;
	contents.reserve(infile.length());
	
	while (1) {
		DTcharacter buffer[1024*16];
		infile.line(buffer,sizeof(buffer));

		if (infile.is_eof())
			break;
						
		// Preprocess buffer for comments
		std::string buffer_str(buffer);

		std::size_t comment = buffer_str.find("///");
		if (comment != std::string::npos) {
			buffer_str = buffer_str.substr(0,comment);
		}
		
		// Put the contents of the buffer in another stream
		contents += buffer_str;
		contents += "\n";
	}
	
	return parse_token_stream(contents, append);
}

DTerr Tokenizer::set_token_stream (const std::string &s, DTboolean append)
{
	std::stringstream ss(s.c_str());
	
	std::string contents;
	contents.reserve(s.size());
	
	while (1) {
		DTcharacter buffer[1024*16];
		ss.getline(buffer,sizeof(buffer));
								
		// Preprocess buffer for comments
		std::string buffer_str(buffer);
		std::size_t comment = buffer_str.find("///");
		if (comment != std::string::npos) {
			buffer_str = buffer_str.substr(0,comment);
		}
		
		// Put the contents of the buffer in another stream
		contents += buffer_str;
		contents += "\n";
        
		if (ss.eof())
			break;
	}
	
	return parse_token_stream(contents, append);
}

DTerr Tokenizer::parse_token_stream (const std::string &contents, DTboolean append)
{
	// Parse out tokens
	std::list<std::string>	tokens;
	DTint pos = 0;
	DTint old_pos = 0;
    
	while (pos < (DTint) contents.size()) {
                    
		if (parse_whitespace(contents, pos, tokens))	{   old_pos = pos;  continue;   }
		if (parse_string(contents, pos, tokens))		{   old_pos = pos;  continue;   }
		if (parse_symbol(contents, pos, tokens))		{   old_pos = pos;  continue;   }
		if (parse_equals(contents, pos, tokens))		{   old_pos = pos;  continue;   }
		if (parse_braces(contents, pos, tokens))		{   old_pos = pos;  continue;   }
		if (parse_multi_param(contents, pos, tokens))	{   old_pos = pos;  continue;   }
		
		syntax_error("Invalid Token: " + contents.substr(old_pos, 1000));
        
        for (auto i = tokens.rbegin(); i != tokens.rend(); ++i)
            syntax_error("Last Tokens: " + *i);
                        
		return DT3_ERR_TOKENIZE_FAILED;
	}
	
	// insert the new tokens
    if (append) {
        _pending_tokens.insert(_pending_tokens.end(), tokens.begin(), tokens.end());
    } else {
        _pending_tokens.insert(_pending_tokens.begin(), tokens.begin(), tokens.end());
    }
	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

std::string Tokenizer::next_token_string (void)
{
	std::string token = next_token_string_no_substitute();

	// Substitute globals
	token = Globals::substitute_global(token);
	
	return token;
}

std::string Tokenizer::next_token_string_no_substitute (void)
{
	std::string token = _pending_tokens.front();
	_pending_tokens.pop_front();
	
	// Substitute defines
	auto i = _defines.find(token);
	if (i != _defines.end())
		token = i->second;
			
	return token;
}

DTfloat Tokenizer::next_token_number (void)
{
	std::string token = _pending_tokens.front();
	_pending_tokens.pop_front();
		
	// Substitute defines
	auto i = _defines.find(token);
	if (i != _defines.end())
		token = i->second;
		
	// Substitute globals
	token = Globals::substitute_global(token);
	
	DTfloat token_num = MoreStrings::cast_from_string<DTfloat>(token);
	
	return token_num;
}

DTuint Tokenizer::next_token_hex (void)
{
	std::string token = _pending_tokens.front();
	_pending_tokens.pop_front();
		
	// Substitute defines
	auto i = _defines.find(token);
	if (i != _defines.end())
		token = i->second;
		
	// Substitute globals
	token = Globals::substitute_global(token);
	    
    DTuint result = 0;
    MoreStrings::from_hex_string (token, &result, 4);
	
	return result;
}

//==============================================================================
//==============================================================================

void Tokenizer::assume_next_token (const std::string &expected_token)
{
	std::string actual_token = next_token_string();
	if (actual_token != expected_token)
		syntax_error("Expected " + expected_token);
}

//==============================================================================
//==============================================================================


//==============================================================================
/// Implement the preprocessor
//==============================================================================

void Tokenizer::preprocessor_include			(void)
{
	FilePath file = FilePath(next_token_string());
    
	load_token_stream(file, false);
    _dependencies.push_back(file);
}

void Tokenizer::preprocessor_if				(void)
{
	std::string arg = next_token_string();
  
    DTfloat result;

    if (MoreStrings::iequals(arg,"0") ||
		MoreStrings::iequals(arg,"FALSE") ||
		MoreStrings::iequals(arg,"") ) {
        
        result = 0.0F;
        
    } else if ( MoreStrings::iequals(arg,"1") ||
                MoreStrings::iequals(arg,"TRUE")) {

        result = 1.0F;
        
    } else {
    
        ExpressionParser expr;
        
        if (!expr.parse(arg)) {
            LOG_MESSAGE << "Syntax error in parsing expression " << arg;
            return;
        }

        if (!expr.eval(result)) {
            LOG_MESSAGE << "Syntax error in evaluating expression " << arg;
            return;
        }
        
    }
	
	if (result == 0.0F) {
			
		// Eat tokens to #endif
        std::string token = next_token_string();
		while (!MoreStrings::iequals(token, "#endif") && _pending_tokens.size() > 0)
			token = next_token_string();
	}
}

void Tokenizer::preprocessor_define			(void)
{
	std::string symbol = next_token_string();
	std::string value = next_token_string();
	
	_defines[symbol] = value;
}

void Tokenizer::preprocessor_print          (void)
{
    LOG_MESSAGE << next_token_string();
}

//==============================================================================
//==============================================================================		

DTboolean Tokenizer::parse_preprocessor_macros (std::string token)
{
	if (MoreStrings::iequals(token,"#include"))		{	preprocessor_include();		return true;	}
	if (MoreStrings::iequals(token,"#if"))			{	preprocessor_if();			return true;	}
	if (MoreStrings::iequals(token,"#define"))		{	preprocessor_define();		return true;	}
	if (MoreStrings::iequals(token,"#endif"))       {	return true;	}   // Ignore any endifs

	if (MoreStrings::iequals(token,"#print"))		{	preprocessor_print();		return true;	}

	return false;
}

//==============================================================================
//==============================================================================

} // DT3
