#pragma once
#include "version.hpp"
#include "scanner.hpp"


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
template <typename chunk>
struct compiler
{
    using opcode = typename chunk::opcode;
    using constant = typename chunk::constant;
    
    
    struct parser
    {
        token current;
        token previous;
        bool had_error;
        bool panic_mode;
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
        parse_fn _prefix;
        parse_fn _infix;
        precedence _precedence;
    };

    
    
//    using parse_fn = void (*) ();
    
    
    
    parse_rule  rules [50] {
        parse_rule {&compiler::grouping, NULL,   precedence::NONE}, //LEFT_PAREN
        parse_rule {NULL,     NULL,   precedence::NONE}, // RIGHT_PAREN
        parse_rule {NULL,     NULL,   precedence::NONE}, //LEFT_BRACE
        parse_rule {NULL,     NULL,   precedence::NONE}, // RIGHT_BRACE
        parse_rule {NULL,     NULL,   precedence::NONE}, // COMMA
        parse_rule {NULL,     NULL,   precedence::NONE}, // DOT
        parse_rule {&compiler::unary,    &compiler::binary, precedence::TERM}, // MINUS
        parse_rule {NULL,     &compiler::binary, precedence::TERM}, // PLUS
        parse_rule {NULL,     NULL,   precedence::NONE}, // SEMICOLON
        parse_rule {NULL,     &compiler::binary, precedence::FACTOR}, // SLASH
        parse_rule {NULL,     &compiler::binary, precedence::FACTOR}, // STAR
        parse_rule {NULL,     NULL,   precedence::NONE}, // BANG
        parse_rule {NULL,     NULL,   precedence::NONE}, // BANG_EQUAL
        parse_rule {NULL,     NULL,   precedence::NONE}, // EQUAL
        parse_rule {NULL,     NULL,   precedence::NONE}, // EQUAL_EQUAL
        parse_rule {NULL,     NULL,   precedence::NONE}, // GREATER
        parse_rule {NULL,     NULL,   precedence::NONE}, // GREATER_EQUAL
        parse_rule {NULL,     NULL,   precedence::NONE}, // LESS
        parse_rule {NULL,     NULL,   precedence::NONE}, // LESS_EQUAL
        parse_rule {NULL,     NULL,   precedence::NONE}, // IDENTIFIER
        parse_rule {NULL,     NULL,   precedence::NONE}, // STRING
        parse_rule {&compiler::number,   NULL,   precedence::NONE}, // NUMBER
        parse_rule {NULL,     NULL,   precedence::NONE}, // AND
        parse_rule {NULL,     NULL,   precedence::NONE}, // CLASS
        parse_rule {NULL,     NULL,   precedence::NONE}, // ELSE
        parse_rule {NULL,     NULL,   precedence::NONE}, // FALSE
        parse_rule {NULL,     NULL,   precedence::NONE}, // FOR
        parse_rule {NULL,     NULL,   precedence::NONE}, // FUN
        parse_rule {NULL,     NULL,   precedence::NONE}, // IF
        parse_rule {NULL,     NULL,   precedence::NONE}, // NIL
        parse_rule {NULL,     NULL,   precedence::NONE}, // OR
        parse_rule {NULL,     NULL,   precedence::NONE}, // PRINT
        parse_rule {NULL,     NULL,   precedence::NONE}, // RETURN
        parse_rule {NULL,     NULL,   precedence::NONE}, // SUPER
        parse_rule {NULL,     NULL,   precedence::NONE}, // THIS
        parse_rule {NULL,     NULL,   precedence::NONE}, // TRUE
        parse_rule {NULL,     NULL,   precedence::NONE}, // VAR
        parse_rule {NULL,     NULL,   precedence::NONE}, // WHILE
        parse_rule {NULL,     NULL,   precedence::NONE}, // ERROR
        parse_rule {NULL,     NULL,   precedence::NONE}, // _EOF
    };
    
    bool _compiled {false};
    
    parser _parser;
    
    chunk & _chunk;
    
