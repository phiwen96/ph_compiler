#pragma once


using namespace std;


template <int major_, int minor_, int patch_>
struct version
{
#define int static constexpr int
    int major = major_;
    int minor = minor_;
    int patch = patch_;
#undef int
};





