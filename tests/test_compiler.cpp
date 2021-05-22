#include <catch2/catch.hpp>
#include <ph_compiler/compiler.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/scanner.hpp>
#include <ph_compiler/bytecode.hpp>
#include <ph_compiler/instruction.hpp>
#include <ph_compiler/disassembler.hpp>
#include <ph_compiler/value.hpp>

using namespace std;






TEST_CASE("")
{
    
    
    version <1, 0, 0> v0;
    
    
    
    bytecode b0;
    b0 += instruction::RETURN;
    b0 += instruction::RETURN;
    b0 += instruction::CONSTANT;
    b0 += 4;
    b0 += instruction::RETURN;
    cout << disassembler {b0} << endl;
    
    
    //    cout << sizeof (operation_code*) << endl;
    //    cout << sizeof (bytecode <operation_code>) << endl;
    cout << alignof (int_fast8_t) << endl;
    cout << sizeof (int_fast8_t*) << endl;
    cout << sizeof (max_align_t) << endl;
    cout << INT_MAX << endl;
    cout << sizeof (double) << endl;
}



