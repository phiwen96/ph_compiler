#pragma once

template <typename version, auto N>
struct arena;

#include "version.hpp"

template <auto N>
struct arena <version <1, 0, 0>, N>
{
    using size_type = decltype (N);
    static constexpr size_type size_value = N;
    static constexpr auto alignment = alignof (std::max_align_t);
    
    
public:
    auto reset ()
    {
        _ptr = _buffer;
    }
    
    static constexpr auto size () -> size_type
    {
        return size_value;
    }
    
    auto used () const -> size_type
    {
        return static_cast <size_type> (_ptr - _buffer);
    }
    
    auto allocate (size_type n) -> std::byte *
    {
        size_type const aligned_n = align_up (n);
    }
    
    auto deallocate (std::byte * p, size_type n) -> void
    {
        
    }
    
    auto align_up (size_type n) const -> size_type
    {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }
    
    auto pointer_in_buffer (std::byte * p) const -> bool
    {
        return std::uintptr_t (p) >= std::uintptr_t (_buffer) and
        std::uintptr_t (p) < std::uintptr_t (_buffer) + size_value;
    }
    
    
    alignas (alignment) std::byte _buffer [size_value];
    std::byte * _ptr {_buffer};
};
