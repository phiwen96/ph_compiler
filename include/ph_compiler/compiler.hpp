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

struct compiler
{
    
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
    
    
    
    
    
private:
    
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
