#include "precompiled.h"

using namespace bandit;
using namespace std;

go_bandit([] {
    
    describe("compiler support for sizeof of members:", [] {

        it("allows sizeof without an instance", [&] {
            class A
            {
            public:
                int _a;
            };

            sizeof(A::_a);      // yields sizeof(int)

            AssertThat(sizeof(A::_a), Equals(sizeof(int)));
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
