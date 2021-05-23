//#include "testlib.hpp"

#ifdef RUN_CATCH

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
#include <ph_compiler/arena.hpp>
#include <ph_compiler/allo.hpp>


using namespace std;




//using arena_type_opcode = arena <std::byte, alignof (std::byte), 1024>;
//using arena_type_constants = arena <std::byte, alignof (std::byte), 1024>;
//
//arena_type_opcode _opcode_arena;
//arena_type_constants _constants_arena;
//
//
//using allocator_opcode = allo <arena_type_opcode>;
//using allocator_constants = allo <arena_type_constants>;
//
//allocator_opcode _opcode_allocator {_opcode_arena};
//allocator_constants _constants_allocator {_constants_arena};





TEST_CASE("AAA")
{
    
    
    codefile <opcode, double> _codefile;
    
    
    auto constant_index = _codefile.add_constant (5);
    _codefile.write_opcode (opcode::CONSTANT);
    _codefile.write_opcode (constant_index);
    
    _codefile.write_opcode (opcode::NEGATE);
    
    _codefile.write_opcode (opcode::RETURN);
    
//    constant_index = _codefile.add_constant (4);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    constant_index = _codefile.add_constant (3);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    constant_index = _codefile.add_constant (2);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
    
//    constant_index = _codefile.add_constant (3);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
    
    
    
    
//    _codefile +=
    
    
//    _codefile += opcode::NEGATE;
    
//    constant = _codefile += 4;
////    cout << constant << endl;
//    _codefile += opcode::CONSTANT;
//    _codefile += constant;
//
//    _codefile += opcode::RETURN;
    
    
//
//
//    _codefile += opcode::NEGATE;
//
//    _codefile += 3;
//    _codefile += opcode::CONSTANT;
//
//
//    _codefile += opcode::NEGATE;
////    _codefile += 4;
//
////    _codefile += opcode::NEGATE;
////    _codefile += 4;
//    _codefile += opcode::RETURN;
    
    virtual_machine _vm {_codefile};
    _vm.run();
    
    
//    cout << disassembler {_codefile} << endl;
    
    
    
    
    
    
    
    
    

//    virtual_machine <>
    
    
    
    
    
    

//    codefile <version> f0;
//    f0.push_back (opcode::CONSTANT);
//    f0.push_back (4);
//    f0.push_back (opcode::RETURN);

    
    
//    virtual_machine <version> vm {f0};
    
    
    
    
    
//    b0 += instruction::RETURN;
//    b0 += instruction::RETURN;
//    b0 += instruction::CONSTANT;
//    b0 += 4;
//    cout << disassembler {f0} << endl;
//    vm.run <version> ();
}



#endif
