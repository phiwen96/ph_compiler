/**
 "The first step in any compiler or interpreter is scanning.
 The scanner takes in raw source code as a series of characters and groups it
 into a series of chunks we call tokens. These are the meaningful “words” and
 “punctuation” that make up the language’s grammar."
 
 https://craftinginterpreters.com/scanning.html
 */


#pragma once

using namespace std;

template <int version>
struct scanner;


template <>
struct scanner </*__version__*/ 0>
{
    
};


