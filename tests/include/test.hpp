#pragma once
#include "test.hpp"
#include <catch2/catch.hpp>
#include <ph_compiler/compiler.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/scanner.hpp>
#include <ph_compiler/program.hpp>
#include <ph_compiler/instruction.hpp>
#include <ph_compiler/disassembler.hpp>
#include <ph_compiler/value.hpp>

using namespace std;








auto run () -> int 
{
    program b0;
//    b0 += instruction::RETURN {};
//    b0 += instruction::RETURN;
//    b0 += instruction::CONSTANT;
//    b0 += 4;
//    b0 += instruction::RETURN;
    cout << disassembler {b0} << endl;
//    cout << instruction {1} << endl;
    return 0;
    
    
    //    cout << sizeof (operation_code*) << endl;
    //    cout << sizeof (bytecode <operation_code>) << endl;
    cout << alignof (int_fast8_t) << endl;
    cout << sizeof (int_fast8_t*) << endl;
    cout << sizeof (max_align_t) << endl;
    cout << INT_MAX << endl;
    cout << sizeof (double) << endl;
    cout << "run" << endl << "====================" << endl;
    return 0;
}
