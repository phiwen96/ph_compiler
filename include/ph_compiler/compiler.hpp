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

template <typename _codefile_type>
struct compiler
{
    using codefile_type = _codefile_type;
    using opcode_type = typename codefile_type::opcode_type;
    using constant_type = typename codefile_type::constant_type;
    
    
    struct parser
    {
      token current;
      token previous;
        bool had_error;
        bool panic_mode;
    };
    
    parser _parser;
    
    codefile_type* _compiling_chunk;

    
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
    
    auto compile (const char* source, codefile_type* chunk) -> bool
    {
        scanner sc {source};
        
        _compiling_chunk = chunk;
        
        _parser.had_error = false;
        _parser.panic_mode = false;
        
        
        advance (sc);
        expression ();
          consume (sc, token_type::TOKEN_EOF, "Expect end of expression.");
        end_compiler ();
        return !_parser.had_error;
    }

    
    codefile_type* current_chunk() {
      return _compiling_chunk;
    }
    
    
    
    
    
private:
    
    void expression() {
      // What goes here?
    }
    
    void end_compiler () {
      emit_return ();
    }
    
    void number() {
        constant_type value = strtod (_parser.previous.start, NULL);
      emit_constant (value);
    }
    
    void emit_constant (constant_type value) {
      emit_bytes (codefile_type::OP_CONSTANT, make_constant (value));
    }
    
    uint_fast8_t make_constant (constant_type value) {
      int constant = addConstant(current_chunk(), value);
      if (constant > UINT8_MAX) {
        error ("Too many constants in one chunk.");
        return 0;
      }

      return (uint_fast8_t) constant;
    }
    
    void emit_return() {
        emit_byte (opcode_type::RETURN);
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
    
    void emit_byte (opcode_type byte) {
        current_chunk()->write_opcode (byte);
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
//    inline static interpret_result interpret (char const* source)
//    {
//        compile (source);
//        return
//    }
//
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
