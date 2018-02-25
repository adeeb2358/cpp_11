#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

namespace API
{
	inline namespace v2
	{
		// v2 allows processing doubles instead of ints
		void process(vector<double>)
        {}
    }

	namespace v1
	{
		void process(vector<int>)
        {}
	}
}

// example of problems with versioning without inline namespaces
namespace API2
{
	namespace v2
	{
		template<typename T>
		class Hash
		{
		public:
			size_t operator()(const T& t) const;
		};
    }
    using namespace v2;
}

// a user defined class which Hash is specialized for
class Part
{};

namespace API2
{
    #if COMPILE_ERROR 
        // specializing in the API2 namespace doesn't work
        // because Hash is actually in API2::v2
        template<>
        class Hash<Part> 
        {
            size_t operator()(const Part& p) const 
            { 
                /* ... */ 
                return 0;
            }
        };
    #endif
}

go_bandit([] {
    
    describe("compiler support for inline namespaces:", [] {

        it("allows inline namespaces", [&] {
            vector<double> doubles;
            API::process(doubles);

            vector<int> ints;
            API::v1::process(ints);

            #if COMPILE_ERROR
                API::process(ints);     // trying to use v2 with vector<int> doesn't compile
            #endif
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
