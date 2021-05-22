#include <catch2/catch.hpp>
#include <ph_compiler/compiler.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/scanner.hpp>
#include <ph_compiler/bytecode.hpp>
#include <ph_compiler/operation_code.hpp>
using namespace std;

TEST_CASE("")
{
    version <1, 0, 0> v0;
    
    bytecode <operation_code> b0;
    b0 += operation_code::RETURN;
    
//    cout << sizeof (operation_code*) << endl;
//    cout << sizeof (bytecode <operation_code>) << endl;
    cout << alignof (int_fast8_t) << endl;
    cout << sizeof (int_fast8_t*) << endl;
    cout << sizeof (max_align_t) << endl;
    cout << INT_MAX << endl;
}
