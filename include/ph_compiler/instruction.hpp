#pragma once

using namespace std;


#define BYTECODES \
    X (RETURN) \
    X (CONSTANT)

/**
 one-byte operation code
 */
enum instruction : uint_fast8_t
{
#define X(byte_code) \
    byte_code,
    BYTECODES
#undef X
};


inline ostream& operator<< (ostream& os, instruction const& o) {
    switch (o) {
#define X(byte_code) \
        case instruction::byte_code: \
            os << #byte_code; \
            break;
        
        BYTECODES
#undef X
            
        default:
            throw runtime_error ("unknown byte-code");
            break;
    }
    return os;
}

#undef BYTECODES
