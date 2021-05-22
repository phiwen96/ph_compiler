#pragma once
//#include "test.hpp"
#include <catch2/catch.hpp>
#include <ph_compiler/compiler.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/scanner.hpp>
#include <ph_compiler/codefile.hpp>
#include <ph_compiler/opcode.hpp>
#include <ph_compiler/disassembler.hpp>
#include <ph_compiler/value.hpp>
#include <ph_compiler/virtual_machine.hpp>
#include <ph_compiler/version.hpp>


using namespace std;






auto run () -> int 
{
    using m_version = version <2, 0, 0>;
    

    codefile <m_version> f0;
//    f0.push_back (opcode::CONSTANT);
//    f0.push_back (4);
//    f0.push_back (opcode::RETURN);

    
    
    virtual_machine <m_version> vm {f0};
    
    
    
    
    
//    b0 += instruction::RETURN;
//    b0 += instruction::RETURN;
//    b0 += instruction::CONSTANT;
//    b0 += 4;
//    cout << disassembler {f0} << endl;
    vm.run <m_version> ();
//    vm.run();
//    cout << instruction {1} << endl;
    
    
//    out (10, "hej", "kuk", "hora");
//    cout << endl;

    
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
