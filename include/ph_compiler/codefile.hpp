#pragma once
#include "constant_pool.hpp"
#include "opcode.hpp"
#include "version.hpp"

using namespace std;



enum struct codefile_versions
{
    first,
    second
};

/**
 program is a series of instructions.
 */

//template <typename Byte, typename constant_type> requires (sizeof (Byte) == sizeof (uint8_t) and is_trivially_default_constructible_v <Byte> and is_trivially_destructible_v <Byte>)

template <typename version>
struct codefile;








template <>
struct codefile <version <1, 0, 0>>
{
    using version = version <1, 0, 0>;
//    using instruction = instruction;
    
    opcode * lines;
    
    constant_pool <version> constants;
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
    
    codefile () : count {0}, capacity {8}, lines {(opcode *) malloc (sizeof (lines ) * 8)} {
        if (lines == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto push_back (opcode byte) -> auto &
    {
        if (not full ())
        {
            transform (2 * capacity);
        }
        
        ::new (lines + count) opcode  {byte};
        ++ count;
        
        return *this;
    }
    
    auto push_back (double byte) -> auto &
    {
        constants += byte;
        return *this;
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
    
    
    
    ~codefile () {
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
        
        lines = (opcode *) realloc (lines, capacity);
        
        if (lines == nullptr)
        {
            throw runtime_error ("failed to reallocate the requested block of memory");
        }
    }
    
    
};

template <>
struct codefile <version <2, 0, 0>>
{
    using version = version <2, 0, 0>;
//    using instruction = instruction;
    
    opcode * lines;
    
    constant_pool <version> constants;
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
    
    codefile () : count {0}, capacity {8}, lines {(opcode *) malloc (sizeof (lines ) * 8)} {
        if (lines == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto push_back (opcode byte) -> auto &
    {
        if (not full ())
        {
            transform (2 * capacity);
        }
        
        ::new (lines + count) opcode  {byte};
        ++ count;
        
        return *this;
    }
    
    auto push_back (double byte) -> auto &
    {
        constants += byte;
        return *this;
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
    
    
    
    ~codefile () {
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
        
        lines = (opcode *) realloc (lines, capacity);
        
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



