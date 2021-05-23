#pragma once

/**
 1) a block of contiguous memory
 2) a strategy for handing out parts of that memory and reclaiming it later on.
 */
template <typename T, std::size_t alignment, std::size_t N>

struct arena
{
    
    using value_type = T;

    auto reset ()
    {
        _ptr = _buffer;
    }
    
    static constexpr auto size () -> std::size_t
    {
        return N;
    }
    
    auto used () const -> std::size_t
    {
        return static_cast <std::size_t> (_ptr - _buffer);
    }
    
    auto allocate (std::size_t n) -> T *
    {
        if (std::size_t aligned = align_up (n);
            available_bytes () >= aligned)
        {
            return std::exchange (_ptr, _ptr + aligned);

        } else
        {
            return static_cast <T*> (::operator new (n));
        }
        
    }
    
    auto deallocate (T * p, std::size_t n) -> void
    {
        if (pointer_in_buffer (p))
        {
            n = align_up (n);
            if (p + n == _ptr)
            {
                _ptr = p;
            }
            
        } else
        {
            ::operator delete (p);
        }
    }
    
    auto align_up (std::size_t n) const -> auto
    {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }
    
    auto pointer_in_buffer (std::byte * p) const -> bool
    {
        return std::uintptr_t (p) >= std::uintptr_t (_buffer) and
        std::uintptr_t (p) < std::uintptr_t (_buffer) + N;
    }
    
    auto available_bytes () const -> std::size_t
    {
        return _buffer + N - _ptr;
    }
    
    
    alignas (alignment) T _buffer [N];
    T * _ptr {_buffer};
};


