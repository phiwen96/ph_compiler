#pragma once

template <typename T>
struct alignment
{
    using value_type = std::size_t;
    inline static constexpr value_type value = alignof (value_type);

    

    // align up
    inline static constexpr auto align_up (value_type n) -> value_type
    {
        return (n + (value - 1)) & ~(value - 1);
    }
};



