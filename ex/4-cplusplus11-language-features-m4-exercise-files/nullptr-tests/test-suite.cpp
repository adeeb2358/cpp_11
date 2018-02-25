#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

bool ambiguous(int)
{
    return false;
}

bool ambiguous(int*)
{
    return true;
}

go_bandit([] {

    describe("compiler support for nullptr:", [] {

        it("allows nullptr to be used instead of NULL", [&] {
            {
                int* p = nullptr;
            }

            {
                int* p = nullptr;
                int* p1 = NULL;
                int* p2 = 0;
                p1 == p;    // true
                p2 == p;    // true

                delete p;   // OK, p is nullptr
            }

            #if COMPILE_ERROR
                cout << int(nullptr) << endl;  // error
            #endif


            #if COMPILE_ERROR
                // example of NULL creating ambiguity and nullptr resolving it
                ambiguous(NULL);       // returns false, ambiguous(int) overload chosen
                AssertThat(ambiguous(NULL), Equals(false));
            #endif
            
            ambiguous(nullptr);    // returns true, ambiguous(int*) overload chosen
            AssertThat(ambiguous(nullptr), Equals(true));

            // uniform initialization syntax sets pointers to nullptr by default
            int* p {};  // p is set to nullptr
            AssertThat(p == nullptr, Equals(true));
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}