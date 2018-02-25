#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0   // set to 1 to include invalid code in the compilation

class Engine
{
public:
	static const int max_power_level = 100;
	void set_power_level(int power_level) {}
};

class JetPlane 
{
    string _model;
    vector<Engine> _engines;
public:
    JetPlane(const string& model = string()) : _model(model)
    {}
        
    const string& model() const { return _model; }
    
    vector<Engine>& engines() { return _engines; }    
};


// example function for decltype
int func(double) { return 10; }


// example of trailing return type syntax in a template
template<typename X, typename Y>
auto multiply(X x, Y y) -> decltype(x * y)
{
    return x * y;
}


// example template for decltype
template <int I>
struct Num
{
    static const int c = I;
    decltype(I) _member;
    Num() : _member(c) {}
};

            
go_bandit([] {

    describe("compiler support for auto:", [] {

        it("allows auto variables", [&] {
            auto i = 5;
            auto plane = JetPlane("Boeing 737");
            cout << plane.model();
            for (auto i = plane.engines().begin(); i != plane.engines().end(); ++i)
                i->set_power_level(Engine::max_power_level);
        });

        
        it("allows multiple declarations with a single auto keyword", [&] {
            auto a = 5.0, b = 10.0;

            auto i = 1.0, *ptr = &a, &ref = b; 

            #if COMPILE_ERROR
                auto j = 10, str = "error";   // compile error
            #endif
        });
        
        
        it("allows modifiers to be added to auto", [&] {
            map<string, int> index;

            auto& ref = index;
            auto* ptr = &index;
            const auto j = index;
            const auto& cref = index; 
        });
        
        it("removes specifiers and decays arrays & functions to pointers", [&] {
            const vector<int> values;
            auto a = values;		// type of a is vector<int>
            AssertThat((is_same<decltype(a), vector<int>>::value), Equals(true));
            
            auto& b = values;		// type of b is const vector<int>&
            AssertThat((is_same<decltype(b), const vector<int>&>::value), Equals(true));
            
            volatile long clock = 0;
            auto c = clock;		// c is not volatile
            AssertThat(is_volatile<decltype(c)>::value, Equals(false));

            JetPlane fleet[10];
            auto e = fleet;		// type of e is JetPlane*
            AssertThat((is_same<decltype(e), JetPlane*>::value), Equals(true));
            
            auto& f = fleet;		// type of f is JetPlane(&)[10] - a reference
            AssertThat((is_same<decltype(f), JetPlane(&)[10]>::value), Equals(true));

            auto g = func;		// type of g is int(*)(double)
            AssertThat((is_same<decltype(g), int(*)(double)>::value), Equals(true));
            
            auto& h = func;		// type of h is int(&)(double)
            AssertThat((is_same<decltype(h), int(&)(double)>::value), Equals(true));
        });
        
        it("follows initialization rules", [&] {
            int i = 10;

            auto a = i;
            auto b(i);


            #if COMPILE_ERROR
                struct Expl
                {
                    Expl() {}
                    explicit Expl(const Expl&) {}
                };

                Expl e;
                auto c = e; // compile error
            #endif
        });
        
    });
    
    
    describe("compiler support for decltype:", [] {

        it("allows getting the type of an expression", [&] {
            int i = 10;
            cout << typeid(decltype(i + 1.0)).name() << endl;  // outputs "double"
            AssertThat((is_same<decltype(i + 1.0), double>::value), Equals(true));

            vector<int> a;
            decltype(a) b; 
            b.push_back(10);
            decltype(a)::iterator iter = a.end(); 
            
            AssertThat(multiply(2, 5.0), EqualsWithDelta(10.0, 0.001));
        });
        
        it("doesn't evaluate decltype expression", [&] {
            auto a = 10;
            decltype(a++) b;
            AssertThat(a, Equals(10));
        });
        

        it("instantiates templates as a side effect", [&] {
            // Num has to be instantiated to check that the expression is valid
            int i;
            decltype(Num<1>::c, i) var = i;     // var is int& 
        });
        
        it("provides declval", [&] {
            // declval allows the use of a type without public constructors in a decltype expression
            class A
            {
            private:
                A();
            };

            #if COMPILE_ERROR
                cout << typeid(decltype(A())).name() << endl; // doesn't compile: 
                                                              // A() is private
            #endif
            
            cout << typeid(decltype(declval<A>())).name() << endl;
            AssertThat((is_same<remove_reference<decltype(declval<A>())>::type, A>::value), Equals(true));
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}

