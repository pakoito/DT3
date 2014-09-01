//==============================================================================
///	
///	File: ExpressionParser.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/ExpressionParser.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Globals.hpp"

#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION(ExpressionParser)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ExpressionParser::ExpressionParser (void)
{

}
			
ExpressionParser::~ExpressionParser (void)
{

}

//==============================================================================
//==============================================================================

void ExpressionParser::set_variable (const std::string &var, const DTfloat &value)
{
    _variables[var] = value;
}

//==============================================================================
//==============================================================================

DTboolean ExpressionParser::parse_whitespace (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    DTcharacter c = string[pos];
    if (::isspace(c)) {
        ++pos;
        return true;
    } else {
        return false;
    }
}

DTboolean ExpressionParser::parse_comma (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];

    if (c == ',') {
        token += c;
        ++pos;
    }

    if (token.size() > 0) {
        Token t;
        t._token = token;
        t._type = TYPE_COMMA;
        tokens.push_back(t);
        return true;
    } else {
        return false;
    }
}

DTboolean ExpressionParser::parse_operator (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];
    
    if (    c == '+' || // Addition
            c == '-' || // Subtraction
            c == '/' || // Division
            c == '*' || // Multiplication
            c == '|' || // Logic OR
            c == '&' || // Logic AND
            c == '!' || // Logic NOT
            c == '=' || // Equals
            c == '>' || // Greater Than
            c == '<' || // Less Than
            c == '^'    // Exponentiation
    ) {
        token += c;
        ++pos;
    }
    
    if (token.size() > 0) {
        Token t;
        t._token = token;
        t._type = TYPE_OPERATOR;
        
        // Check unary
        if (tokens.size() == 0 || tokens.back()._type == TYPE_OPEN_BRACKET || tokens.back()._type == TYPE_OPERATOR)
            t._unary = true;
        else
            t._unary = false;
        
        tokens.push_back(t);
        return true;
    } else {
        return false;
    }
    
}

DTboolean ExpressionParser::parse_function (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];
    
    // First character must be alphabetical
    if (!::isalpha(c))
        return false;
        
    while ( ::isalpha(c) || ::isdigit(c) ) {
        token += c;
        ++pos;
        c = string[pos];
    }
    
    Token t;
    t._token = token;
    t._type = TYPE_FUNCTION;
    tokens.push_back(t);
    return true;
}

DTboolean ExpressionParser::parse_number (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];
    
    // First character must be alphabetical
    if (!::isdigit(c))
        return false;
        
    while ( ::isdigit(c) || c == '.' ) {
        token += c;
        ++pos;
        c = string[pos];
    }
    
    Token t;
    t._token = token;
    t._type = TYPE_NUMBER;
    tokens.push_back(t);
    return true;
}

DTboolean ExpressionParser::parse_variable (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];
    
    // First character must be alphabetical
    if (c != '$')
        return false;
        
    token += c;
    ++pos;
    c = string[pos];

    while ( ::isalpha(c) || ::isdigit(c) ) {
        token += c;
        ++pos;
        c = string[pos];
    }
    
    Token t;
    t._token = token;
    t._type = TYPE_NUMBER;
    tokens.push_back(t);
    return true;
}

DTboolean ExpressionParser::parse_bracket (const std::string &string, DTuint &pos, std::list<Token> &tokens)
{
    std::string token;
    DTcharacter c = string[pos];

    if (c == '(' || c == ')') {
        token += c;
        ++pos;
    }

    if (token.size() > 0) {
        Token t;
        t._token = token;
        t._type = (c == '(') ? TYPE_OPEN_BRACKET : TYPE_CLOSE_BRACKET;
        tokens.push_back(t);
        return true;
    } else {
        return false;
    }
}

//==============================================================================
//==============================================================================

DTuint ExpressionParser::precedence(Token *t)
{
    if (t->_token == "!")   return 6;
    if (t->_token == "+" && t->_unary)   return 6;
    if (t->_token == "-" && t->_unary)   return 6;

    if (t->_token == "^")   return 5;
    
    if (t->_token == "*")   return 4;
    if (t->_token == "/")   return 4;
    
    if (t->_token == "+")   return 3;
    if (t->_token == "-")   return 3;
    
    if (t->_token == "|")   return 1;
    if (t->_token == "&")   return 1;

    return 0;
}

