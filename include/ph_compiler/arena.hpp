#pragma once
using namespace std;

template <typename version>
struct arena;

#include "version.hpp"

template <>
struct arena <version <1, 0, 0>>
{
    
};
