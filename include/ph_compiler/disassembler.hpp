#pragma once
#include "opcode.hpp"
#include "codefile.hpp"
#include "version.hpp"

using namespace std;


template <typename Version>
struct disassembler;


template <>
struct disassembler <version <1, 0, 0>>
{
    codefile <version <1, 0, 0>> & code;

    
    
    friend ostream& operator<< (ostream& os, disassembler const& d)
    {
        for (auto const& i : d.code)
        {
            os << i << endl;
//            os << i << endl;
        }
        return os;
    }
};
