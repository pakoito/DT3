#ifndef DT3_EXPRESSIONPARSER
#define DT3_EXPRESSIONPARSER
//==============================================================================
///	
///	File: ExpressionParser.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <list>
#include <map>
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ExpressionParser:public BaseClass {
    public:
        DEFINE_TYPE(ExpressionParser, BaseClass)
         
									ExpressionParser        (void);	
	private:
									ExpressionParser        (const ExpressionParser &rhs);
        ExpressionParser &          operator =              (const ExpressionParser &rhs);
        
	public:
        virtual						~ExpressionParser       (void);
		
	public:
		/// Compile the expression
		/// \param s Expression string
		/// \return Success
        DTboolean                   parse                   (const std::string &s);
        
		/// Evaluate the expression
		/// \param result Where the result is stored
		/// \return Success
        DTboolean                   eval                    (DTfloat &result);

		/// Set an external variable for the expression
		/// \param var variable name
		/// \param value variable value
        void                        set_variable            (const std::string &var, const DTfloat &value);

    private:
        struct Token {
            std::string             _token;
            DTboolean               _unary;
            DTuint                  _type;
        };

        DTboolean                   parse_whitespace        (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_comma             (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_operator          (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_function          (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_number            (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_variable          (const std::string &string, DTuint &pos, std::list<Token> &tokens);
        DTboolean                   parse_bracket           (const std::string &string, DTuint &pos, std::list<Token> &tokens);

        enum {
            TYPE_OPERATOR,
            TYPE_FUNCTION,
            TYPE_NUMBER,
            TYPE_OPEN_BRACKET,
            TYPE_CLOSE_BRACKET,
            TYPE_COMMA
        };
        
        DTuint                      precedence              (Token *t);
        DTboolean                   left_associative        (Token *t);
                
        std::list<Token>            _tokens;
        std::list<Token*>           _output_queue;
        
        std::map<std::string, DTfloat>   _variables;
};

//==============================================================================
//==============================================================================


} // DT3

#endif
