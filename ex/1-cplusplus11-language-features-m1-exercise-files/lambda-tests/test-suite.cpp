#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0 // set to 1 to include invalid code in the compilation

typedef int date_t;     // fake it, type not important
typedef int Person;     // fake it, type not important


class JetPlane 
{
public:
    string model() const 
    {
        return string("Boeing 737");
    }
    
    void require_service(const date_t& date) 
    {}
        
    void load_passenger(const Person& passenger) 
    {}
};

// non-local variables can always be accessed in a lambda
function<bool()> g() 
{
    static auto a = 5;
    static auto b = -3;
    return []() { return a + b > 0; };
}

// local variables have to be captured
function<bool()> f() 
{
    auto a = 5;
    auto b = -3;
    // won't compile if a & b aren't captured
    return [a, b]() { return a + b > 0; };  
}


// a function to test passing lambdas by const reference
template <class Func> 
void by_const_ref(const Func& f) { f(); }


go_bandit([] {

    describe("compiler support for lambda expressions:", [] {

        it("compiles basic lambdas", [&] {
            vector<JetPlane> v{JetPlane()};

            for_each(v.begin(), v.end(), 
                [](const JetPlane &jet) { cout << jet.model() << endl; });
        });

        
        it("lets me initialise constants cleverly", [&] {
            int some_default_value = 10;
            bool some_condition_is_true = true;
            auto calculate = [] { return 1; };            

            // const_val can't be declared const here due to complex init sequence
            {
                auto const_val = some_default_value;
                if (some_condition_is_true) 
                { 
                    // Do some operations and
                    // calculate the value of const_val
                    const_val = calculate(); 
                }
                const_val = 1000;
            }
            
            // wrapping init code in a lambda combines it into one block
            const auto const_val = [&] {
                auto const_val = some_default_value;
                if(some_condition_is_true) 
                { 
                    // Do some operations and
                    // calculate the value of const_val
                    const_val = calculate(); 
                }
                return const_val;
            }(); // lamdba is invoked immediately!

            AssertThat(const_val, Equals(1));
        });
        
        it("deduces return type", [&] {
            auto f = [](int i) { if (i > 10) return 0.0; return double(i); };
            
            AssertThat(f(5), EqualsWithDelta(5.0, 0.001));
            
            [](int i) -> double { if (i > 10) return 0.0; return double(i); };
        });
        
        it("can take parameters", [&] {
            JetPlane jet;
            date_t today(1);
            [](JetPlane& jet, const date_t& date) { jet.require_service(date); }(jet, today);
        });
        
        it("can be stored in a variable", [&] {
            auto f = [](int i) { return i > 10; };

            f(5);   // returns false
            
            AssertThat(f(5), Equals(false));
        });
        
        it("can be stored in std::function", [&] {
            class LambdaStore 
            {
                function<bool(double)> _stored_lambda;
            public:
                function<int(int)> get_abs() const
                {
                    return [](int i) { return abs(i); };
                }

                void set_lambda(const function<bool(double)>& lambda)
                {
                    _stored_lambda = lambda;
                }
            };

            LambdaStore ls;

            ls.set_lambda([](double d) { return d > 0.0; });

            auto abs_lambda = ls.get_abs();
            abs_lambda(-10);   // returns 10
            
            AssertThat(abs_lambda(-10), Equals(10));
        });
        
        it("allows capturing by value", [&] {
            JetPlane jet;
            date_t today(10);
            [today](JetPlane& jet) { jet.require_service(today); }(jet);
        });
        
        it("can always refer to non-local vars", [&] {
            AssertThat(g()(), Equals(true));
        });
        
        it("allows capturing by reference", [&] {
            JetPlane jet;
            vector<Person> passengers;

            for_each(passengers.begin(), passengers.end(), 
                [&jet](const Person& p) { jet.load_passenger(p); });
            
            int a, b, c, d;
            [a, &b, c, &d]() {};
                
        });
        
        it("supports default capture modes", [&] {
            int a, b, c, d;

            [=]() { return (a > b) && (c < d); };

            [&]() { a = b = c = d = 10; };
            
            // override default capture by value
            [=, &a]() { a = 20; };

            #if COMPILE_ERROR 
                // override default capture by reference
                [&, d]() { d = 20; };  // doesn't compile because d is captured by value
            #endif 
            
        });
        
        it("allows capturing this", [&] {
            class Tank 
            {
            public:
                int fuel_level() { return 0; }
                
            };
                
            class JetPlane
            {
                const int _min_fuel_level;
                vector<Tank> _tanks;

            public:
                bool is_fuel_level_safe()
                {
                    return all_of(_tanks.begin(), _tanks.end(), 
                        [this](Tank& t) { return t.fuel_level() > _min_fuel_level; });
                }

                bool is_fuel_level_critical() 
                {
                    return any_of(_tanks.begin(), _tanks.end(), 
                        [=](Tank& t) { return t.fuel_level() <= _min_fuel_level; });
                }
            };
        });
        
        it("supports mutable", [&] {
            vector<pair<int, int>> flight_hours;
            // ... flight hours are populated with values ... 
            flight_hours.push_back(make_pair(1, 0)); flight_hours.push_back(make_pair(2, 0)); flight_hours.push_back(make_pair(3, 0));

            auto running_total = 100; // from previous month

            for_each(flight_hours.begin(), flight_hours.end(), 
                [running_total](pair<int, int>& x) mutable
                    { running_total += x.first; x.second = running_total; });

            for_each(flight_hours.begin(), flight_hours.end(), [](pair<int, int> x) { cout << x.first << "|" << x.second << endl; });
            
            []() mutable {}();
            
            by_const_ref([] {});            // OK
                
            by_const_ref([]() mutable {});  // OK

            #if COMPILE_ERROR
                string s("executing mutable lambda");
                by_const_ref([s]() mutable { cout << s << endl; });  // error, calling a non-const member                  
                                               // function of a const object 
            #endif
        });
        
        it("allows conversion to function pointer", [&] {
            typedef int (*Func)();

            Func f = [] { return 10; };

            f(); // invoke lambda via function pointer
        });
        
        it("allows nesting", [&] {
            enum SeatScreenMode 
            {
                public_announcement
            };

            struct SeatScreen 
            {
                void set_mode(SeatScreenMode) {}
            };

            struct Cabin {
                vector<SeatScreen> _seat_screens;
                vector<SeatScreen>& seat_screens() { return _seat_screens; }
            };
            
            auto mode = public_announcement;
            vector<Cabin> cabins;

            for_each(cabins.begin(), cabins.end(), 
                [=](Cabin& cabin)
                {
                    for_each(cabin.seat_screens().begin(), cabin.seat_screens().end(), 
                        [=](SeatScreen& seat_screen) { seat_screen.set_mode(mode); });
                });
        });
        
        it("allows recursion emulation via std::function", [&] {
            function<int(int)> fibonacci = [&](int n) -> int
            {
                if (n < 1)
                    return -1;
                else if (n == 1 || n == 2)
                    return 1;
                else
                    return fibonacci(n - 1) + fibonacci(n - 2);
            };
            
            AssertThat(fibonacci(10), Equals(55));
        });
        
        it("can cause undefined behavior", [&] {
            {
                function<int()> f;
                {
                    auto i = 5;
                    f = [&i] { return i; };
                }
                auto j = 10;
                f();	// undefined because i is out of scope
            }

            {
                function<int()> f;
                {
                    auto p = new int(10);
                    f = [=] { return *p; };
                    delete p;
                }
                f();  // undefined behavior because p has been deleted
            }

            {
                function<int()> f;

                class Plane
                {
                    int _capacity;
                public:
                    Plane(int capacity): _capacity(capacity) {}
                    function<int()> get_lambda() const
                    {
                        // at first glance you might think that _capacity is captured by 
                        // value but remember that using a default capture modes implicitly 
                        // captures *this* and you can only access class members by 
                        // capturing *this*    
                        return [=] { return _capacity; };	
                    }
                };

                {
                    Plane plane(10);
                    f = plane.get_lambda();
                }
                f();	// undefined behavior because *plane* is out of scope now
            }
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}