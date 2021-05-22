#ifdef RUN_CATCH

#include <catch2/catch.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/arena.hpp>

using namespace std;






TEST_CASE("b")
{
    using version = version <1, 0, 0>;
    
    arena <version> _arena;
    
    
}



#endif

