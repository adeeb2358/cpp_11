#include "precompiled.h"

#include <complex>

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

enum class Unit
{
    astronomical_unit,
    light_year, 
};

class Distance
{
    const double _val;
    static constexpr double c_au_in_light_year = 63241.077;
public:
    constexpr Distance(double val, Unit unit) : _val(unit == Unit::astronomical_unit ? val : to_astronomical_units(val))
    {}

    constexpr double to_light_years() const 
    {
        return _val / c_au_in_light_year;
    }
private:
    static constexpr double to_astronomical_units(double val)
    {
        return val * c_au_in_light_year;
    }
};

constexpr Distance operator "" _au(unsigned long long n)  
{
    return Distance(n, Unit::astronomical_unit);
}

// create a 'complex' instance from an imaginary literal
constexpr complex<double> operator "" _i(long double d)  
{
    return {0, double(d)};   
}

namespace raw_operators
{
    // example of raw integer literal operator
    unsigned long long operator"" _b(const char* digits)
    {
        if (strlen(digits) > numeric_limits<unsigned long long>::digits)
            throw runtime_error("Too many digits in binary literal");

        unsigned long long res = 0;
        auto digit = digits;
        while (*digit != '\0')
        {
            if (*digit != '1' && *digit != '0')
                throw runtime_error("Only 1 and 0 allowed in binary literals");

            res = (*digit - '0') + (res << 1);
            ++digit;
        }
        return res;
    }
}

// example of string literal operator operating on UTF-16 strings
u16string operator "" _reverse(const char16_t* str, size_t len)
{
	u16string s(str);
	reverse(s.begin(), s.end());
	return s;
}

namespace template_operators
{
    template<char... Digits>
    struct Binary;

    template<char digit, char... Digits>
    struct Binary<digit, Digits...> 
    {
        static_assert(
            sizeof...(Digits) + 1 <= numeric_limits<unsigned long long>::digits, 
            "Too many digits in binary literal");
        static_assert(digit == '1' || digit == '0', 
            "Only 1 and 0 allowed in binary literals");

        static constexpr unsigned long long value = 
            ((digit - '0') << sizeof...(Digits)) + Binary<Digits...>::value;
    };

    template<>
    struct Binary<> 
    {
        static constexpr unsigned long long value = 0;
    };

    template<char... Digits>
    constexpr unsigned long long operator "" _b() 
    {
        return Binary<Digits...>::value;
    }
}

go_bandit([] {

    describe("compiler support for Unicode literals:", [] {

        it("supports different Unicode prefixes", [&] {        
            string utf8str(u8"UTF-8: \u00BD");
	        u16string utf16str(u"UTF-16: \uA654");
	        u32string utf32str(U"UTF-32: \U0002387F");

            string s(u8"\u00BD \u00B5s");   // the string represents ½ µs
            cout << s << endl;
        });

        it("has different character and string types", [&] {
            char16_t c1;
            char32_t c2;
            u16string s1;
            u32string s2;
        });

    });


    describe("compiler support for raw literals:", [] {

        it("allows raw literals", [&] {        
            cout << R"(use "\n" to represent newline)" << endl;
        });

        it("allows custom delimiters for raw literals", [&] {        
            cout << R"!!(A raw literal is delimited with "( )")!!" << endl;
        });

        it("allows literal newlines in raw literals", [&] {
            string s(R"(multiline
                        literal)");

            AssertThat(s, Contains("\n"));
            cout << s << endl;
        });

    });

    describe("compiler support for user defined literals:", [] {

        it("allows raw literal operators for integers", [&] {
            constexpr double radius = (30_au).to_light_years();
            cout << "Neptune orbit radius: " << radius << " light years" << endl;
        });


        it("allows raw literal operators for integers", [&] {    
            using namespace raw_operators;

            101_b;      // equals 5
            -1011_b;    // equals -11

            AssertThat(0_b, Equals(0));
            AssertThat(1_b, Equals(1));
            AssertThat(-10000000000_b, Equals(-1024));
            AssertThat(101_b, Equals(5));
            AssertThat(1011_b, Equals(11));
            AssertThrows(runtime_error, 123_b);
            AssertThat(string(LastException<runtime_error>().what()), Contains("1 and 0"));

        });

        it("allows string literal operators", [&] {
            AssertThat(u"palindrome"_reverse, Equals(u16string(u"emordnilap")));

            uR"(two\nlines)"_reverse;	// yields "seniln\owt"

            AssertThat(u"The quick brown fox "
	            "jumps over a lazy dog "
	            "and back in reverse"_reverse, 
                Equals(u"The quick brown fox jumps over a lazy dog and back in reverse"_reverse));
        });

        it("allows template literal operators", [&] {
            using namespace template_operators;
            constexpr auto value = 101_b;
            static_assert(value == 5, "Binary error");
            static_assert(0_b == 0, "Binary error");

            #if COMPILE_ERROR
                123_b;
            #endif
            static_assert(10000000000_b == 1024, "Binary error");
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}