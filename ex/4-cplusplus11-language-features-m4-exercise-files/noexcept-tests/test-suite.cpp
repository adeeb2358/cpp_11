#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// basic use of noexcept
constexpr long fibonacci(int n) noexcept
{
    return n < 1 ? -1 : 
        (n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2));
};


// example of noexcept operator used as a noexcept condition
template<typename T>
auto square(const T& v) noexcept(noexcept(v * v)) -> decltype(v * v) 
{
    return v * v;
}


// custom std::terminate handler
void terminate_with_output()
{
    cout << "std::terminate was invoked" << endl;
    _Exit(0);
}

// noexcept(false) example
void thrower(const string& error) noexcept(false)
{
    throw runtime_error(error);
}

go_bandit([] {
    
    set_terminate(terminate_with_output);

    describe("compiler support for noexcept:", [] {

        it("allows noexcept", [&] {        
            AssertThat(fibonacci(10), Equals(55));
        });

        it("marks compiler generated operations noexcept", [&] {
            class A
            {};

            AssertThat(is_nothrow_default_constructible<A>::value, Equals(true));
            AssertThat(is_nothrow_copy_constructible<A>::value, Equals(true));
            AssertThat(is_nothrow_move_constructible<A>::value, Equals(true));
            AssertThat(is_nothrow_copy_assignable<A>::value, Equals(true));
            AssertThat(is_nothrow_move_assignable<A>::value, Equals(true));
            AssertThat(is_nothrow_destructible<A>::value, Equals(true));            
        });

        it("allows functions to be marked conditionally noexcept", [&] {
            class B
            {
            public:
                ~B() noexcept(false)
                {}
            };

            AssertThat(is_nothrow_destructible<B>::value, Equals(false));

            class C
            {
                int _c;
            public:
                C() noexcept : _c(fibonacci(10))
                {}
            };

            AssertThat(is_nothrow_constructible<C>::value, Equals(true));

            AssertThat(square(10), Equals(100));
        });

        it("invokes std::terminate if an exception is thrown from a noexcept function", [&] {
            class Naughty
            {
            public:
                Naughty() noexcept
                {
                    throw runtime_error("I'm a bad object");
                }
            };
            Naughty naught;     // should result in a call to terminate_with_output
        });

        it("allows noexcept to be added to function pointers", [&] {
            long (*p_fib)(int) = fibonacci;  // OK, but noexcept is lost

            long (*p_fib2)(int) noexcept = fibonacci; // instead, noexcept can be preserved

            void(*p_thrower)(const string&) noexcept = thrower;

            using Func = void(const string&) noexcept;
            Func* func = thrower;
        });
    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}