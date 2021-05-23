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
        skip_whitespaces ();
        _start = _current;
        
        if (is_at_end ())
        {
            return make_token (token_type::TOKEN_EOF);
        }
        
        char c = advance ();
        
        
        if (is_alpha (c))
        {
            return identifier ();
        }
        
        if (isdigit (c))
        {
            return number ();
        }
        
        switch (c)
        {
            case '(': return make_token (token_type::TOKEN_LEFT_PAREN);
            case ')': return make_token (token_type::TOKEN_RIGHT_PAREN);
            case '{': return make_token (token_type::TOKEN_LEFT_BRACE);
            case '}': return make_token (token_type::TOKEN_RIGHT_BRACE);
            case ';': return make_token (token_type::TOKEN_SEMICOLON);
            case ',': return make_token (token_type::TOKEN_COMMA);
            case '.': return make_token (token_type::TOKEN_DOT);
            case '-': return make_token (token_type::TOKEN_MINUS);
            case '+': return make_token (token_type::TOKEN_PLUS);
            case '/': return make_token (token_type::TOKEN_SLASH);
            case '*': return make_token (token_type::TOKEN_STAR);
            case '!':
                return make_token (match ('=') ? token_type::TOKEN_BANG_EQUAL : token_type::TOKEN_BANG);
            case '=':
                return make_token (match ('=') ? token_type::TOKEN_EQUAL_EQUAL : token_type::TOKEN_EQUAL);
            case '<':
                return make_token (match ('=') ? token_type::TOKEN_LESS_EQUAL : token_type::TOKEN_LESS);
            case '>':
                return make_token (match ('=') ? token_type::TOKEN_GREATER_EQUAL : token_type::TOKEN_GREATER);
                
            case '"':
                return string ();
        }
        
        return error_token ("Unexpected character.");
    }
    
private:
    
    auto check_keyword (int start, int length, char const* rest, token_type type) -> token_type
    {
        if (_current - _start == start + length and memcmp (_start + start, rest, length) == 0)
        {
            return type;
        }
        return token_type::TOKEN_IDENTIFIER;
    }
    auto identifier_type () -> token_type
    {
        switch (_start [0])
        {
            case 'a': return check_keyword (1, 2, "nd", token_type::TOKEN_AND);
            case 'c': return check_keyword (1, 4, "lass", token_type::TOKEN_CLASS);
            case 'e': return check_keyword (1, 3, "lse", token_type::TOKEN_ELSE);
            case 'f':
                  if (_current - _start > 1)
                  {
                    switch (_start [1])
                    {
                        case 'a': return check_keyword (2, 3, "lse", token_type::TOKEN_FALSE);
                        case 'o': return check_keyword (2, 1, "r", token_type::TOKEN_FOR);
                        case 'u': return check_keyword (2, 1, "n", token_type::TOKEN_FUN);
                    }
                  }
                  break;
            case 'i': return check_keyword (1, 1, "f", token_type::TOKEN_IF);
            case 'n': return check_keyword (1, 2, "il", token_type::TOKEN_NIL);
            case 'o': return check_keyword (1, 1, "r", token_type::TOKEN_OR);
            case 'p': return check_keyword (1, 4, "rint", token_type::TOKEN_PRINT);
            case 'r': return check_keyword (1, 5, "eturn", token_type::TOKEN_RETURN);
            case 's': return check_keyword (1, 4, "uper", token_type::TOKEN_SUPER);
            case 't':
                  if (_current - _start > 1)
                  {
                    switch (_start [1])
                    {
                        case 'h': return check_keyword (2, 2, "is", token_type::TOKEN_THIS);
                        case 'r': return check_keyword (2, 2, "ue", token_type::TOKEN_TRUE);
                    }
                  }
                  break;
            case 'v': return check_keyword (1, 2, "ar", token_type::TOKEN_VAR);
            case 'w': return check_keyword (1, 4, "hile", token_type::TOKEN_WHILE);
        }
        
        return token_type::TOKEN_IDENTIFIER;
    }
    auto identifier() -> token
    {
        while (is_alpha(peek()) || isdigit(peek ())) advance ();
        return make_token (identifier_type ());
    }
    auto is_alpha (char c) -> bool
    {
        return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
    }
    auto number () -> token
    {
        while (isdigit (peek ()))
        {
            advance ();
        }
        
        if (peek () == '.' && isdigit (peek_next ()))
        {
            advance ();
            
            while (isdigit (peek ()))
            {
                advance ();
            }
        }
        return make_token (token_type::TOKEN_NUMBER);
    }
    auto string () -> token
    {
        while (peek () != '"' and ! is_at_end ())
        {
            if (peek () == '\n')
            {
                ++_line;
                advance ();
            }
        }
        if (is_at_end ())
        {
            return error_token ("Unterminated string.");
        }
        
        // The closing quote.
        advance ();
        return make_token (token_type::TOKEN_STRING);
    }
    auto skip_whitespaces () -> void
    {
        for (;;)
        {
            switch (peek ())
            {
                case ' ':
                case '\r':
                case '\t':
                    
                    advance ();
                    break;
                    
                case '\n':
                    ++_line;
                    advance ();
                    break;
                    
                case '/':
                    if (peek_next () == '/')
                    {
                        while (peek () != '\n' && is_at_end())
                        {
                            advance ();
                        }
                    } else
                    {
                        return;
                    }
                    break;
                    
                default:
                    return;
            }
        }
    }
    
    auto peek_next() -> char
    {
        if (is_at_end ()) return '\0';
        return _current [1];
    }
    
    auto peek () -> char
    {
        return *_current;
    }
    // if found, consume and return it
    auto match (char expected) -> bool
    {
        if (is_at_end ()) return false;
        if (*_current != expected) return false;
        ++_current;
        return true;
    }
    auto advance () -> char
    {
        ++_current;
        return _current [-1];
    }
    auto is_at_end () const -> bool
    {
        return *_current == '\0';
    }
    
    inline auto make_token (token_type type) -> token
    {
        return token
        {
            ._type = type,
            ._start = this -> _start,
            ._length = (int) (this -> _current - this -> _start),
            ._line = this -> _line
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


