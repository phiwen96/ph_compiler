//#ifndef CODEFILE_HPP
//#define CODEFILE_HPP
#pragma once
#include "value_array.hpp"
//#include "opcode.hpp"
#include "version.hpp"
//#include "value.hpp"

using namespace std;



/**
 program is a series of instructions.
 */

//template <typename Byte, typename constant_type> requires (sizeof (Byte) == sizeof (uint8_t) and is_trivially_default_constructible_v <Byte> and is_trivially_destructible_v <Byte>)










//template <typename opcode_allo, typename constants_allo>
template <typename __code_type, typename __value_type, typename __index_type>
struct chunk
{
    using code_type = __code_type;
    using value_type = __value_type;
    using index_type = __index_type;
    
    code_type* m_code;
    
    value_array <value_type, index_type> m_constants;
    index_type m_count;          // How many of the allocated elements are actually in use.
    index_type m_capacity;       // Number of elements in the array we have allocated.

    
    chunk () : m_count {0}, m_capacity {8}, m_code {(code_type *) malloc (sizeof (m_code ) * 8)}, m_constants {} {
        if (m_code == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }
    auto write_opcode (code_type byte) -> void
    {
        if (full ())
        {
            transform (2 * m_capacity);
        }

        ::new (m_code + m_count) code_type  {byte};
        ++ m_count;
    }
    
    auto add_constant (value_type byte) -> index_type
    {
        m_constants.write_constant (byte);
        return static_cast <index_type> (m_constants.m_count - 1);
    }
    
//    auto operator += (double constant) -> auto&
//    {
//        consts += constant;
//        return *this;
//    }
    
    auto begin () -> auto *
    {
        return m_code;
    }
    
    auto end () -> auto *
    {
        return m_code + count;
    }
    
    
    
    ~chunk () {
        free (m_code);
    }
    
private:
    /**
     if we need to allocate more before copying value into code
     */
    auto full () const -> bool
    {
        return m_capacity < m_count + 1;
    }
    

    auto transform (int nr_of_bytes) -> bool
    {
        if (nr_of_bytes < 0)
            throw runtime_error ("trying to transform into negative size");
        
        if (nr_of_bytes == m_capacity)
            throw runtime_error ("trying to transform into same size as is");
        
        int old_capacity = exchange (m_capacity, nr_of_bytes);
        
        m_code = (code_type *) realloc (m_code, m_capacity);
        
        if (m_code == nullptr)
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
