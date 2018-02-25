#include "precompiled.h"

using namespace bandit;
using namespace std;

go_bandit([] {

    describe("compiler support for unions:", [] {

        it("allows unions to have non-static data members with constructors", [&] {
            union Compact
	        { 
		        int _i;
		        string _s;
	        };

            #if COMPILE_ERROR
                Compact c1;
            #endif
        });

        it("requires user provided operations in unions when members disable them", [&] {
            union Compact
	        { 
		        int _i;
		        string _s;

                Compact() : _i(100)
                {}
                ~Compact()
                {}
	        };

            Compact c1;

            cout << c1._i << endl;   // outputs 100

            AssertThat(c1._i, Equals(100));

            new (&c1._s) string("ABC");     //switch the active member to _s

            AssertThat(c1._s, Equals(string("ABC")));
            
            // ... some time later
            c1._s.~string();    // destroy the string before making _i active
            c1._i = 0;          // switch the active member back to _i

            AssertThat(c1._i, Equals(0));

        });

        it("allows discriminated unions", [&] {
            class Compact 
            {
                enum 
                {
                    Int,
                    String
                } _active_type;

                union 
                {
                    int _i;
                    string _s;
                };
            public:
                Compact(int i) : _active_type(Int), _i(i)
                {}

                Compact(const string& s) : _active_type(String)
                {
                    new (&_s) string(s);                    
                }

                ~Compact()
                {
                    if (_active_type == String)
                        _s.~string();
                }

                int get_int() const 
                {
                    if (_active_type != Int)
                        throw runtime_error("Inactive type requested");

                    return _i;
                }

                void set(int i)
                {
                    if (_active_type == String)
                        _s.~string();

                    _i = i;
                    _active_type = Int;
                }

                string get_string() const 
                {
                    if (_active_type != String)
                        throw runtime_error("Inactive type requested");

                    return _s;
                }

                void set(const string& s)
                {
                    if(_active_type == String)
                        _s = s;
                    else
                        new(&_s) string(s);
                    _active_type = String;
                }
            };

            Compact c1(100);

            cout << c1.get_int() << endl;       // outputs 100
            AssertThat(c1.get_int(), Equals(100));

            AssertThrows(runtime_error, c1.get_string());  // throws because _s is inactive

            string s("ABC");
            c1.set(s);

            cout << c1.get_string() << endl;    // outputs ABC
            AssertThat(c1.get_string(), Equals("ABC"));
            AssertThrows(runtime_error, c1.get_int());

        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
