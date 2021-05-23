#pragma once
#include "version.hpp"
#include "scanner.hpp"


using namespace std;


template <typename compiler>
concept is_compiler = requires (compiler& c)
{
    {c.type};
};









#define TEST_FILE_0

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
    
    bool _compiled {false};
    
    parser _parser;
    
    chunk & _chunk;
    
    
    
    
    auto compile (char const* source) -> void
    {
        scanner sc {source};
        
        
        int line = -1;
        
        for (;;)
        {
            token _token =  sc.scan_token ();
            
            if (_token._line == line)
            {
                line = _token._line;
            }
            
            if (_token._type == token_type::TOKEN_EOF) break;
        }
    }
    
    compiler (const char* source, chunk& c) : _chunk {c}
    {
        scanner sc {source};
        
        
        _parser.had_error = false;
        _parser.panic_mode = false;
        
        
        advance (sc);
        expression ();
        consume (sc, token_type::TOKEN_EOF, "Expect end of expression.");
        end_compiler ();
        _compiled = !_parser.had_error;
    }
    
//    auto compile (const char* source, codefile_type* chunk) -> bool
//    {
//        scanner sc {source};
//
//        _compiling_chunk = chunk;
//
//        _parser.had_error = false;
//        _parser.panic_mode = false;
//
//
//        advance (sc);
//        expression ();
//        consume (sc, token_type::TOKEN_EOF, "Expect end of expression.");
//        end_compiler ();
//        return !_parser.had_error;
//    }
    
    
    operator bool () const
    {
        return !_parser.had_error;
    }
    
    
    
    
private:
    
    void expression() {
        // What goes here?
    }
    
    void end_compiler () {
        emit_return ();
    }
    
    auto number() -> void
    {
        constant value = strtod (_parser.previous.start, NULL);
        emit_constant (value);
    }
    
    auto emit_constant (constant value) -> void
    {
        emit_bytes (opcode::OP_CONSTANT, make_constant (value));
    }
    
    uint_fast8_t make_constant (constant value)
    {
        int constant = addConstant(_chunk, value);
        
        if (constant > UINT8_MAX) {
            error ("Too many constants in one chunk.");
            return 0;
        }
        
        return (uint_fast8_t) constant;
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
        
        if (tok->_type == token_type::TOKEN_EOF) {
            fprintf(stderr, " at end");
        } else if (tok->_type == token_type::TOKEN_ERROR) {
            // Nothing.
        } else {
            fprintf(stderr, " at '%.*s'", tok->_length, tok->_start);
        }
        
        fprintf(stderr, ": %s\n", message);
        _parser.had_error = true;
    }
    
    void consume (scanner& sc, token_type type, const char* message) {
        if (_parser.current._type == type) {
            advance (sc);
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
        consume (token_type::TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
    }
    
    /*
     It writes the given byte, which may be an opcode or an operand to an instruction.
     */
    void emit_byte (opcode byte) {
        _chunk.write_opcode (byte);
        //        writeChunk(current_chunk (), byte, _parser.previous._line);
    }
    
    void emit_bytes(uint_fast8_t byte1, uint_fast8_t byte2) {
        emit_byte (byte1);
        emit_byte (byte2);
    }
    
    auto advance (scanner& sc) -> void
    {
        _parser.previous = _parser.current;
        
        for (;;) {
            _parser.current = sc.scan_token ();
            if (_parser.current._type != token_type::TOKEN_ERROR) break;
            
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
        
        // Emit the operator instruction.
        switch (operatorType)
        {
            case token_type::TOKEN_MINUS: emit_byte(opcode::NEGATE); break;
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
