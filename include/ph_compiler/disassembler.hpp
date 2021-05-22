#pragma once
#include "instruction.hpp"
#include "program.hpp"

using namespace std;



struct disassembler 
{
    program & code;

    
    
    friend ostream& operator<< (ostream& os, disassembler const& d)
    {
        for (auto const& i : d.code)
        {
//            os << i << endl;
            os << i << endl;
        }
        return os;
    }
};
