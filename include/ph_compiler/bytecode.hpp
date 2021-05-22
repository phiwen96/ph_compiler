#pragma once

using namespace std;


/**
 Bytecode is a series of instructions.
 */

template <typename Byte> requires (sizeof (Byte) == sizeof (uint8_t) and is_trivially_default_constructible_v <Byte> and is_trivially_destructible_v <Byte>)
struct bytecode 
{
    Byte * code;
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
    bytecode () : count {0}, capacity {8}, code {(Byte*) malloc (sizeof (Byte) * 8)} {
        if (code == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }
    
    
    auto operator += (Byte byte) -> auto& {
        
        if (capacity < count + 1)
        {
            int old_capacity = capacity;
            capacity *= 2;
            code = (Byte*) realloc (code, capacity);
            
            if (code == nullptr)
            {
                throw runtime_error ("failed to reallocate the requested block of memory");
            }
        }
        
        code [count] = byte;
        ++ count;
        
        return *this;
    }
    
    
    
    ~bytecode () {
        free (code);
    }
};



/**
 auto * memory = malloc (sizeof (user));
 auto* user = ::new (memory) user ("philip");
 
 user -> ~user ();
 free (memory);
 */
