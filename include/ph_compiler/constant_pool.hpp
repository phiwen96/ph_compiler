#pragma once
#include "version.hpp"
using namespace std;



template <typename Version>
struct constant_pool;

/**
 constants is a series of instructions.
 */

template <>
struct constant_pool <version <1, 0, 0>>
{
    using value_type = double;
    
    double * values;
    
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
    constant_pool () : count {0}, capacity {8}, values {(value_type*) malloc (sizeof (value_type) * 8)} {
        if (values == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto operator += (value_type byte) -> auto& {
        
        if (not full ())
        {
            transform (2 * capacity);
        }
        
        ::new (values + count) value_type {byte};
        ++ count;
        
        return *this;
    }
    
    auto operator[] (int i) -> double &
    {
        assert (i >= 0 and i < count);
        return values [i];
    }
    
    auto begin () -> double*
    {
        return values;
    }
    
    auto end () -> double*
    {
        return values + count;
    }
    
    
    
    ~constant_pool () {
        free (values);
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
        
        values = (value_type*) realloc (values, capacity);
        
        if (values == nullptr)
        {
            throw runtime_error ("failed to reallocate the requested block of memory");
        }
    }
};


template <>
struct constant_pool <version <2, 0, 0>>
{
    using value_type = double;
    
    double * values;
    
    int count;          // How many of the allocated elements are actually in use.
    int capacity;       // Number of elements in the array we have allocated.
    
    constant_pool () : count {0}, capacity {8}, values {(value_type*) malloc (sizeof (value_type) * 8)} {
        if (values == nullptr) throw runtime_error ("failed to allocate the requested block of memory");
    }

    
    auto operator += (value_type byte) -> auto& {
        
        if (not full ())
        {
            transform (2 * capacity);
        }
        
        ::new (values + count) value_type {byte};
        ++ count;
        
        return *this;
    }
    
    auto operator[] (int i) -> double &
    {
        assert (i >= 0 and i < count);
        return values [i];
    }
    
    auto begin () -> double*
    {
        return values;
    }
    
    auto end () -> double*
    {
        return values + count;
    }
    
    
    
    ~constant_pool () {
        free (values);
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
        
        values = (value_type*) realloc (values, capacity);
        
        if (values == nullptr)
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
