#pragma once
#include "operation_code.hpp"
#include "bytecode.hpp"

using namespace std;



struct disassembler 
{
    bytecode <operation_code> & code;

    
    
    friend ostream& operator<< (ostream& os, disassembler const& d)
    {
        for (auto const& i : d.code)
        {
            os << i << endl;
        }
        return os;
    }
};