    scanner _scanner;
    

    
    compiler (const char* source, chunk& c) : _chunk {c}, _scanner {source}
    {
        // init scanner
        
        
        
        _parser.had_error = false;
        _parser.panic_mode = false;
        
        
        advance ();
        expression ();
        consume (token_type::_EOF, "Expect end of expression.");
        end_compiler ();
        _compiled = !_parser.had_error;
    }
    
    operator bool () const
    {
        return !_parser.had_error;
    }
    
    
    
    
private:
    
    // starts at the current token and parses any expression at the given precedence level or higher
    auto parse_precedence (precedence p) -> void
    {
        advance ();
        parse_fn prefix_rule = get_rule (_parser.previous._type)->_prefix;
        if (prefix_rule == NULL)
        {
            error ("Expect expression.");
            return;
        }
        CALL_MEMBER_FN (*this, prefix_rule) ();
//        prefix_rule ();
        
        while (p <= get_rule (_parser.current._type) -> _precedence)
        {
            advance ();
            parse_fn infix_rule = get_rule (_parser.previous._type) -> _infix;
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
        token_type operatorType = _parser.previous._type;
        parse_rule* rule = get_rule (operatorType);
        parse_precedence ((precedence) (rule->_precedence + 1));
        
        switch (operatorType) {
            case token_type::PLUS:          emit_byte (opcode::ADD); break;
            case token_type::MINUS:         emit_byte (opcode::SUBTRACT); break;
            case token_type::STAR:          emit_byte (opcode::MULTIPLY); break;
            case token_type::SLASH:         emit_byte (opcode::DIVIDE); break;
            default: return; // Unreachable.
        }
    }
    
    auto number() -> void
    {
        constant value = strtod (_parser.previous._start, NULL);
        emit_constant (value);
    }
    
    auto emit_constant (constant value) -> void
    {
        emit_bytes (opcode::CONSTANT, make_constant (value));
    }
    
    opcode make_constant (constant value)
    {
        
        auto constant = _chunk.add_constant (value);
        
        if (constant > UINT8_MAX) {
            error ("Too many constants in one chunk.");
//            return 0;
        }
        
        return (opcode) constant;
    }
    
    void emit_return() {
        emit_byte (opcode::RETURN);
    }
    
    
    
    void error_at_current (const char* message)
    {
        error_at(&_parser.current, message);
    }
    
    void error (const char* message) {
        error_at(&_parser.previous, message);
    }
    
    void error_at (token* tok, const char* message) {
        if (_parser.panic_mode) return;
        _parser.panic_mode = true;
        fprintf(stderr, "[line %d] Error", tok->_line);
        
        if (tok->_type == token_type::_EOF) {
            fprintf(stderr, " at end");
        } else if (tok->_type == token_type::ERROR) {
            // Nothing.
        } else {
            fprintf(stderr, " at '%.*s'", tok->_length, tok->_start);
        }
        
        fprintf(stderr, ": %s\n", message);
        _parser.had_error = true;
    }
    
    void consume (token_type type, const char* message) {
        if (_parser.current._type == type) {
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
        consume (token_type::RIGHT_PAREN, "Expect ')' after expression.");
    }
    
    /*
     It writes the given byte, which may be an opcode or an operand to an instruction.
     */
    void emit_byte (opcode byte) {
        _chunk.write_opcode (byte);
        //        writeChunk(current_chunk (), byte, _parser.previous._line);
    }
    
    void emit_bytes (opcode byte1, opcode byte2) {
        emit_byte (byte1);
        emit_byte (byte2);
    }
    
    auto advance () -> void
    {
        _parser.previous = _parser.current;
        
        for (;;) {
            _parser.current = _scanner.scan_token ();
            if (_parser.current._type != token_type::ERROR) break;
            
            error_at_current(_parser.current._start);
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
        token_type operatorType = _parser.previous._type;
        
        // Compile the operand.
        expression ();
        
        parse_precedence (precedence::UNARY);
        
        // Emit the operator instruction.
        switch (operatorType)
        {
            case token_type::MINUS: emit_byte(opcode::NEGATE); break;
            default: return; // Unreachable.
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
