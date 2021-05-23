#pragma once

using namespace std;

template <bool endline = true, int tabs = 0, int mellanrum = 10>
inline void out (auto&&... s) {
    for(int i = 0; i < tabs; ++i)
    {
        cout << "\t";
    }

    ((cout << " •  " << left << setw (mellanrum) << s), ...);
    cout << setw (0);
    
    if constexpr (endline)
        cout << endl;
}


#define BYTE_INSTRUCTION \
    X (RETURN, 0) \
    X (CONSTANT, 1) \
    X (NEGATE, 0) \
    X (ADD, 2) \
    X (SUBTRACT, 2) \
    X (MULTIPLY, 2) \
    X (DIVIDE, 2) \
    X (NIL, 2) \
    X (TRUE, 2) \
    X (FALSE, 2)

/**
 one-byte operation code
 */
//inline static constexpr char const* byte_code = #byte_code;

enum opcode : uint_fast8_t
{
    
//    struct byte_code
//    {
//        int operands;
//        char const* str;
//        byte_code* code;
//
//        friend ostream& operator<< (ostream& os, byte_code const& o) {
//            os << o.str << "\t" << o.operands;
//            return os;
//        }
//    };
    
//    int operands;
    
    
//    auto execute_in_program (auto& program)
//    {
//
//    }
    
//    friend ostream& operator<< (ostream& os, instruction const& o) {
////        os << o.str << "\t" << o.operands;
//        return os;
//    }
    
    
    
#define X(code, opers, ...) \
    code,

    BYTE_INSTRUCTION
#undef X
};
//enum instruction : uint_fast8_t
//{
//#define X(byte_code, ...) \
//    byte_code,
//    BYTE_INSTRUCTION
//#undef X
//};


inline ostream& operator<< (ostream& os, opcode const& o) {
    switch (o)
    {
#define X(a, b) \
case opcode::a: \
os << #a; \
break;
            BYTE_INSTRUCTION
#undef X
//        case opcode::CONSTANT:
//            os << "CONSTANT " << ;
//            break;
//
//        case opcode::RETURN:
//            os << "RETURN";
//            break;
//
        default:
            throw;
    }
    

        return os;
}


#undef BYTE_INSTRUCTION



enum {
    OP_RETURN
};

