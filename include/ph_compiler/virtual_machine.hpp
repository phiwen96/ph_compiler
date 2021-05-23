#pragma once
//#include "codefile.hpp"
#include "opcode.hpp"
#include "version.hpp"

//using version = version <1, 0, 0>;


using namespace std;

enum struct result
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
    
    
    
    
    codefile_type & _code_file;
    opcode_type * _current_opcode;
    
    constant_type _stack [MAX_STACK];
    constant_type * _stack_top;
    
    
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

    
    
    virtual_machine (codefile & code_file) : _code_file {code_file}
    {
        _current_opcode = _code_file.begin();
        _stack_top = _stack;
    }
    
   
    
    auto run () -> result
    {
#define READ_BYTE (*_current_opcode++)
#define READ_CONSTANT (_code_file.constants.constants [READ_BYTE])
        for (;;)
        {
            uint_fast8_t instruction;

            switch (instruction = READ_BYTE)
            {
                case opcode::RETURN:
                {
                    cout << "return " << top () << endl;
                    pop ();
                    return result::INTERPRET_OK;
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
                    return result::INTERPRET_COMPILE_ERROR;
                }
            }
        }
    }
};
