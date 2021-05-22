#pragma once
#include "constant_pool.hpp"
#include "instruction.hpp"

using namespace std;


/**
 Bytecode is a series of instructions.
 */

//template <typename Byte, typename constant_type> requires (sizeof (Byte) == sizeof (uint8_t) and is_trivially_default_constructible_v <Byte> and is_trivially_destructible_v <Byte>)
struct bytecode
{
    using value_type = instruction;
    
    value_type * code;
    
    
    constant_pool consts;
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
    bytecode () : count {0}, capacity {8}, code {(value_type*) malloc (sizeof (value_type) * 8)} {
        if (code == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto operator += (value_type byte) -> auto& {
        
        if (not full ())
        {
            transform (2 * capacity);
        }
        
        ::new (code + count) value_type {byte};
        ++ count;
        
        return *this;
    }
    
    auto operator += (double constant) -> auto&
    {
        consts += constant;
        return *this;
    }
    
    auto begin () -> auto* {
        return code;
    }
    
    auto end () -> auto* {
        return code + count;
    }
    
    
    
    ~bytecode () {
        free (code);
    }
    
private:
    /**
     if we need to allocate more before copying value into code
     */
    auto full () const -> bool
    {
        return capacity < count + 1;
    }
    

    auto transform (int nr_of_bytes) -> bool
    {
        if (nr_of_bytes < 0)
            throw runtime_error ("trying to transform into negative size");
        
        if (nr_of_bytes == capacity)
            throw runtime_error ("trying to transform into same size as is");
        
        int old_capacity = exchange (capacity, nr_of_bytes);
        
        code = (value_type*) realloc (code, capacity);
        
        if (code == nullptr)
        {
            throw runtime_error ("failed to reallocate the requested block of memory");
        }
    }
    
    
};



/**
 auto * memory = malloc (sizeof (user));
 auto* user = ::new (memory) user ("philip");
 
 user -> ~user ();
 free (memory);
 */
