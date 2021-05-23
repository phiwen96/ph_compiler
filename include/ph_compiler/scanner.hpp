/**
 "The first step in any compiler or interpreter is scanning.
 The scanner takes in raw source code as a series of characters and groups it
 into a series of chunks we call tokens. These are the meaningful “words” and
 “punctuation” that make up the language’s grammar."
 
 https://craftinginterpreters.com/scanning.html
 */


#pragma once

using namespace std;


enum token_type
{
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // Literals.
    IDENTIFIER, STRING, NUMBER,
    // Keywords.
    AND, CLASS, ELSE, FALSE,
    FOR, FUN, IF, NIL, OR,
    PRINT, _RETURN, SUPER, THIS,
    TRUE, VAR, WHILE,
    
    ERROR, _EOF
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
        
        // check to see if we’ve reached the end of the source code
        if (is_at_end ())
        {
            return make_token (token_type::_EOF);
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
            case '(': return make_token (token_type::LEFT_PAREN);
            case ')': return make_token (token_type::RIGHT_PAREN);
            case '{': return make_token (token_type::LEFT_BRACE);
            case '}': return make_token (token_type::RIGHT_BRACE);
            case ';': return make_token (token_type::SEMICOLON);
            case ',': return make_token (token_type::COMMA);
            case '.': return make_token (token_type::DOT);
            case '-': return make_token (token_type::MINUS);
            case '+': return make_token (token_type::PLUS);
            case '/': return make_token (token_type::SLASH);
            case '*': return make_token (token_type::STAR);
            case '!':
                return make_token (match ('=') ? token_type::BANG_EQUAL : token_type::BANG);
            case '=':
                return make_token (match ('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
            case '<':
                return make_token (match ('=') ? token_type::LESS_EQUAL : token_type::LESS);
            case '>':
                return make_token (match ('=') ? token_type::GREATER_EQUAL : token_type::GREATER);
                
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
        return token_type::IDENTIFIER;
    }
    auto identifier_type () -> token_type
    {
        switch (_start [0])
        {
            case 'a': return check_keyword (1, 2, "nd", token_type::AND);
            case 'c': return check_keyword (1, 4, "lass", token_type::CLASS);
            case 'e': return check_keyword (1, 3, "lse", token_type::ELSE);
            case 'f':
                  if (_current - _start > 1)
                  {
                    switch (_start [1])
                    {
                        case 'a': return check_keyword (2, 3, "lse", token_type::FALSE);
                        case 'o': return check_keyword (2, 1, "r", token_type::FOR);
                        case 'u': return check_keyword (2, 1, "n", token_type::FUN);
                    }
                  }
                  break;
            case 'i': return check_keyword (1, 1, "f", token_type::IF);
            case 'n': return check_keyword (1, 2, "il", token_type::NIL);
            case 'o': return check_keyword (1, 1, "r", token_type::OR);
            case 'p': return check_keyword (1, 4, "rint", token_type::PRINT);
            case 'r': return check_keyword (1, 5, "eturn", token_type::_RETURN);
            case 's': return check_keyword (1, 4, "uper", token_type::SUPER);
            case 't':
                  if (_current - _start > 1)
                  {
                    switch (_start [1])
                    {
                        case 'h': return check_keyword (2, 2, "is", token_type::THIS);
                        case 'r': return check_keyword (2, 2, "ue", token_type::TRUE);
                    }
                  }
                  break;
            case 'v': return check_keyword (1, 2, "ar", token_type::VAR);
            case 'w': return check_keyword (1, 4, "hile", token_type::WHILE);
        }
        
        return token_type::IDENTIFIER;
    }
    auto identifier() -> token
    {
        while (is_alpha (peek ()) or isdigit (peek ())) advance ();
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
        return make_token (token_type::NUMBER);
    }
    
    // consume characters until we reach the closing quote
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
        return make_token (token_type::STRING);
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
    // If the current character is the desired one, we advance and return true. Otherwise, we return false to indicate it wasn’t matched.
    auto match (char expected) -> bool
    {
        if (is_at_end ()) return false;
        if (*_current != expected) return false;
        ++_current;
        return true;
    }
    
    // consumes the current character and returns it.
    auto advance () -> char
    {
        ++_current;
        return _current [-1];
    }
    auto is_at_end () const -> bool
    {
        return *_current == '\0';
    }
    
    
    // It uses the scanner’s start and current pointers to capture the token’s lexeme.
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
            ._type = token_type::ERROR,
            ._start = message,
            ._length = (int) strlen (message),
            ._line = _line
        };
    }
};


