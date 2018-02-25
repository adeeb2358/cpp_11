#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// constexpr functions have to rely on the ternary operator and recursion
constexpr long fibonacci(int n)
{
    return n < 1 ? -1 : 
        (n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2));
};


// function templates can be constexpr
template<typename T>
constexpr auto square(const T& v) -> decltype(v * v)
{
    return v * v;
}

constexpr auto c_sq = square(12345);

// branches which aren't taken aren't evaluated at compile time
constexpr int percentage(int i)
{
    return (i >= 0 && i <= 100) ? i : throw "out of range";
}

// examples of constexpr variables
constexpr int val = percentage(99);

constexpr auto c_dimensions = 3;
constexpr auto c_threshold = 42.5;
constexpr auto c_name = "constexpr evaluator";

// Complex is a literal type so it can be used to initialize constexpr variables
class Complex 
{
    double _real, _imaginary;
public:
    constexpr Complex(double real, double imaginary) : _real(real), _imaginary(imaginary) 
    {}

    constexpr double real() const { return _real; }
    constexpr double imaginary() const { return _imaginary; }
};

// constexpr functions can operate on user-defined literal types
constexpr Complex operator+(const Complex& lhs, const Complex& rhs)
{
    return Complex(lhs.real() + rhs.real(), lhs.imaginary() + rhs.imaginary());
}

constexpr Complex c1(1, 2);
constexpr Complex c2(3, 4);

constexpr Complex c3 = c1 + c2;

constexpr auto c_real = c3.real();


go_bandit([] {

    describe("compiler support for constexpr:", [] {

        it("allows function calls to be evaluated at compile time", [&] {        
            enum Fibonacci
            {
                Ninth = fibonacci(9),
                Tenth = fibonacci(10)
            };

            static_assert(Fibonacci::Tenth == 55, "Fib");

            auto a = 4, b = 6;
            cout << fibonacci(a + b) << endl;  // outputs 55

            AssertThat(fibonacci(a + b), Equals(55));
        });

        it("allows constexpr to be initialized with a const", [&] {
            auto a = 10;
            const auto b = a;
            const auto c = 10;

            #if COMPILE_ERROR
                constexpr auto d = b;
            #endif  
            constexpr auto e = c;

            cout << c_name << endl;
        });
    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}