#ifndef DT3_TOKENIZER
#define DT3_TOKENIZER
//==============================================================================
///	
///	File: Tokenizer.hpp
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
#include <vector>
#include <list>
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;

//==============================================================================
/// Class
//==============================================================================

class Tokenizer: public BaseClass {
    public:
        DEFINE_TYPE(Tokenizer,BaseClass)
		DEFINE_CREATE_AND_CLONE
         
										Tokenizer                       (void);
										Tokenizer                       (const Tokenizer &rhs);
										Tokenizer                       (Tokenizer &&rhs);
        Tokenizer &						operator =                      (const Tokenizer &rhs);
        Tokenizer &						operator =                      (Tokenizer &&rhs);
        virtual							~Tokenizer                      (void);
 
	public:
		/// Loads a token stream from a file
		/// \param pathname Path to file
		/// \param append Append to end of token stream, otherwise insert at current location
		/// \return Error
		DTerr							load_token_stream               (const FilePath &pathname, DTboolean append = true);

		/// Loads a token stream from a string
		/// \param s String to parse
		/// \param append Append to end of token stream, otherwise insert at current location
		/// \return Error
		DTerr							set_token_stream                (const std::string &s, DTboolean append = true);

		/// Called when the tokenizer gets a syntax error  TODO: Make Callback
		/// \param err Error Message
		void							syntax_error                    (std::string err);
		
		/// Get the next token as a string
		/// \return Next token
		std::string                     next_token_string               (void);

		/// Get the next token as a string and don't do globals substitution
		/// \return Next token
		std::string                     next_token_string_no_substitute (void);

		/// Get the next token as a number
		/// \return Next token
		DTfloat							next_token_number               (void);

		/// Get the next token as a hex string
		/// \return Next token
		DTuint							next_token_hex                  (void);

		/// Eat and check the next token
		/// \param expected_token Token to eat
		void							assume_next_token               (const std::string &expected_token);

		/// If the token is a preprocessor macro, then process it
		/// \param token token to process
		/// \return was processed
		DTboolean						parse_preprocessor_macros       (std::string token);
		
		/// Returns wether the token stream is empty or not
		/// \return returns true token stream is empty
		DTboolean						is_done                         (void)              {	return _pending_tokens.size() == 0;	}

        /// Returns a list of all of the files that were loaded during processing of this stream
		/// \return Files processed
        const std::vector<FilePath>&    dependencies                    (void) const        {   return _dependencies;               }

	private:
		DTerr							parse_token_stream              (const std::string &contents, DTboolean append = true);
	
		DTboolean						parse_whitespace                (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
		DTboolean						parse_symbol                    (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
		DTboolean						parse_equals                    (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
		DTboolean						parse_braces                    (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
		DTboolean						parse_string                    (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
		DTboolean						parse_multi_param               (const std::string &buffer, DTint &pos, std::list<std::string> &tokens);
	
		void							preprocessor_include            (void);
		void							preprocessor_if                 (void);
		void							preprocessor_define             (void);
		void							preprocessor_print              (void);

		// Tokenizer
		std::list<std::string>              _pending_tokens;
		std::map<std::string,std::string>   _defines;
        
        std::vector<FilePath>               _dependencies;

};

//==============================================================================
//==============================================================================


} // DT3

#endif
