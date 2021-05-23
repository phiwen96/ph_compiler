#pragma once
//#include "chunk.hpp"
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

template <typename chunk>
struct virtual_machine
{
    using self = virtual_machine;
    using opcode = typename chunk::opcode;
    using constant = typename chunk::constant;
    
    
    
    
    chunk * _chunk;
    opcode * _current;
    
    constant _stack [MAX_STACK];
    constant * _stack_top;
    
    interpret_result _result;
    
    
    auto reset_stack () -> void
    {
        _stack_top = _stack;
    }
    
    auto push (constant c) -> void
    {
        *_stack_top = c;
        _stack_top++;
    }
    
    auto pop () -> constant&
    {
        _stack_top--;
        return *_stack_top;
    }
    auto top () const -> constant&
    {
        return * (_stack_top - 1);
    }

    
    operator interpret_result () {
        return _result;
    }
    
    virtual_machine (chunk & code_file) : _chunk {&code_file}
    {
        _current = _chunk->begin();
        _stack_top = _stack;
    }
    
    virtual_machine () //: _chunk {code_file}
    {
//        'compiler c {source};
//        _result = interpret_result::INTERPRET_OK;'
    }
    
    
    
    auto interpret (char const* source) -> interpret_result
    {
        chunk _chunk;
        
//        compiler <chunk> _compiler {source, _chunk};
        auto _compiler = compiler {source, _chunk};
        
//        compile (source);
//        return interpret_result::INTERPRET_OK;
        
        chunk code_file;
        
        if (not _compiler)
        {
            return interpret_result::INTERPRET_COMPILE_ERROR;
        }
        
        _current = _chunk.lines;
        
        interpret_result result = run ();
        return result;
    }
    
   
    
    auto run () -> interpret_result
    {
#define READ_BYTE (*_current++)
#define READ_CONSTANT (_chunk -> constants.constants [READ_BYTE])
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
                    constant constant = READ_CONSTANT;
                    
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
                    constant c0 = pop();
                    constant c1 = pop();
                    push (c0 + c1);
                    
                    cout << "add " << top () << endl;
//                    push (-pop ());
                    break;
                }
                    
                case opcode::SUB:
                {
                    constant c0 = pop();
                    constant c1 = pop();
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
