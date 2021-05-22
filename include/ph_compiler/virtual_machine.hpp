#pragma once
#include "codefile.hpp"
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



template <typename Version>
struct virtual_machine;



template <>
struct virtual_machine <version <1, 0, 0>>
{
    using version = version <1, 0, 0>;
    
    codefile <version> & _code_file;
    opcode * _current_opcode;
    
    
    virtual_machine (codefile <version> & code_file) : _code_file {code_file}
    {
        _current_opcode = _code_file.begin();
    }
    
    
    
    auto run () -> result
    {
        for (;;)
        {
            opcode & current_opcode = * exchange (_current_opcode, _current_opcode + 1);
            opcode & next_opcode = * _current_opcode;
            
            switch (current_opcode)
            {
                case opcode::RETURN:
                {
                    cout << "return" << endl;
                    return result::INTERPRET_OK;
                }
                    
                case opcode::CONSTANT:
                {
                    cout << "constant " << _code_file.constants [next_opcode] << endl;
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


template <>
struct virtual_machine <version <2, 0, 0>>
{
    using m_version = version <2, 0, 0>;
    using self = virtual_machine <m_version>;
    
    codefile <m_version> & _code_file;
    opcode * _current_opcode;
    
    
    virtual_machine (codefile <m_version> & code_file) : _code_file {code_file}
    {
        _current_opcode = _code_file.begin();
    }
    
    
    template <typename Version>
    auto run () -> result;
    
    template <>
    auto run <version <2, 0, 0>> () -> result
    {
        for (;;)
        {
            opcode & current_opcode = * exchange (_current_opcode, _current_opcode + 1);
            opcode & next_opcode = * _current_opcode;
            
            switch (current_opcode)
            {
                case opcode::RETURN:
                {
                    cout << "return" << endl;
                    return result::INTERPRET_OK;
                }
                    
                case opcode::CONSTANT:
                {
                    cout << "constant " << _code_file.constants [next_opcode] << endl;
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
