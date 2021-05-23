//#pragma once
//using namespace std;
//
//
//
//
//
//template <typename _arena_type>
//struct allo
//{
//    using arena_type = _arena_type;
//    using value_type = typename _arena_type::value_type;
//
//    allo (arena_type& arena) : _arena {arena}
//    {
//
//    }
//    
//    auto allocate (size_t n)
//    {
//        return reinterpret_cast <value_type*> (_arena.allocate (sizeof (value_type) * n));
//    }
//    auto deallocate (value_type* p, size_t n)
//    {
//        _arena.deallocate (reinterpret_cast <std::byte*> (p), n);
//    }
//
//private:
//    arena_type& _arena;
//};
//
////template <typename arena>
////allo (arena) -> allo <arena>;
