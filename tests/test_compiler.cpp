//#include "testlib.hpp"

#ifdef RUN_CATCH

//#include "test.hpp"
#include <catch2/catch.hpp>
#include <ph_compiler/compiler.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/scanner.hpp>
#include <ph_compiler/chunk.hpp>
#include <ph_compiler/opcode.hpp>
#include <ph_compiler/disassembler.hpp>
#include <ph_compiler/value.hpp>
#include <ph_compiler/virtual_machine.hpp>
#include <ph_compiler/version.hpp>
#include <ph_compiler/arena.hpp>
#include <ph_compiler/allo.hpp>


using namespace std;




//using arena_type_opcode = arena <std::byte, alignof (std::byte), 1024>;
//using arena_type_constants = arena <std::byte, alignof (std::byte), 1024>;
//
//arena_type_opcode _opcode_arena;
//arena_type_constants _constants_arena;
//
//
//using allocator_opcode = allo <arena_type_opcode>;
//using allocator_constants = allo <arena_type_constants>;
//
//allocator_opcode _opcode_allocator {_opcode_arena};
//allocator_constants _constants_allocator {_constants_arena};





struct app
{
    using chunk = chunk <opcode, value, signed>;
    
    char* _path;
    interpret_result _result;
    
    app (char const * path)
    {
        _path = (char*) malloc (strlen (path) + 1);
        strcpy (_path, path);
    }
    
    auto run () -> void
    {
        run_file (_path);
    }
    
    
private:
    
    inline static auto run_file (char const* path) -> void
    {
        auto source = read_file (path);
        
        auto result = (interpret_result) virtual_machine <chunk> {source};
        
        free (source);
        
        if (result == interpret_result::INTERPRET_COMPILE_ERROR) exit (65);
        if (result == interpret_result::INTERPRET_RUNTIME_ERROR) exit (70);
    }

    inline static char* read_file (const char* path) {
        FILE* file = fopen (path, "rb");
        
        if (file == NULL) {
            fprintf(stderr, "Could not open file \"%s\".\n", path);
            exit(74);
        }
        
        fseek (file, 0L, SEEK_END);
        size_t fileSize = ftell (file);
        rewind (file);
        
        char* buffer = (char*) malloc (fileSize + 1);
        
        if (buffer == NULL) {
            fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
            exit(74);
        }
        
        size_t bytesRead = fread (buffer, sizeof (char), fileSize, file);
        
        if (bytesRead < fileSize) {
            fprintf(stderr, "Could not read file \"%s\".\n", path);
            exit(74);
        }
        
        
        buffer [bytesRead] = '\0';
        
        fclose (file);
        return buffer;
    }
};


template <typename code>
struct vm
{
    code& m_code;
    
};



template <typename T>
struct sequence
{
    sequence () : m_begin {nullptr}, m_end {nullptr}, m_current {nullptr}
    {
        
    }
    
    auto operator* () -> T&
    {
        return m_current;
    }
    
    auto capacity () const
    {
        return m_end - m_begin;
    }
    
    auto active () const
    {
        return m_current - m_begin;
    }
    
    auto begin () -> T*&
    {
        return m_begin;
    }
    
    auto end () -> T*&
    {
        return m_end;
    }
    
    auto operator= (auto&& other)
    {
        m_current = forward <decltype (other)> (other);
    }
    
    auto operator+= (auto&& i)
    {
        m_current += std::forward <decltype (i)> (i);
    }
    
    auto operator-= (auto&& i)
    {
        m_current -= std::forward <decltype (i)> (i);
    }
    
    auto operator*= (auto&& i)
    {
        m_current *= std::forward <decltype (i)> (i);
    }
    
    auto operator/= (auto&& i)
    {
        m_current /= std::forward <decltype (i)> (i);
    }
    
    auto operator--(auto&& i) -> sequence&
    {
        m_current -= std::forward <decltype (i)> (i);
    }
    
    
    
    
    
//private:
    T* m_begin;
    T* m_end;
    T* m_current;
};

template <typename instruction, typename value>
struct code
{
    
    
    auto operator+= (instruction i) -> code&
    {
        m_instructions.push_back (i);
        return *this;
    }
    
    auto operator+= (value v) -> code&
    {
        m_values.push_back (v);
        return *this;
    }
    
    
//    auto operator* () -> instruction
//    {
//        return *()
//    }
    
    
private:
    sequence <instruction> m_instructions;
    sequence <value> m_values;
    
};

TEST_CASE ("sequence")
{
    sequence <int> s;
    REQUIRE (s.capacity() == 0);
    REQUIRE (s.active() == 0);
    
    s.begin() = (int*) aligned_alloc (alignof (int), sizeof (int) * 10);
    s.end() = s.begin() + 10;
    
    REQUIRE (s.capacity() == 10);
    
    s = s.begin();
    REQUIRE (s.active() == 0);
//    s +=
    s = s.begin () + 2;
    
    REQUIRE (s.active() == 2);
    s += 2;
    
    REQUIRE (s.active() == 4);
    
}

TEST_CASE("")
{
    code <uint8_t, double> m_code;
    vm <code <uint8_t, double>> m_vm {m_code};
    
}


TEST_CASE("AAA")
{
    return;
    
    
//    char szOrbits[] = "365.243434589";
//      char* pEnd;
//      double d1, d2;
//      d1 = strtod (szOrbits, &pEnd);
//      d2 = strtod (pEnd, NULL);
//    cout << d1 << endl << d2 << endl;
//      printf ("The moon completes %.2f orbits per Earth year.\n", d1/d2);
    
    auto path = filesystem::path (__FILE__).parent_path() / filesystem::path {"test_file_0.hpp"};
    app _app {path.c_str()};
    _app.run();
    
    
    
//
//
//
//    return 0;
    
    
//    codefile <opcode, double> _codefile;
    
//    auto constant_index = _codefile.add_constant (6);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    constant_index = _codefile.add_constant (5);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    _codefile.write_opcode (opcode::ADD);
//
//    constant_index = _codefile.add_constant (6);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    constant_index = _codefile.add_constant (6);
//    _codefile.write_opcode (opcode::CONSTANT);
//    _codefile.write_opcode (constant_index);
//
//    _codefile.write_opcode (opcode::SUB);
    
    
    
    
    
    //    _codefile.write_opcode (opcode::NEGATE);
    
//    _codefile.write_opcode (opcode::RETURN);

    
//    virtual_machine _vm {_codefile};
//    _vm.run();
    
    
    //    cout << disassembler {_codefile} << endl;
    
    
    
    
    
    
    
    
    
    
    //    virtual_machine <>
    
    
    
    
    
    
    
    //    codefile <version> f0;
    //    f0.push_back (opcode::CONSTANT);
    //    f0.push_back (4);
    //    f0.push_back (opcode::RETURN);
    
    
    
    //    virtual_machine <version> vm {f0};
    
    
    
    
    
    //    b0 += instruction::RETURN;
    //    b0 += instruction::RETURN;
    //    b0 += instruction::CONSTANT;
    //    b0 += 4;
    //    cout << disassembler {f0} << endl;
    //    vm.run <version> ();
}



#endif