DTboolean ExpressionParser::left_associative(Token *t)
{
    if (t->_token == "^")   return false;
    if (t->_token == ">")   return false;
    if (t->_token == "<")   return false;
    if (t->_token == ">=")  return false;
    if (t->_token == "<=")  return false;
    if (t->_token == "==")  return false;
    if (t->_token == "!=")  return false;

    return true;
}

//==============================================================================
//==============================================================================

DTboolean ExpressionParser::parse (const std::string &contents)
{
    _tokens.clear();
    _output_queue.clear();
    
    std::string full_contents = Globals::substitute_global(contents);
    
    //
    // Tokenize expression
    //
    
    DTuint pos = 0;
    
    while (pos < full_contents.size()) {
        if (parse_whitespace(full_contents, pos, _tokens)) continue;
        if (parse_comma(full_contents, pos, _tokens))      continue;
        if (parse_operator(full_contents, pos, _tokens))   continue;
        if (parse_function(full_contents, pos, _tokens))   continue;
        if (parse_number(full_contents, pos, _tokens))     continue;
        if (parse_variable(full_contents, pos, _tokens))   continue;
        if (parse_bracket(full_contents, pos, _tokens))    continue;
    
        LOG_MESSAGE << "Syntax Error";
        
        _tokens.clear();
        _output_queue.clear();

        return false;
    }
    
    //
    // Evaluate expression using Shunting Yard
    // http://en.wikipedia.org/wiki/Shunting_yard_algorithm
    //
    
    std::list<Token*>    stack;
        
    for (auto i = _tokens.begin(); i != _tokens.end(); ) {

        // If the token is a number, then add it to the output queue.
        if (i->_type == TYPE_NUMBER)
            _output_queue.push_back( &(*i) );
            
        // If the token is a function token, then push it onto the stack.
        else if (i->_type == TYPE_FUNCTION)
            stack.push_back( &(*i) );
            
        // If the token is a function argument separator
        else if (i->_type == TYPE_COMMA) {
            // Until the token at the top of the stack is a left 
            // parenthesis, pop operators off the stack onto the 
            // output queue. If no left parentheses are encountered, 
            // either the separator was misplaced or parentheses 
            // were mismatched.
        
            while (i->_type != TYPE_OPEN_BRACKET) {
                _output_queue.push_back(stack.back());
                stack.pop_back();
                
                ++i;
                if (i == _tokens.end()) {
                    LOG_MESSAGE << "Unmatched bracket";
                    
                    _tokens.clear();
                    _output_queue.clear();

                    return false;
                }
            }
            
        // If the token is an operator, o1, then:
        } else if (i->_type == TYPE_OPERATOR) {
        
            while (stack.size() > 0) {

                if( (stack.back()->_type == TYPE_OPERATOR) && 
                    ((left_associative( &(*i) ) && (precedence( &(*i) ) <= precedence( stack.back() ))) ||
                    (!left_associative( &(*i) ) && (precedence( &(*i) ) < precedence( stack.back() ))))) {

                    // pop o2 off the stack, onto the output queue
                    _output_queue.push_back(stack.back());
                    stack.pop_back();
                    
                } else {
                    break;
                }   
                
            }
            
            stack.push_back( &(*i) );
        
        // If the token is a left parenthesis, then push it onto the stack.
        } else if (i->_type == TYPE_OPEN_BRACKET) {
            stack.push_back( &(*i) );
            
        // If the token is a right parenthesis
        } else if (i->_type == TYPE_CLOSE_BRACKET) {
        
            // Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
            while (stack.size() > 0 && stack.back()->_type != TYPE_OPEN_BRACKET) {
                _output_queue.push_back(stack.back());
                stack.pop_back();
                
                if (stack.size() == 0) {
                    LOG_MESSAGE << "Unmatched bracket";
                    
                    _tokens.clear();
                    _output_queue.clear();

                    return false;
                }
            }
            
            // Pop the left parenthesis from the stack, but not onto the output queue.
            stack.pop_back();

            // If the token at the top of the stack is a function token, pop it onto the output queue.
            if (stack.size() > 0 && stack.back()->_type == TYPE_FUNCTION) {
                _output_queue.push_back(stack.back());
                stack.pop_back();
            }

        }

        ++i;
    }
    
    // When there are no more tokens to read
    while (stack.size() > 0) {
        if (stack.back()->_type == TYPE_OPEN_BRACKET || 
            stack.back()->_type == TYPE_CLOSE_BRACKET ) {
            
            LOG_MESSAGE << "Unmatched bracket";
            
            _tokens.clear();
            _output_queue.clear();

            return false;
        }
            
        _output_queue.push_back(stack.back());
        stack.pop_back();
    }
        
//    FOR_EACH (j,_output_queue) {
//        LOG_MESSAGE << (**j)._token;
//    }

    return true;
}

