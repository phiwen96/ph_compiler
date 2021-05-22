#pragma once

using namespace std;


#define BYTECODES \
    X (RETURN) \
    X (CONSTANT)

/**
 one-byte operation code
 */
enum struct operation_code : int_fast8_t
{
#define X(byte_code) \
    byte_code,
    BYTECODES
#undef X
};


inline ostream& operator<< (ostream& os, operation_code const& o) {
    switch (o) {
#define X(byte_code) \
        case operation_code::byte_code: \
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
