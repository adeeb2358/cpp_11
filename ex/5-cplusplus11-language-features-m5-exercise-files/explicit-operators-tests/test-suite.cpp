#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

go_bandit([] {
    
    describe("compiler support for explicit conversion operators:", [] {

        it("disables implicit conversions using explicit operators", [&] {
            class Probability
            {
                double _value;
            public:
                Probability(double value) : _value(value)
                {}

                explicit operator bool() const noexcept 
                {
                    return _value > 0.0;
                }
            };
            
            Probability p1(0.5), p2(0.0);

            #if COMPILE_ERROR 
                double sum = p1 + p2;       // this would compile if operator bool wasn't explicit; but likely not what you'd want
            #endif
            
            if (!p1)
                AssertThat(false, Equals(true));

            if (p2)
                AssertThat(false, Equals(true));
        
            
            function<void(int, int)> f1, f2;
            #if COMPILE_ERROR 
	            auto a = f1 + f2;	// error
    	        bool flag = f1;	    // error
            #endif

            if (f1)
                f1(10, 20);
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
