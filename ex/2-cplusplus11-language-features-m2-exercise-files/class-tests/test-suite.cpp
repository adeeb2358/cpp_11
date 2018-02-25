#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0   // set to 1 to include invalid code in the compilation

// example of template argument as friend
template <typename T, typename U> 
class Ami 
{
    friend T;            // OK
    #if COMPILE_ERROR  
        friend class U;      // still an error, can't use an elaborate specifier 
                             // in a template
    #endif
};


// example of disabling a template instantiaion
template<typename T>
void serialize(const T& obj) 
{
	cout << obj.to_string();
};

class PasswordStore 
{};

// PasswordStore not allowed to be serialized
void serialize(const PasswordStore&) = delete; 


go_bandit([] {

    describe("compiler support for inheritance control:", [] {

        it("allows override and final as variable names", [&] {
            int override = 5;       // OK
            int final = 10;         // OK
        });

        it("enforces override specifier", [&] {
            struct Base
            {
                virtual void f(int)
                {}
            };

            struct Derived : public Base
            {
                virtual void f(int) override        // OK
                {}

                #if COMPILE_ERROR 
                    virtual void f(double) override      // error
                    {}                                  
                #endif	                              
            };
        });

        it("enforces final specifier on a class", [&] {
            struct Base final
            {};

            #if COMPILE_ERROR 
                struct Derived : public Base            // compile error, can't inherit from final class
                {};
            #endif
        });

        it("enforces final specifier on a method", [&] {
            struct Interface 
            {
                virtual void f() 
                {}
            };

            struct Base : public Interface
            {
                virtual void f() final
                {}
            };          

            struct Derived : public Base
            {
                #if COMPILE_ERROR   
                    virtual void f()                    // compile error, can't override 
                    {}                                  // a final method
                #endif
            };
        });

    });

    describe("compiler support for nested classes: ", [&] {

        it("gives nested class member rights", [&] {
            class JetPlane
            {
                // ...
            private:
                int _flap_angle;

                class GPSNavigator {};

                class Autopilot 
                {
                    GPSNavigator _gps_navigator;        // OK, JetPlane::Autopilot can access 
                                                        // JetPlane::GPSNavigator
                    void adjust_flaps(JetPlane& plane, int flap_angle)
                    {
                        plane._flap_angle = flap_angle; // OK, JetPlane::Autopilot can
                                                        // access JetPlane::_flap_angle
                    }
                };
            };

            class Autopilot 
            {
            protected:
                void adjust_flaps() { cout << "Adjusting flaps" << endl; }
            };

            // more complex example of nesting
            struct Airplane 
            {
                struct EnhancedAutopilot : public Autopilot 
                {
                    struct Controller 
                    {
                        void adjust_speed() 
                        { 
                            // ... 
                            EnhancedAutopilot().adjust_flaps(); // OK
                        }
                    };

                    Controller _controller;
                };

                EnhancedAutopilot _autopilot;

                Airplane() 
                {
                    _autopilot._controller.adjust_speed();
                }

            };

            Airplane plane;
        });

    });

    describe("compiler support for friend declarations: ", [&] {

        it("allows friend declarations without 'class'", [&] {
            class A;
            class B;

            class Friend 
            {     
                friend class A;   // old declarations are still OK
                friend B;         // you can also do this now
            };
        });

        it("allows friend declarations of unknown classes", [&] {
            class Amigo 
            {
               
                friend class D;   // OK: declares new class D
                #if COMPILE_ERROR 
                    friend D;         // error: undeclared class D
                #endif
            };
        });

        it("allows typedefs to be friends", [&] {
            class B;
            typedef B B2;

            class Amigo 
            {
                friend B2;       // OK
            };
        });

        it("allows template parameters to be friends", [&] {            
            Ami<string, string> rc;   // OK
            Ami<char, string> f;      // OK, "friend char" has no effect in the template
        });

    });

    describe("compiler support for default/delete", [&] {

        it("allows defaulted functions", [&] {
            class JetPlane
            {
            public:
                JetPlane() = default;
                JetPlane(const JetPlane& other);
                JetPlane(JetPlane&&) = default;
            };

            JetPlane plane;
        });

        it("allows modified declarations of compiler-generated functions", [&] {
            class JetPlane
            {
            public:
                JetPlane() = default;
                virtual ~JetPlane() = default;
            protected:
                JetPlane(const JetPlane& other) = default;
                JetPlane& operator=(const JetPlane&) = default;
            };
        });

        it("allows deleting compiler generated methods", [&] {
            class JetPlane
            {
            public:
                JetPlane() = default;
                JetPlane(const JetPlane&) = delete;
                JetPlane& operator=(const JetPlane&) = delete;
                JetPlane(JetPlane&&) = default;
                JetPlane& operator=(JetPlane&&) = default;
            };
            JetPlane plane1;

            #if COMPILE_ERROR
                auto plane2 = plane1;
            #endif
        });

        it("allows any function to be deleted", [&] {
            PasswordStore ps;
            #if COMPILE_ERROR
                serialize(ps);
            #endif

            // mark a constructor as deleted
            class Altimeter 
            {
            public:
                Altimeter(double) {}
                Altimeter(int) = delete;
            };

            Altimeter a1(10.0);
            #if COMPILE_ERROR   
                Altimeter a2(10);
            #endif

            // disable heap allocation
            class StackOnly 
            {
            public:
                void* operator new(size_t) = delete;
            };

            StackOnly inst;  // OK

            #if COMPILE_ERROR 
                auto* p = new StackOnly();  // error
            #endif

            // all (or no) declarations of a virtual function in the hierarchy must be marked deleted
            class Base
            {
            public:
                virtual void f(int) = delete;
            };

            class Derived : public Base 
            {
            public:
                #if COMPILE_ERROR   
                    virtual void f(int) {}
                #endif
            };
        });
    });

    class Engine {};

    describe("compiler support for in-class initializers:", [&] {

        it("allows initializers in member declarations", [&] {
            class JetPlane 
            {
            public:
                vector<Engine> _engines = {Engine(), Engine()};

                #if COMPILE_ERROR
                    string _manufacturer("Boeing");
                #else
                    string _manufacturer;
                #endif
            
                string _model = "Unknown";
            };

            AssertThat(JetPlane()._engines.size(), Equals(2));
            AssertThat(JetPlane()._model, Equals("Unknown"));

            {
                // example of duplication in constructors
                class JetPlane 
                {
                    vector<Engine> _engines;
                    string _manufacturer;
                    string _model;
                public:
                    JetPlane() : _engines(2), _manufacturer("Unknown"), _model("Unknown")
                    {}

                    JetPlane(const string& manufacturer) : _engines(2), _manufacturer(manufacturer), _model("Unknown")
                    {}
                };
            }
        });

        it("allows function calls in in-class initializers", [&] {
            class JetPlane 
            {
            public:
                string _manufacturer = "Unknown";
                string _model = "Unknown";
                vector<Engine> _engines {get_engine_count(_manufacturer, _model)};

                static size_t get_engine_count(const string& manufacturer, const string& model)
                {
                    if (manufacturer == "Unknown" || model == "Unknown")
                        return 0;

                    if (manufacturer == "Airbus")
                    {
                        if (model == "A380-500")
                            return 2;
                    }
                    // ...
                }
            };

            AssertThat(JetPlane()._engines.size(), Equals(0));
        });

        it("chooses constructor initializers over in-class initializers", [&] {
            class JetPlane 
            {
            public:
                vector<Engine> _engines {2};

                JetPlane() : _engines(4)
                {}
            };

            AssertThat(JetPlane()._engines.size(), Equals(4));
        });

        it("allows misc. other uses", [&] {
            {
                struct Counter 
                {
                    int _count = 1;
                };
                #if COMPILE_ERROR 
                    Counter c = {10};
                #endif
            }
        });

    });

    describe("compiler support for inheriting constructors:", [&] {
        class PlaneID {};

        struct Lookup
        {
            static size_t engine_count(PlaneID) { return 2; }
            static string manufacturer(PlaneID) { return string("Boeing"); }
            static string model(PlaneID) { return string("737-300"); }
        };

        it("requires constructors to be reimplemented in derived classes", [&] {
            // broken example without inheriting constructors
            class Plane
            {
            public: 
                vector<Engine> _engines;
                string _manufacturer;
                string _model;
            
                Plane(const string& manufacturer) : _engines(2), _manufacturer(manufacturer), _model("Unknown")
                {}

                Plane(const PlaneID& tail_number) : _engines(Lookup::engine_count(tail_number)), 
                    _manufacturer(Lookup::manufacturer(tail_number)), _model(Lookup::model(tail_number))
                {}
            };

            // example of constructor duplication in a derived class
            class JetPlane : public Plane
            {
            public:
                // boring
                JetPlane(const string& manufacturer) : Plane(manufacturer)
                {}

                // boring
                JetPlane(const PlaneID& tail_number) : Plane(tail_number)
                {}
            };

            AssertThat(JetPlane("Boeing")._engines.size(), Equals(2));
            
        });

        class Plane
        {
        public: 
            vector<Engine> _engines;
            string _manufacturer;
            string _model;
            
            Plane(const string& manufacturer) : _engines(2), _manufacturer(manufacturer), _model("Unknown")
            {}

            Plane(const PlaneID& tail_number) : _engines(Lookup::engine_count(tail_number)), 
                _manufacturer(Lookup::manufacturer(tail_number)), _model(Lookup::model(tail_number))
            {}
        };

        it("allows inheriting constructors", [&] {
            // inheriting constructors from a base class
            class JetPlane : public Plane
            {
                using Plane::Plane;
            };

            AssertThat(JetPlane("Boeing")._engines.size(), Equals(2));  // OK
        });

        it("allows inherited constructors to be overridden", [&] {
            // override an inherited constructor
            class PropPlane : public Plane
            {
            public:
                using Plane::Plane;
                
                // overrides Plane constructor with the same parameters
                PropPlane(const string& manufacturer) : Plane(manufacturer)
                { cout << "In PropPlane()" << endl; }
            };
            PropPlane prop_plane("ATR");
        });

        it("has a problem with adding data members and inheriting constructors", [&] {
            class PropPlane : public Plane
            {
                size_t _prop_count;
            public:
                using Plane::Plane;
            };

            // oops, _prop_count is not initialized
            PropPlane prop_plane("ATR");
        });

        class Boat 
        {
            string _boat_manufacturer;
        public: 
            Boat(const string& manufacturer) : _boat_manufacturer(manufacturer)
            {}

        };

        it("complains when there's a conflict between base constructors", [&] {
            class FloatPlane : public Plane, public Boat
            {
                using Plane::Plane;
                #if COMPILE_ERROR
                    using Boat::Boat;
                #endif
            };
        });

        it("allows to resolve base class constructor conflict", [&] {
            class FloatPlane : public Plane, public Boat
            {
                using Plane::Plane;
                using Boat::Boat;
                FloatPlane(const string& manufacturer) : Plane(manufacturer), Boat("n/a")
                {}
            };
        });

    });

    describe("compiler support for delegating constructors:", [&] {
        struct Lookup
        {
            static size_t engine_count(const string& manufacturer, const string& model) { return 2; }
        };

        it("can cause code duplication without delegation", [&] {
            class JetPlane
            {
                vector<Engine> _engines;
                string _manufacturer;
                string _model;
            public:
                JetPlane() : _engines(2), _manufacturer("Unknown"), _model("Unknown")
                {
                    configure_engines();
                }

                JetPlane(const string& manufacturer, const string& model) : 
                    _engines(Lookup::engine_count(manufacturer, model)), _manufacturer(manufacturer), _model(model)
                {
                    configure_engines();
                    assign_tail_number();
                }

                // ...
            private:
                void configure_engines() 
                {}

                void assign_tail_number()
                {}
            };
        });

        it("allows delegating to other constructors", [&] {
            class JetPlane
            {
            public:
                vector<Engine> _engines;

            private:
                string _manufacturer;
                string _model;
            public:
                JetPlane() : JetPlane(2, "Unknown", "Unknown")
                {}

                JetPlane(const string& manufacturer, const string& model) : 
                    JetPlane(Lookup::engine_count(manufacturer, model), manufacturer, model)
                {
                    assign_tail_number();
                }
            private:
                JetPlane(size_t engine_count, const string& manufacturer, const string& model) : 
                    _engines(engine_count), _manufacturer(manufacturer), _model(model)
                {
                    configure_engines();
                }

                // ...

                void configure_engines() 
                {}

                void assign_tail_number()
                {}
            };

            JetPlane plane;
            AssertThat(plane._engines.size(), Equals(2));
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
