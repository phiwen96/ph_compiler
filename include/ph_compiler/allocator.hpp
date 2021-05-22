#pragma once
using namespace std;

template <typename version, typename T, size_t N>
struct allo;

#include "version.hpp"
#include "arena.hpp"


template <typename T, size_t N>
struct allo <version <1, 0, 0>, T, N>
{
    using value_type = T;
    
};
