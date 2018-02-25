#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

go_bandit([] {
    
    describe("compiler support for alignment keywords:", [] {

        it("supports alignof", [&] {
            AssertThat(alignof(double), Equals(8));

            AssertThat(alignof(double&), Equals(alignof(double)));
            AssertThat(alignof(double[10]), Equals(alignof(double)));

        });

        it("supports alignas", [&] {
            using Buffer = double[10];

            alignas(32) int arr[10];

            AssertThat(alignof(arr), Equals(32));

            struct S
            {
	            alignas(32) Buffer _buf;
            };

            AssertThat(alignof(S::_buf), Equals(32));

            struct alignas(2 * alignof(double)) Doubled
            {};

            AssertThat(alignof(Doubled), Equals(2 * alignof(double)));
        });

        it("supports alignas with a type argument", [&] {
            alignas(double) auto d = 1.0;

            AssertThat(alignment_of<decltype(d)>::value, Equals(alignof(double)));
        });

        it("allows multiple alignas specifiers", [&] {
            using A = double;
            using T = long double;
            constexpr auto N = 100;

            alignas(T) alignas(A) T buffer[N]; 

            AssertThat(alignof(buffer), Equals(max(alignof(double), alignof(long double))));
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
