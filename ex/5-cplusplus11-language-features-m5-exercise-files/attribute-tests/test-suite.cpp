#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// example of standard noreturn attribute
[[noreturn]] void thrower(const string& error) noexcept(false)
{
    throw runtime_error(error);
}


// example of standard carries_dependency attribute
void f(int* p_handle [[carries_dependency]])
{}

go_bandit([] {
    
    describe("compiler support for attributes:", [] {
        it("doesn't allow [[ anywhere other than attribute specifier", [&] {
            vector<int> v = {1, 2, 3, 4};
            // ...
            
            #if COMPILE_ERROR
                int a = v[[] { return 1; }()];   // error
                int b = v[ [] { return 2; }()];  // error
            #endif
            
            auto lambda = [] { return 2; };
            int c = v[lambda()];             // OK
        });

        it("allows GNU alignment attributes", [&] {        

            struct [[gnu::aligned (32)]] S1 {};
            struct __attribute((aligned (32))) S2 {};

            AssertThat(alignment_of<S1>::value, Equals(alignment_of<S2>::value));

            struct S {};
            AssertThat(alignment_of<S>::value, Is().Not().EqualTo(alignment_of<S2>::value));
            
            cout << alignment_of<S1>::value << endl;

            int *p = nullptr;
            f(p);
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
