#pragma once
using namespace std;




/**
 constants is a series of instructions.
 */




template <typename __value_type, typename __index_type>
struct value_array
{
    using value_type = __value_type;
    using index_type = __index_type;
    
    value_type * m_values;
    
    index_type m_count;          // How many of the allocated elements are actually in use.
    index_type m_capacity;       // Number of elements in the array we have allocated.
    
    value_array () : m_count {0}, m_capacity {8}, m_values {(value_type*) malloc (sizeof (value_type) * 8)} {
//        if (constants == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto write_constant (value_type byte) -> void
    {
        
        if (full ())
        {
            cout << "FULL" << endl;
            transform (2 * m_capacity);
        }
        
        m_values [m_count] = byte;
        
//        ::new (constants + count) constant_type {byte};
        m_count++;
        
//        return count - 1;
    }
    
//    auto operator[] (int i) -> constant_type &
//    {
//        assert (i >= 0 and i < count);
//        return constants [i];
//    }
    
    auto begin () -> value_type*
    {
        return m_values;
    }
    
    auto end () -> value_type*
    {
        return m_values + m_count;
    }
    
    
    
    ~value_array () {
        free (m_values);
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
//        if (nr_of_bytes < 0)
//            throw runtime_error ("trying to transform into negative size");
        
//        if (nr_of_bytes == count)
//            throw runtime_error ("trying to transform into same size as is");
        
//        int old_capacity = exchange (capacity, nr_of_bytes);
        m_capacity = nr_of_bytes;
        
        m_values = (value_type*) realloc (m_values, m_capacity);
        
        if (m_values == nullptr)
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