//==============================================================================
//==============================================================================

#define POP(a) { if (stack.size() == 0) return false; a = stack.back();   stack.pop_back();    }

DTboolean ExpressionParser::eval (DTfloat &result)
{
    result = 0.0F;
    
    std::list<DTfloat> stack;

    FOR_EACH (i,_output_queue) {
        std::string op = (**i)._token;
        
        // Push numbers onto the stack
        if ( (**i)._type == TYPE_NUMBER ) {
            
            // If this is a variable, substitute it
            if (op.size() > 0 && op[0] == '$') {
                std::string var = op.substr(1);
                
                if (_variables.find(var) != _variables.end()) {
                    stack.push_back(_variables[var]);
                } else {
                    // Cannot find variable
                    return false;
                }
                    
            } else {
                stack.push_back(MoreStrings::cast_from_string<DTfloat>( op ));
            }
            
            continue;
        }
     
        DTboolean unary = (**i)._unary;
        DTuint type = (**i)._type;
        
        // Functions
        if (type == TYPE_FUNCTION) {
            if (op == "abs") {
                DTfloat a;
                POP(a);
                stack.push_back(std::abs(a));
                continue;
            }
            
            if (op == "sin") {
                DTfloat a;
                POP(a);
                stack.push_back(std::sin(a));
                continue;
            }
            
            if (op == "cos") {
                DTfloat a;
                POP(a);
                stack.push_back(std::cos(a));
                continue;
            }
            
            if (op == "tan") {
                DTfloat a;
                POP(a);
                stack.push_back(std::tan(a));
                continue;
            }

            if (op == "asin") {
                DTfloat a;
                POP(a);
                stack.push_back(std::asin(a));
                continue;
            }
            
            if (op == "acos") {
                DTfloat a;
                POP(a);
                stack.push_back(std::acos(a));
                continue;
            }
            
            if (op == "atan") {
                DTfloat a;
                POP(a);
                stack.push_back(std::atan(a));
                continue;
            }
            
            if (op == "floor") {
                DTfloat a;
                POP(a);
                stack.push_back(std::floor(a));
                continue;
            }
            
            if (op == "ceil") {
                DTfloat a;
                POP(a);
                stack.push_back(std::ceil(a));
                continue;
            }
            
            LOG_MESSAGE << "Unknown function: " << op;
            return false;
        }
        
        // Operations
        if (type == TYPE_OPERATOR) {
            if (op == "*" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(a*b);
                continue;
            }

            if (op == "/" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b/a);
                continue;
            }
            
            if (op == "+" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(a+b);
                continue;
            }
            
            if (op == "+" && unary) {
                // Does nothing
                continue;
            }
            
            if (op == "-" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b-a);
                continue;
            }

            if (op == "-" && unary) {
                DTfloat a;
                POP(a);
                stack.push_back(-a);
                continue;
            }

            if (op == "|" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back( (static_cast<DTboolean>(a) || static_cast<DTboolean>(b)) ? 1.0F : 0.0F );
                continue;
            }

            if (op == "&" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back( (static_cast<DTboolean>(a) && static_cast<DTboolean>(b)) ? 1.0F : 0.0F );
                continue;
            }

            if (op == "!" && unary) {
                DTfloat a;
                POP(a);
                stack.push_back( (!static_cast<DTboolean>(a)) ? 1.0F : 0.0F );
                continue;
            }

            if (op == ">" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b>a);
                continue;
            }

            if (op == "<" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b<a);
                continue;
            }

            if (op == ">=" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b>=a);
                continue;
            }

            if (op == "<=" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b<=a);
                continue;
            }

            if (op == "==" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b==a);
                continue;
            }

            if (op == "!=" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(b!=a);
                continue;
            }


            if (op == "^" && !unary) {
                DTfloat a,b;
                POP(a);
                POP(b);
                stack.push_back(std::pow(b,a));
                continue;
            }
            
            LOG_MESSAGE << "Unknown operator: " << op;
        }

    }
    
    if (stack.size() == 0) {
        return false;
    } else {
        result = stack.back();
        return true;
    }
}

//==============================================================================
//==============================================================================

} // DT3
