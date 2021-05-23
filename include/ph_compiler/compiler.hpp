#pragma once
#include "version.hpp"
#include "scanner.hpp"
#include "value.hpp"


#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

using namespace std;


template <typename compiler>
concept is_compiler = requires (compiler& c)
{
    {c.type};
};






#define TEST_FILE_0


/**
 The compiler will take the user’s program and fill up the chunk with bytecode
 */
template <typename __chunk_type>
struct compiler
{
    using chunk_type = __chunk_type;
    using code_type = typename chunk_type::code_type;
    using value_type = typename chunk_type::value_type;
    using index_type = typename chunk_type::index_type;
    
    
    struct parser
    {
        token m_current;
        token m_previous;
        bool m_had_error;
        bool m_panic_mode;
    };
    
    // These are all of Lox’s precedence levels in order from lowest to highest
    enum precedence
    {
        // lowest
        NONE,
        ASSIGNMENT,  // =
        OR,          // or
        AND,         // and
        EQUALITY,    // == !=
        COMPARISON,  // < > <= >=
        TERM,        // + -
        FACTOR,      // * /
        UNARY,       // ! -
        CALL,        // . ()
        PRIMARY
        // higher
    };
    
    using parse_fn = void (compiler::*) ();

    struct parse_rule
    {
        parse_fn m_prefix;
        parse_fn m_infix;
        precedence m_precedence;
    };

    
    
//    using parse_fn = void (*) ();
    
    
    
    parse_rule  rules [50] {
         {&compiler::grouping, NULL,   precedence::NONE}, //LEFT_PAREN
         {NULL,     NULL,   precedence::NONE}, // RIGHT_PAREN
         {NULL,     NULL,   precedence::NONE}, //LEFT_BRACE
         {NULL,     NULL,   precedence::NONE}, // RIGHT_BRACE
         {NULL,     NULL,   precedence::NONE}, // COMMA
         {NULL,     NULL,   precedence::NONE}, // DOT
         {&compiler::unary,    &compiler::binary, precedence::TERM}, // MINUS
         {NULL,     &compiler::binary, precedence::TERM}, // PLUS
         {NULL,     NULL,   precedence::NONE}, // SEMICOLON
         {NULL,     &compiler::binary, precedence::FACTOR}, // SLASH
         {NULL,     &compiler::binary, precedence::FACTOR}, // STAR
         {NULL,     NULL,   precedence::NONE}, // BANG
         {NULL,     NULL,   precedence::NONE}, // BANG_EQUAL
         {NULL,     NULL,   precedence::NONE}, // EQUAL
         {NULL,     NULL,   precedence::NONE}, // EQUAL_EQUAL
         {NULL,     NULL,   precedence::NONE}, // GREATER
         {NULL,     NULL,   precedence::NONE}, // GREATER_EQUAL
         {NULL,     NULL,   precedence::NONE}, // LESS
         {NULL,     NULL,   precedence::NONE}, // LESS_EQUAL
         {NULL,     NULL,   precedence::NONE}, // IDENTIFIER
         {NULL,     NULL,   precedence::NONE}, // STRING
         {&compiler::number,   NULL,   precedence::NONE}, // NUMBER
         {NULL,     NULL,   precedence::NONE}, // AND
         {NULL,     NULL,   precedence::NONE}, // CLASS
         {NULL,     NULL,   precedence::NONE}, // ELSE
         {&compiler::literal,     NULL,   precedence::NONE}, // FALSE
         {NULL,     NULL,   precedence::NONE}, // FOR
         {NULL,     NULL,   precedence::NONE}, // FUN
         {NULL,     NULL,   precedence::NONE}, // IF
         {&compiler::literal,     NULL,   precedence::NONE}, // NIL
         {NULL,     NULL,   precedence::NONE}, // OR
         {NULL,     NULL,   precedence::NONE}, // PRINT
         {NULL,     NULL,   precedence::NONE}, // RETURN
         {NULL,     NULL,   precedence::NONE}, // SUPER
         {NULL,     NULL,   precedence::NONE}, // THIS
         {&compiler::literal,     NULL,   precedence::NONE}, // TRUE
         {NULL,     NULL,   precedence::NONE}, // VAR
         {NULL,     NULL,   precedence::NONE}, // WHILE
         {NULL,     NULL,   precedence::NONE}, // ERROR
         {NULL,     NULL,   precedence::NONE}, // _EOF
    };
    
    auto literal() -> void
    {
      switch (m_parser.m_previous.m_type)
      {
          case token_type::TOK_FALSE: emit_byte (code_type::FALSE); break;
          case token_type::TOK_NIL: emit_byte (code_type::NIL); break;
          case token_type::TOK_TRUE: emit_byte (code_type::TRUE); break;
        default: return; // Unreachable.
      }
    }
    
    bool m_compiled {false};
    
    parser m_parser;
    
    chunk_type & m_chunk;
    
    scanner m_scanner;
    

    
    compiler (const char* source, chunk_type& c) : m_chunk {c}, m_scanner {source}
    {
        // init scanner
        
        
        
        m_parser.m_had_error = false;
        m_parser.m_panic_mode = false;
        
        
        advance ();
        expression ();
        consume (token_type::TOK_EOF, "Expect end of expression.");
        end_compiler ();
        m_compiled = !m_parser.m_had_error;
    }
    
    operator bool () const
    {
        return !m_parser.m_had_error;
    }
    
    
    
    
private:
    
