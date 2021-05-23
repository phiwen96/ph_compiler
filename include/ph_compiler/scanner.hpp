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
    TOK_LEFT_PAREN, TOK_RIGHT_PAREN,
    TOK_LEFT_BRACE, TOK_RIGHT_BRACE,
    TOK_COMMA, TOK_DOT, TOK_MINUS, TOK_PLUS,
    TOK_SEMICOLON, TOK_SLASH, TOK_STAR,
    // One or two character tokens.
    TOK_BANG, TOK_BANG_EQUAL,
    TOK_EQUAL, TOK_EQUAL_EQUAL,
    TOK_GREATER, TOK_GREATER_EQUAL,
    TOK_LESS, TOK_LESS_EQUAL,
    // Literals.
    TOK_IDENTIFIER, TOK_STRING, TOK_NUMBER,
    // Keywords.
    TOK_AND, TOK_CLASS, TOK_ELSE, TOK_FALSE,
    TOK_FOR, TOK_FUN, TOK_IF, TOK_NIL, TOK_OR,
    TOK_PRINT, TOK_RETURN, TOK_SUPER, TOK_THIS,
    TOK_TRUE, TOK_VAR, TOK_WHILE,
    
    TOK_ERROR, TOK_EOF
};

struct token
{
    token_type m_type;
    char const* m_start;
    int m_length;
    int m_line;
};


struct scanner
{
    char const* m_start;
    char const* m_current;
    int m_line;
    
    scanner (char const* source) : m_start {source}, m_current {source}, m_line {1}
    {
        
    }
    
    auto scan_token () -> token
    {
        skip_whitespaces ();
        
        m_start = m_current;
        
        // check to see if we’ve reached the end of the source code
        if (is_at_end ())
        {
            return make_token (TOK_EOF);
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
            case '(': return make_token (TOK_LEFT_PAREN);
            case ')': return make_token (TOK_RIGHT_PAREN);
            case '{': return make_token (TOK_LEFT_BRACE);
            case '}': return make_token (TOK_RIGHT_BRACE);
            case ';': return make_token (TOK_SEMICOLON);
            case ',': return make_token (TOK_COMMA);
            case '.': return make_token (TOK_DOT);
            case '-': return make_token (TOK_MINUS);
            case '+': return make_token (TOK_PLUS);
            case '/': return make_token (TOK_SLASH);
            case '*': return make_token (TOK_STAR);
            case '!':
                return make_token (match ('=') ? TOK_BANG_EQUAL : TOK_BANG);
            case '=':
                return make_token (match ('=') ? TOK_EQUAL_EQUAL : TOK_EQUAL);
            case '<':
                return make_token (match ('=') ? TOK_LESS_EQUAL : TOK_LESS);
            case '>':
                return make_token (match ('=') ? TOK_GREATER_EQUAL : TOK_GREATER);
                
            case '"':
                return string ();
        }
        
        return error_token ("Unexpected character.");
    }
    
    
private:
    
    auto check_keyword (int start, int length, char const* rest, token_type type) -> token_type
    {
        if (m_current - m_start == start + length and memcmp (m_start + start, rest, length) == 0)
        {
            return type;
        }
        return TOK_IDENTIFIER;
    }
    auto identifier_type () -> token_type
    {
        switch (m_start [0])
        {
            case 'a': return check_keyword (1, 2, "nd", TOK_AND);
            case 'c': return check_keyword (1, 4, "lass", TOK_CLASS);
            case 'e': return check_keyword (1, 3, "lse", TOK_ELSE);
            case 'f':
                  if (m_current - m_start > 1)
                  {
                    switch (m_start [1])
                    {
                        case 'a': return check_keyword (2, 3, "lse", TOK_FALSE);
                        case 'o': return check_keyword (2, 1, "r", TOK_FOR);
                        case 'u': return check_keyword (2, 1, "n", TOK_FUN);
                    }
                  }
                  break;
            case 'i': return check_keyword (1, 1, "f", TOK_IF);
            case 'n': return check_keyword (1, 2, "il", TOK_NIL);
            case 'o': return check_keyword (1, 1, "r", TOK_OR);
            case 'p': return check_keyword (1, 4, "rint", TOK_PRINT);
            case 'r': return check_keyword (1, 5, "eturn", TOK_RETURN);
            case 's': return check_keyword (1, 4, "uper", TOK_SUPER);
            case 't':
                  if (m_current - m_start > 1)
                  {
                    switch (m_start [1])
                    {
                        case 'h': return check_keyword (2, 2, "is", TOK_THIS);
                        case 'r': return check_keyword (2, 2, "ue", TOK_TRUE);
                    }
                  }
                  break;
            case 'v': return check_keyword (1, 2, "ar", TOK_VAR);
            case 'w': return check_keyword (1, 4, "hile", TOK_WHILE);
        }
        
        return token_type::TOK_IDENTIFIER;
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
        return make_token (TOK_NUMBER);
    }
    
    // consume characters until we reach the closing quote
    auto string () -> token
    {
        while (peek () != '"' and ! is_at_end ())
        {
            if (peek () == '\n')
            {
                ++m_line;
                advance ();
            }
        }
        
        if (is_at_end ())
        {
            return error_token ("Unterminated string.");
        }
        
        // The closing quote.
        advance ();
        return make_token (TOK_STRING);
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
                    ++m_line;
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
        return m_current [1];
    }
    
    auto peek () -> char
    {
        return *m_current;
    }
    // If the current character is the desired one, we advance and return true. Otherwise, we return false to indicate it wasn’t matched.
    auto match (char expected) -> bool
    {
        if (is_at_end ()) return false;
        if (*m_current != expected) return false;
        ++m_current;
        return true;
    }
    
    // consumes the current character and returns it.
    auto advance () -> char
    {
        ++m_current;
        return m_current [-1];
    }
    auto is_at_end () const -> bool
    {
        return *m_current == '\0';
    }
    
    
    // It uses the scanner’s start and current pointers to capture the token’s lexeme.
    inline auto make_token (token_type type) -> token
    {
        return token
        {
            .m_type = type,
            .m_start = m_start,
            .m_length = (int) (m_current - m_start),
            .m_line = m_line
        };
    }
    
    auto error_token (const char* message) -> token
    {
        return
        {
            .m_type = TOK_ERROR,
            .m_start = message,
            .m_length = (int) strlen (message),
            .m_line = m_line
        };
    }
};


