#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

class JetPlane
{
    string _model;
public:
    JetPlane() = default;
    JetPlane(const string& model) : _model(model) 
    {}

    string model() const { return _model; }

    void set_model(const string& model);
    void set_model(string&& model);
};

void JetPlane::set_model(const string& model)
{
    _model = model;
}

void JetPlane::set_model(string&& model)
{
    _model = move(model); // careful: model is a named rvalue ref so it's an
                                  // lvalue; use std::move to force a move operation
    model.clear();
}


void f(JetPlane& plane) { cout << "lvalue reference" << endl; }
void f(const JetPlane& plane) { cout << "const lvalue reference" << endl; }
void f(JetPlane&& plane) { cout << "rvalue reference" << endl; }
void f(const JetPlane&& plane) { cout << "const rvalue reference" << endl; }


// perfect forwarding used to implement make_unique
template<typename T, typename... Args> 
unique_ptr<T> make_unique(Args&&... args) 
{
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

go_bandit([] {

    describe("compiler support for move semantics:", [] {

        it("copies in the absence of move semantics", [&] {
            vector<string> v;

            v.push_back(string("a"));
            v.push_back(string("b"));
            
            // constructing a string like this results in many temporary objects
            string s = string("Boeing") + "737" + "-" + "300";
            cout << s << endl;

        });

        it("allows efficient return by value", [&] {
            // fake the types
            using Surface3D = int;  
            using Latitude = int;
            using Longitude = int;

            class Surface
            {
            public:
                Surface3D get_surface(const Latitude& lat, const Longitude& lon)
                {
                    Surface3D surface; 
                    // load up millions of points making up the surface
                    return surface; 
                }
            };

            Surface surface;
            Surface3D surface3d = surface.get_surface(Latitude(40), Longitude(20));  // no copying here
        });

        it("distinguishes between lvalues and rvalues", [&] {
            vector<int> v;
            v[0];          // lvalue because vector<int>::operator[] returns int&
            v.size();      // rvalue because because vector<int>::size() returns size_t

            string s;
            s + "abc";     // rvalue because string::operator+ returns string

            JetPlane jet;
            // example use of non-const rvalue
            cout << jet.model().append("_RR").size() << endl;  // append modifies the string
        });

        it("follows the rules of reference initialization with lvalues/rvalues", [&] {
            // non-const lvalue reference initialization
            JetPlane jet;
            JetPlane& jet_ref = jet;

            const JetPlane grounded_jet;
            #if COMPILE_ERROR 
                JetPlane& jet_ref2 = grounded_jet;     // doesn't compile

                JetPlane& jet_ref3 = JetPlane();       // doesn't compile
            #endif

            auto make_const_jet = []() -> const JetPlane { return JetPlane(); };
            
            #if COMPILE_ERROR 
                JetPlane& jet_ref4 = make_const_jet(); // doesn't compile
            #endif  

            // now a const lvalue reference
            const JetPlane& jet_ref5 = jet;                 // OK

            const JetPlane& jet_ref6 = grounded_jet;        // OK

            const JetPlane& jet_ref7 = JetPlane();          // OK

            const JetPlane& jet_ref8 = make_const_jet();    // OK

            jet_ref8; // jet_ref8 is a const lvalue – it has a name!

            // now a non-const rvalue reference
            JetPlane&& jet_ref9 = JetPlane();        // OK       
                
            #if COMPILE_ERROR
                JetPlane&& jet_ref10 = jet;  	         // doesn't compile to prevent accidental
						                                 // modification of an lvalue

                JetPlane&& jet_ref11 = grounded_jet;     // doesn't compile

                JetPlane&& jet_ref12 = make_const_jet(); // doesn't compile
            #endif

            // example of overload resolution
            {
                JetPlane jet;
                f(jet);

                const JetPlane grounded_jet;
                f(grounded_jet);        // OK

                f(JetPlane());          // OK

                auto make_const_jet = []() -> const JetPlane { return JetPlane(); };
                f(make_const_jet());    // OK
            }
        });

        it("selects move operations correctly", [&] {
            {
                // example of operation sequence with moving
                struct A
                {
                    A() 
                    {
                        cout << "A's constructor" << endl;
                    }
                    A(const A& rhs)
                    {
                        cout << "A's copy constructor" << endl;
                    }
                    A(A&& rhs) noexcept
                    {
                        cout << "A's move constructor" << endl;
                    }
                };

                vector<A> v;
                cout << "==> push_back A():" << endl;
                v.push_back(A());
                cout << "==> push_back A():" << endl;
                v.push_back(A());
            }


            {
                // example of implementing move operations
                class A
                {
                    double _d;
                    int* _p;
                    string _str;
                public:
                    A(A&& rhs) : _d(rhs._d), _p(rhs._p), _str(move(rhs._str))
                    {
                        rhs._p = nullptr;
                        rhs._str.clear();
                    }
                    A& operator=(A&& rhs)
                    {
                        delete _p;

                        _d = rhs._d;
                        _p = rhs._p;
                        _str = move(rhs._str); // careful!
                        rhs._p = nullptr;
                        rhs._str.clear();

                        return *this;
                    }
                };
            }     

            {
                // example of move semantics in regular method overloads 

                string model("Airbus 320");
                JetPlane jet;

                jet.set_model(model);                   // copy overload used
                jet.set_model(string("Airbus 320"));    // move overload used
            }
        });

        it("allows member function ref qualifiers", [&] {
            struct A
            {
                bool run() const & { return false; }
                bool run() && { return true; }
            };

            A a;
            a.run();    // calls run() const &
            A().run();  // calls run() &&

            AssertThat(a.run(), Equals(false));
            AssertThat(A().run(), Equals(true));

            struct Counter
            {
                int _count;

                Counter(int count) : _count(count) 
                {}

                Counter& operator+=(const Counter& other) 
                {
                    _count += other._count;
                    return *this;
                }

                Counter operator+(const Counter& other) const &
                {
                    Counter c(*this); // take a copy of this
                    c += other;
                    return c;
                }

                Counter operator+(const Counter& other) &&
                {
                    cout << "Calling operator+ &&" << endl;
                    *this += other;
                    return move(*this); 
                }
            };

            Counter c(1);
            c = Counter(10) + c;    // uses efficient && version of operator+
            AssertThat(c._count, Equals(11));
        });

        it("allows questionable code in the absence of ref-qualifiers", [&] {
            struct Curious 
            {
                int _count = 10;
                Curious& operator ++() { ++_count; return *this; } 
                Curious* operator &() { return this; }
            };
            
            Curious() = Curious();      // assign to rvalue
            Curious& c = ++Curious();   // c is a dangling reference
            &Curious();                 // address of rvalue

        });

        it("doesn't compile questionable code thanks to ref-qualifiers", [&] {
            struct Curious 
            {
                int _count = 10;
                Curious& operator ++() & { ++_count; return *this; } 
                Curious* operator &() & { return this; }
            };
            
            #if COMPILE_ERROR
                Curious() = Curious();      // assign to rvalue
                Curious& c = ++Curious();   // dangling reference
                &Curious();                 // address of rvalue
            #endif

        });

        it("requires ref-qualifiers on all overloads", [&] {
            struct A
            {
                bool run() const { return false; }
                #if COMPILE_ERROR 
                    bool run() && { return true; }
                #endif
            };

        });

        it("enables creation of move-only types", [&] {
            class MoveOnly
            {
                int* _p;
            public:
                MoveOnly() : _p(new int(10))
                {}

                ~MoveOnly()
                {
                    delete _p;
                }

                MoveOnly(const MoveOnly& rhs) = delete;
                MoveOnly& operator=(const MoveOnly& rhs) = delete;

                MoveOnly(MoveOnly&& rhs) 
                {
                    *this = move(rhs);
                }

                MoveOnly& operator=(MoveOnly&& rhs)
                {
                    if (this == &rhs)
                        return *this;
                    _p = rhs._p;
                    rhs._p = nullptr;
                    return *this;
                }
            };

            MoveOnly a;

            #if COMPILE_ERROR 
                MoveOnly b(a);          // copying, doesn't compile
            #endif

            MoveOnly b(move(a));    // OK
            MoveOnly c;
            c = move(b);            // OK
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}