    // starts at the current token and parses any expression at the given precedence level or higher
    auto parse_precedence (precedence p) -> void
    {
        advance ();
        parse_fn prefix_rule = get_rule (m_parser.m_previous.m_type)->m_prefix;
        if (prefix_rule == NULL)
        {
            error ("Expect expression.");
            return;
        }
        CALL_MEMBER_FN (*this, prefix_rule) ();
//        prefix_rule ();
        
        while (p <= get_rule (m_parser.m_current.m_type) -> m_precedence)
        {
            advance ();
            parse_fn infix_rule = get_rule (m_parser.m_previous.m_type) -> m_infix;
            CALL_MEMBER_FN (*this, infix_rule) ();
        }
    }
    
    auto get_rule (token_type type) -> parse_rule *
    {
        return &rules [type];
    }
    
    auto expression () -> void
    {
        parse_precedence (precedence::ASSIGNMENT);
        // What goes here?
        
        
    }
    
    void end_compiler () {
        emit_return ();
    }
    
    auto binary () -> void
    {
        token_type operatorType = m_parser.m_previous.m_type;
        parse_rule* rule = get_rule (operatorType);
        parse_precedence ((precedence) (rule->m_precedence + 1));
        
        switch (operatorType) {
            case token_type::TOK_PLUS:          emit_byte (code_type::ADD); break;
            case token_type::TOK_MINUS:         emit_byte (code_type::SUBTRACT); break;
            case token_type::TOK_STAR:          emit_byte (code_type::MULTIPLY); break;
            case token_type::TOK_SLASH:         emit_byte (code_type::DIVIDE); break;
            default: return; // Unreachable.
        }
    }
    
    auto number () -> void
    {
        double _value = strtod (m_parser.m_previous.m_start, NULL);
//        value val = NUMBER_VAL (_value);
        emit_constant (NUMBER_VAL(_value));
    }
    
    auto emit_constant (value_type val) -> void
    {
        emit_bytes (code_type::CONSTANT, make_constant (val));
    }
    
    index_type make_constant (value_type val)
    {
        
        index_type constant_index = m_chunk.add_constant (val);
        
        if (constant_index > UINT8_MAX) {
            error ("Too many constants in one chunk.");
//            return 0;
        }
        
        return constant_index;
    }
    
    void emit_return() {
        emit_byte (code_type::RETURN);
    }
    
    
    
    void error_at_current (const char* message)
    {
        error_at(&m_parser.m_current, message);
    }
    
    void error (const char* message) {
        error_at(&m_parser.m_previous, message);
    }
    
    void error_at (token* tok, const char* message) {
        if (m_parser.m_panic_mode) return;
        m_parser.m_panic_mode = true;
        fprintf(stderr, "[line %d] Error", tok->m_line);
        
        if (tok->m_type == token_type::TOK_EOF) {
            fprintf(stderr, " at end");
        } else if (tok->m_type == token_type::TOK_ERROR) {
            // Nothing.
        } else {
            fprintf(stderr, " at '%.*s'", tok->m_length, tok->m_start);
        }
        
        fprintf(stderr, ": %s\n", message);
        m_parser.m_had_error = true;
    }
    
    void consume (token_type type, const char* message) {
        if (m_parser.m_current.m_type == type) {
            advance ();
            return;
        }
        
        error_at_current (message);
    }
    
    auto grouping () -> void
    {
        expression ();
        // We recursively call back into expression()
        // to compile the expression between the parentheses,
        // then parse the closing ) at the end.
        consume (token_type::TOK_RIGHT_PAREN, "Expect ')' after expression.");
    }
    
    /*
     It writes the given byte, which may be an opcode or an operand to an instruction.
     */
    void emit_byte (auto b) {
//        m_chunk.write_opcode (b);
        //        writeChunk(current_chunk (), byte, _parser.previous._line);
    }
    
    void emit_bytes (auto byte1, auto byte2) {
        emit_byte (byte1);
        emit_byte (byte2);
    }
    
    auto advance () -> void
    {
        m_parser.m_previous = m_parser.m_current;
        
        for (;;) {
            m_parser.m_current = m_scanner.scan_token ();
            if (m_parser.m_current.m_type != token_type::TOK_ERROR) break;
            
            error_at_current(m_parser.m_current.m_start);
        }
    }
    
    inline static auto run_file (char const* path) -> void
    {
        char* source = read_file (path);
        //        interpret_result result =
    }
    //      inline static interpret_result interpret (char const* source)
    //              {
    //                  compile (source);
    //        return
    //    }
    //
    
    
    /**
     The leading - token has been consumed and is sitting in parser.previous. We grab the token type from that to note which unary operator we’re dealing with.
     */
    auto unary () -> void
    {
        token_type operatorType = m_parser.m_previous.m_type;
        
        // Compile the operand.
        expression ();
        
        parse_precedence (precedence::UNARY);
        
        // Emit the operator instruction.
        switch (operatorType)
        {
            case token_type::TOK_MINUS:
                emit_byte (code_type::NEGATE);
                break;
                
            default:
                return; // Unreachable.
        }
    }
    
    
    
    
    inline static char* read_file (const char* path) {
        FILE* file = fopen (path, "rb");
        
        if (file == NULL) {
            fprintf(stderr, "Could not open file \"%s\".\n", path);
            exit(74);
        }
        
        fseek (file, 0L, SEEK_END);
        size_t fileSize = ftell (file);
        rewind (file);
        
        char* buffer = (char*) malloc (fileSize + 1);
        
        if (buffer == NULL) {
            fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
            exit(74);
        }
        
        size_t bytesRead = fread (buffer, sizeof (char), fileSize, file);
        
        if (bytesRead < fileSize) {
            fprintf(stderr, "Could not read file \"%s\".\n", path);
            exit(74);
        }
        
        
        buffer [bytesRead] = '\0';
        
        fclose (file);
        return buffer;
    }
};
