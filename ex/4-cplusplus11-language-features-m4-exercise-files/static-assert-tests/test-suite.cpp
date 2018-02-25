#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

int int_magic(int a, int b)
{
    static_assert(sizeof(int) <= 4, "int must be no more than 4 bytes");
    // ... do things with a and b
}

template<unsigned int dimensions>
struct Matrix
{
    Matrix()
    {
        static_assert(dimensions <= 3, "dimensions must not exceed 3");
    }
};

struct Base
{
    virtual ~Base() {}
};

template<typename T>
class Derived : public T
{
    static_assert(has_virtual_destructor<T>::value, 
        "The base class must have a virtual destructor");
};

go_bandit([] {

    describe("compiler support for static_assert:", [] {

        it("allows static_assert in functions, classes and templates", [&] {        
            int_magic(1, 2);

            Matrix<3> m3;     // OK

            #if COMPILE_ERROR
                Matrix<4> m4;
            #endif

            Derived<Base> d;        // OK
            
            #if COMPILE_ERROR 
                Derived<string> s;      // triggers static_assert
            #endif

        });
    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}