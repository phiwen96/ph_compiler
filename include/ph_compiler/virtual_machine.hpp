#pragma once
//#include "chunk.hpp"
#include "opcode.hpp"
#include "version.hpp"
#include "compiler.hpp"
//#include "value.hpp"

//using version = version <1, 0, 0>;


using namespace std;

enum struct interpret_result
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};








#define MAX_STACK 256

template <typename chunk_type>
struct virtual_machine
{
    using self = virtual_machine;
    using code_type = typename chunk_type::code_type;
    using value_type = typename chunk_type::value_type;
    
    virtual_machine (char const* source) : m_source {source}
    {
        
    }
    
    operator interpret_result ()
    {
//        chunk_type _chunk;
        
        auto _compiler = compiler <chunk_type> {m_source, *m_chunk};
        
                
        if (not _compiler)
        {
            return interpret_result::INTERPRET_COMPILE_ERROR;
        }
        
        m_current_instruction = m_chunk -> m_code;
        
        interpret_result result = run ();
        return result;
    }
    
    
    char const* m_source;
    
    chunk_type * m_chunk;
    code_type * m_current_instruction;
    
    value_type m_stack [MAX_STACK];
    value_type * m_top_stack;
    
    
    
    auto reset_stack () -> void
    {
        m_top_stack = m_stack;
    }
    
    auto push (value_type c) -> void
    {
        *m_top_stack = c;
        m_top_stack++;
    }
    
    auto pop () -> value_type
    {
        m_top_stack--;
        return *m_top_stack;
    }
    auto top () const -> value
    {
        return * (m_top_stack - 1);
    }

    

    
    virtual_machine (chunk_type & code_file) : m_chunk {&code_file}
    {
        m_current_instruction = m_chunk->begin();
        m_top_stack = m_stack;
    }
    
    virtual_machine () //: _chunk {code_file}
    {
//        'compiler c {source};
//        _result = interpret_result::INTERPRET_OK;'
    }
    
    
    
  
    
   
    
    auto run () -> interpret_result
    {
#define READ_BYTE (*m_current_instruction++)
#define READ_CONSTANT (m_chunk -> m_constants.m_values [READ_BYTE])
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtimeError("Operands must be numbers."); \
        return interpret_result::INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b)); \
    } while (false)
        for (;;)
        {
            uint_fast8_t instruction;

            switch (instruction = READ_BYTE)
            {
                case opcode::RETURN:
                {
//                    cout << "return " << top () << endl;
                    pop ();
                    return interpret_result::INTERPRET_OK;
                }
                    
                case opcode::CONSTANT:
                {
                    value_type constant = READ_CONSTANT;
                    
//                    push (constant);
//                    cout << "constant " << top () << endl;
                    break;
                }
                    
                case opcode::NEGATE:
                {
//                    cout << "negate " << top () << endl;
                    if (! IS_NUMBER (peek (0)))
                    {
                        runtimeError ("Operand must be a number.");
                        return interpret_result::INTERPRET_RUNTIME_ERROR;
                    }
                    double d = -AS_NUMBER (pop ());
                    
                    push (NUMBER_VAL (-AS_NUMBER (pop ())));
                    break;
                }
                    
       
                case code_type::ADD:      BINARY_OP (NUMBER_VAL, +); break;
                case code_type::SUBTRACT: BINARY_OP (NUMBER_VAL, -); break;
                case code_type::MULTIPLY: BINARY_OP (NUMBER_VAL, *); break;
                case code_type::DIVIDE:   BINARY_OP (NUMBER_VAL, /); break;
                    
             
                    
                default:
                {
                    return interpret_result::INTERPRET_COMPILE_ERROR;
                }
            }
        }
    }
    
    auto peek (int distance) -> value
    {
      return m_top_stack [-1 - distance];
    }
    
    auto runtimeError(const char* format, ...) -> void
    {
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      fputs("\n", stderr);

      size_t instruction = m_current_instruction - m_chunk->m_code - 1;
      int line = m_chunk -> m_code [instruction];
      fprintf(stderr, "[line %d] in script\n", line);
        reset_stack ();
    }
    
    
#undef READ_BYTE
#undef READ_CONSTANT
};
