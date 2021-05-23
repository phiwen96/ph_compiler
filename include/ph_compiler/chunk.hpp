//#ifndef CODEFILE_HPP
//#define CODEFILE_HPP
#pragma once
#include "constant_pool.hpp"
//#include "opcode.hpp"
#include "version.hpp"

using namespace std;




/**
 program is a series of instructions.
 */

//template <typename Byte, typename constant_type> requires (sizeof (Byte) == sizeof (uint8_t) and is_trivially_default_constructible_v <Byte> and is_trivially_destructible_v <Byte>)










//template <typename opcode_allo, typename constants_allo>
template <typename _opcode_type, typename _constant_type>
struct chunk
{
    using opcode_type = _opcode_type;
    using constant_type = _constant_type;
    
//    using value_type = typename allo::value_type;
//    using instruction = instruction;
    
//    opcode_allo & _opcode_allocator;
    
//    constants_allo & _constants_allocator;
    
    
    opcode_type * lines;
    
    constant_pool <constant_type> constants;
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
//    alignas (alignment)
//    struct iterator
//    {
//        using iterator_category = std::contiguous_iterator_tag;
//        using difference_type   = std::ptrdiff_t;
//        using value_type        = opcode*;
//        using pointer           = int*;  // or also value_type*
//        using reference         = int&;  // or also value_type&
//    };
    
    chunk () : count {0}, capacity {8}, lines {(opcode_type *) malloc (sizeof (lines ) * 8)}, constants {} {
        if (lines == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto write_opcode (opcode_type byte) -> void
    {
        if (full ())
        {
            transform (2 * capacity);
        }

        ::new (lines + count) opcode_type  {byte};
        ++ count;
    }
    
    auto add_constant (constant_type byte) -> opcode_type
    {
        constants.write_constant (byte);
        return static_cast <opcode_type> (constants.count - 1);
    }
    
//    auto operator += (double constant) -> auto&
//    {
//        consts += constant;
//        return *this;
//    }
    
    auto begin () -> auto *
    {
        return lines;
    }
    
    auto end () -> auto *
    {
        return lines + count;
    }
    
    
    
    ~chunk () {
        free (lines);
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
        
        lines = (opcode_type *) realloc (lines, capacity);
        
        if (lines == nullptr)
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



//#endif
