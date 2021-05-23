#pragma once
//#include "codefile.hpp"
#include "opcode.hpp"
#include "version.hpp"
#include "compiler.hpp"

//using version = version <1, 0, 0>;


using namespace std;

enum struct interpret_result
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};







#define MAX_STACK 256

template <typename codefile>
struct virtual_machine
{
    using self = virtual_machine;
    using codefile_type = codefile;
    using opcode_type = typename codefile_type::opcode_type;
    using constant_type = typename codefile_type::constant_type;
    
    
    
    
    codefile_type * _code_file;
    opcode_type * _current_opcode;
    
    constant_type _stack [MAX_STACK];
    constant_type * _stack_top;
    
    interpret_result _result;
    
    
    auto reset_stack () -> void
    {
        _stack_top = _stack;
    }
    
    auto push (constant_type c) -> void
    {
        *_stack_top = c;
        _stack_top++;
    }
    
    auto pop () -> constant_type&
    {
        _stack_top--;
        return *_stack_top;
    }
    auto top () const -> constant_type&
    {
        return * (_stack_top - 1);
    }

    
    operator interpret_result () {
        return _result;
    }
    
    virtual_machine (codefile & code_file) : _code_file {&code_file}
    {
        _current_opcode = _code_file->begin();
        _stack_top = _stack;
    }
    
    virtual_machine () //: _code_file {code_file}
    {
//        'compiler c {source};
//        _result = interpret_result::INTERPRET_OK;'
    }
    
    
    
    auto interpret (char const* source) -> interpret_result
    {
        
        
        compiler <codefile_type> comp {};
//        compile (source);
//        return interpret_result::INTERPRET_OK;
        
        codefile_type code_file;
        
        if (!comp.compile (source, &code_file))
        {
            return interpret_result::INTERPRET_COMPILE_ERROR;
        }
        
        _code_file = &code_file;
        _current_opcode = _code_file -> lines;
        
        interpret_result result = run ();
        return result;
    }
    
   
    
    auto run () -> interpret_result
    {
#define READ_BYTE (*_current_opcode++)
#define READ_CONSTANT (_code_file -> constants.constants [READ_BYTE])
        for (;;)
        {
            uint_fast8_t instruction;

            switch (instruction = READ_BYTE)
            {
                case opcode::RETURN:
                {
                    cout << "return " << top () << endl;
                    pop ();
                    return interpret_result::INTERPRET_OK;
                }
                    
                case opcode::CONSTANT:
                {
                    constant_type constant = READ_CONSTANT;
                    
                    push (constant);
                    cout << "constant " << top () << endl;
                    break;
                }
                    
                case opcode::NEGATE:
                {
                    cout << "negate " << top () << endl;
                    push (-pop ());
                    break;
                }
                    
                case opcode::ADD:
                {
                    constant_type c0 = pop();
                    constant_type c1 = pop();
                    push (c0 + c1);
                    
                    cout << "add " << top () << endl;
//                    push (-pop ());
                    break;
                }
                    
                case opcode::SUB:
                {
                    constant_type c0 = pop();
                    constant_type c1 = pop();
                    push (c0 - c1);
                    
                    cout << "add " << top () << endl;
                    break;
                }
                    
                default:
                {
                    return interpret_result::INTERPRET_COMPILE_ERROR;
                }
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
};
