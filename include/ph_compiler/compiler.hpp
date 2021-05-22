#pragma once
#include "version.hpp"

using namespace std;


template <typename compiler>
concept is_compiler = requires (compiler& c)
{
    {c.type};
};




template <typename version>
struct compiler
{
    
};

