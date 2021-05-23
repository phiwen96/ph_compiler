#pragma once


template <typename T>
struct elements
{
    
    using type = T;
    
    template <std::size_t N = alignof (T)>
    struct alignment
    {
        using value_type = std::size_t;
        inline static constexpr value_type value = N;
    };
    
    template <std::size_t N>
    struct size
    {
        using value_type = std::size_t;
        inline static constexpr value_type value = N;
    };
    
};
