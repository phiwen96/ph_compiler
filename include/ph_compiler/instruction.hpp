#pragma once

using namespace std;


#define BYTE_INSTRUCTION \
    X (RETURN, 0) \
    X (CONSTANT, 1)

/**
 one-byte operation code
 */
//inline static constexpr char const* byte_code = #byte_code;

struct instruction
{
    struct byte_code
    {
        int operands;
        string str;
        
        friend ostream& operator<< (ostream& os, byte_code const& o) {
            
            return os;
        }
    };
#define X(code, opers, ...) \
    inline static auto code = byte_code \
{ \
opers, #code\
};
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


inline ostream& operator<< (ostream& os, instruction const& o) {
    switch (10) {
//#define X(byte_code, operands, ...) \
//        case instruction::byte_code: \
//            os << #byte_code; \
//for (int i = 0; i < operands; ++i) \
//{ \
//   \
//} \
//            break;
//
//        BYTE_INSTRUCTION
//#undef X
            
        default:
            throw runtime_error ("unknown byte-code");
            break;
    }
    return os;
}

#undef BYTE_INSTRUCTION



