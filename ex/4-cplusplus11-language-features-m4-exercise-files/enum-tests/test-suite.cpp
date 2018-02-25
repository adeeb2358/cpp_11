#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// navigator.h
struct Navigator  
{
    Navigator();
private:
    enum CompassPoint : int;  // forward declaration
    CompassPoint _compass_point;
};

// navigator.cpp
enum Navigator::CompassPoint : int { North, South, East, West };

Navigator::Navigator() : _compass_point(North)
{}


go_bandit([] {

    describe("compiler support for enums:", [] {

        it("allows scoped enums", [&] {        
            enum class Proportion 
            {
                OneHalf,
                OneThird,
                OneQuarter
            };

            #if COMPILE_ERROR 
                Proportion prop = OneThird;                 // error
            #endif

            Proportion prop2 = Proportion::OneThird;    // OK         

            #if COMPILE_ERROR
                prop2 == 1;     // error
            #endif

            auto prop = Proportion::OneThird;
            #if COMPILE_ERROR
                if (prop == 1)      // error
                {
                    // ...
                }
            #endif
        });

        it("allows specifying enumerant type", [&] {
            enum Direction : unsigned short
            {
                South,
                West,
                East,
                North
            };

            cout << sizeof(North) << endl;  // outputs sizeof(unsigned short)
            AssertThat(sizeof(North), Equals(sizeof(unsigned short)));

            #if COMPILE_ERROR 
                enum Color : double             // error
                {
                    Black
                };
            #endif
        });

        it("allows forward declaration of enums", [&] {
            // flight_board.h
            enum class AirportCode; // forward declared enum

            struct FlightBoard
            {
                void print_airport_name(AirportCode code) 
                {}

                void print_flight(AirportCode code, const string& flight)
                {
                    // ...
                    print_airport_name(code);
                }
            };

            Navigator navigator;

            enum E : short;       // OK
            #if COMPILE_ERROR
                enum F;               // error, underlying type is required
            #endif
            enum class G : short; // OK
            enum class H;         // OK, underlying type for scoped enums is int by default
    
            enum E : short;       // OK, redeclaration
            enum class G : short; // OK, redeclaration
            enum class H;         // OK, redeclaration
            enum class H : int;   // OK, redeclaration with the same underlying type
            
            #if COMPILE_ERROR
                enum class E : short; // error, can't change from unscoped to scoped
                enum G : short;       // error, can't change from scoped to unscoped

                enum E : int;         // error, different underlying type
                enum class G;         // error, different underlying type
                enum class H : short; // error, different underlying type
            #endif

            enum class H {};      // OK, this redeclaration is a definition

        });
    
    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}