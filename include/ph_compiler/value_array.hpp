#pragma once
using namespace std;




/**
 constants is a series of instructions.
 */




template <typename T>
struct value_array
{
    using constant_type = T;
    
    constant_type * constants;
    
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
    value_array () : count {0}, capacity {8}, constants {(constant_type*) malloc (sizeof (constant_type) * 8)} {
//        if (constants == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto write_constant (constant_type byte) -> void
    {
        
        if (full ())
        {
            cout << "FULL" << endl;
            transform (2 * capacity);
        }
        
        constants [count] = byte;
        
//        ::new (constants + count) constant_type {byte};
        count++;
        
//        return count - 1;
    }
    
//    auto operator[] (int i) -> constant_type &
//    {
//        assert (i >= 0 and i < count);
//        return constants [i];
//    }
    
    auto begin () -> constant_type*
    {
        return constants;
    }
    
    auto end () -> constant_type*
    {
        return constants + count;
    }
    
    
    
    ~value_array () {
        free (constants);
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
//        if (nr_of_bytes < 0)
//            throw runtime_error ("trying to transform into negative size");
        
//        if (nr_of_bytes == count)
//            throw runtime_error ("trying to transform into same size as is");
        
//        int old_capacity = exchange (capacity, nr_of_bytes);
        capacity = nr_of_bytes;
        
        constants = (constant_type*) realloc (constants, capacity);
        
        if (constants == nullptr)
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
