#pragma once
#include "version.hpp"


using namespace std;


template <typename compiler>
concept is_compiler = requires (compiler& c)
{
    {c.type};
};






struct interpret_result
{
    
};



struct compiler
{
    
    compiler (char const* path)
    {
        char* source = readFile (path);
//        interpret_result result =
    }
    
    
    
    
private:
//    inline static interpret_result interpret (char const* source)
//    {
//        compile (source);
//        return
//    }
//
    inline static char* readFile (const char* path) {
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
