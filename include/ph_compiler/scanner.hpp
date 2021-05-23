/**
 "The first step in any compiler or interpreter is scanning.
 The scanner takes in raw source code as a series of characters and groups it
 into a series of chunks we call tokens. These are the meaningful “words” and
 “punctuation” that make up the language’s grammar."
 
 https://craftinginterpreters.com/scanning.html
 */


#pragma once

using namespace std;


enum struct token_type
{
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
    
    TOKEN_ERROR, TOKEN_EOF
};

struct token
{
    token_type _type;
    char const* _start;
    int _length;
    int _line;
};


struct scanner
{
    char const* _start;
    char const* _current;
    int _line;
    
    scanner (char const* source) : _start {source}, _current {source}, _line {1}
    {
        
    }
    
    auto scan_token () -> token
    {
        _start = _current;
        
        if (is_at_end ())
        {
            return make_token (token_type::TOKEN_EOF);
        }
        
        return error_token ("Unexpected character.");
    }
    
private:
    auto is_at_end () const -> bool
    {
        return *_current == '\0';
    }
    
    inline auto make_token (token_type type) -> token
    {
        return token
        {
            ._type = type,
            ._start = _start,
            ._length = (int) (_current - _start),
            ._line = _line
        };
    }
    
    auto error_token (const char* message) -> token
    {
        return
        {
            ._type = token_type::TOKEN_ERROR,
            ._start = message,
            ._length = (int) strlen (message),
            ._line = _line
        };
    }
};


