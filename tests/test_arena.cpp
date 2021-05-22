#ifdef RUN_CATCH

#include <catch2/catch.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/arena.hpp>

using namespace std;



TEST_CASE ("align up")
{
//    using version = version <1, 0, 0>;
    
    arena <version <1, 0, 0>, 10> _arena;
    
    REQUIRE (_arena.alignment == 8);
    REQUIRE (_arena.align_up (10) == 16);
    
}


TEST_CASE("b")
{
    using version = version <1, 0, 0>;
    
    arena <version, 10> _arena;
    
    
    
}



#endif

