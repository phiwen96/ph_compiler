#pragma once


using namespace std;



enum struct valuetype
{
    BOOL,
    NIL,
    NUMBER
};


struct value
{
    valuetype _type;
    union
    {
        bool _boolean;
        double _number;
    } _as;
};

#define BOOL_VAL(v)   ((value){valuetype::BOOL, {._boolean = v}})
#define NIL_VAL           ((value){valuetype::NIL, {._number = 0}})
#define NUMBER_VAL(x) ((value){valuetype::NUMBER, {._number = x}})


#define IS_BOOL(v)    ((v)._type == valuetype::BOOL)
#define IS_NIL(v)     ((v)._type == valuetype::NIL)
#define IS_NUMBER(v)  ((v)._type == valuetype::NUMBER)


#define AS_BOOL(v)    ((v)._as._boolean)
#define AS_NUMBER(v)  ((v)._as._number)
