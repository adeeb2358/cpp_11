#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// used to demonstrate the use of unnamed types as template arguments
template <typename T>
void print(const T& t) 
{
    t.print();
}

// used to demonstrate the use of unnamed enums as template arguments
template <typename T>
void output(const T& t)
{
    cout << t << endl;
}

// building blocks of an example showing arbitrary expressions 
// in a template deduction context
template<int N> 
struct A 
{ 
    static int size() { return N; } 
};

int f(int);
double f(double);

template <typename T> 
A<sizeof(f((T)0))> calc_size(T) 
{ 
    return A<sizeof(f((T)0))>(); 
}

// another example of overload resolution required for template instantiation
struct X {};
struct Y 
{
    Y(X) {}
    X operator+(const Y& other) { return X(); }
};

template <class T> 
auto add(T t1, T t2) -> decltype(t1 + t2)
{
    return t1 + t2;
}

X add(Y y1, Y y2) 
{
    cout << "Calling Y version" << endl;
    return y1 + y2;
}

// template alias example
template<typename T>
using StrKeyMap = map<string, T>;

// a more complex example of aliasing unique_ptr with a custom deleter
template<typename Stream>
struct StreamDeleter
{
    void operator()(Stream* os) const
    {
        os->close();
        delete os;
    }
};

template<typename Stream>
using StreamPtr = unique_ptr<Stream, StreamDeleter<Stream>>;


using date_t = int;   // for illustration purposes only of course

template<typename Stream, typename... Columns>
class CSVPrinter 
{
    Stream& _stream;
    array<string, sizeof...(Columns)> _headers;
    // rest of implementation
    const string _sep {","};
public:
    template<typename... Headers>
    CSVPrinter(Stream& stream, const Headers&... headers) : 
        _stream(stream), _headers({string(headers)...})
    {
        static_assert(sizeof...(Headers) == sizeof...(Columns), 
            "Number of headers must match number of columns");
    }

    void output_headers() const
    {
        for_each(begin(_headers), end(_headers) - 1, 
            [=](const string& header) { write_column(header, _sep); });
        write_column(_headers.back(), "\n");
    }

    void output_line(const Columns&... columns)
    { 
        write_line(validate(columns)...); 
    }
    // other methods, constructors etc. not shown
    template<typename... Strings>
    void output_strings(const string& s, const Strings&... strings) const 
    {
        write_column(s, _sep);
        output_strings(strings...);
    }

    void output_strings(const string& s) const
    {
        write_column(s, "\n");
    }

private:
    template<typename Value, typename... Values>
    void write_line(const Value& val, const Values&... values) const
    {
        write_column(val, _sep);
        write_line(values...);
    }

    template<typename Value>
    void write_line(const Value &val) const
    {
        write_column(val, "\n");
    }

    template<typename T>
    const T& validate(const T& val)
    {
        // this method is for illustration so it does nothing
        return val;
    }

    template<typename T>
    void write_column(const T& val, const string& terminator) const 
    {
        if (is_same<typename remove_const<T>::type, string>::value)
            _stream << "\"" << val << "\"";
        else 
            _stream << val;
        _stream << terminator;
    }
};

// example of using template parameter pack traversal to calculate
// size requirements of a set of types
template<typename... Types> 
struct TupleSize;

template<typename Head, typename... Tail> // traverse types
struct TupleSize<Head, Tail...> 
{
    static const size_t value = sizeof(Head) + TupleSize<Tail...>::value;
};

template<> struct TupleSize<>   // end recursion
{ 
    static const size_t value = 0;
};

template<typename Head, typename... Tail> 
const size_t TupleSize<Head, Tail...>::value;

const size_t TupleSize<>::value;


// example of expanding a template parameter pack into a set of base classes
template<typename... Bases>
class Derived : public Bases...
{};


// nested parameter packs
template<typename... Args1> 
struct zip 
{
  template<typename... Args2> 
  struct with 
  {
    typedef tuple<pair<Args1, Args2>...> type;
  };
};

typedef zip<short, int>::with<unsigned short, unsigned>::type T1;
// T1 is tuple<pair<short, unsigned short>, pair<int, unsigned>>

#if COMPILE_ERROR
    typedef zip<short>::with<unsigned short, unsigned>::type T2;
    // error: different number of arguments specified for Args1 and Args2
#endif

// example of non-type template parameter pack as well as 
// two parameter packs in a single function template
template <size_t... Ns>
struct Indexes 
{};

template<typename... Ts, size_t... Ns>
auto cherry_pick(const tuple<Ts...>& t, Indexes<Ns...>) ->
    decltype(make_tuple(get<Ns>(t)...)) 
{
    return make_tuple(get<Ns>(t)...);
}

go_bandit([] {

    describe("compiler support for templates:", [] {

        it("allows variadic templates", [&] {
            ofstream stream("out.csv");
            CSVPrinter<decltype(stream), int, date_t, string, string, int, double> printer(stream, 
                "ID", "Date", "Product ID", "Product Description", "Quantity", "Price");

            printer.output_headers();
            printer.output_line(10, 12012013, "B737", "Boeing 737", 2, 125000000);
            
            #if COMPILE_ERROR
                printer.output_line("10", 12012013, "B737", "Boeing 737", 2, 125000000);
            #endif

            printer.output_strings("abc", "def", "ghi");

            #if COMPILE_ERROR
                printer.output_strings(10, "abc", "def");
            #endif  

            AssertThat((TupleSize<int, double, char>::value), Equals(sizeof(int) + sizeof(double) + sizeof(char))); 
            AssertThat(TupleSize<>::value, Equals(0)); 
        });

        it("allows variadic function templates with two parameter packs", [&] {
            // construct tuple<int, int, const char*, const char*, int, int>
            auto data = make_tuple(10, 12012013, "B737", "Boeing 737", 2, 125000000);
            auto cherry_picked = make_tuple(get<0>(data), get<2>(data), get<4>(data));

            // construct a tuple of (10, "B737", 2)
            auto even_index_data = cherry_pick(data, Indexes<0, 2, 4>());  
            AssertThat(get<2>(even_index_data), Equals(2));
        });

        it("allows template aliases", [&] {
            using PlaneID = int;

            struct Lookup
            {
                static bool by_id(PlaneID plane_id) { return false; }
            };

            AssertThat(Lookup::by_id(PlaneID(5)), Equals(false));

            StrKeyMap<int> counters;

            {
                StreamPtr<ofstream> p_log(new ofstream("log.log"));
                *p_log << "Log statement";
            }  // stream gets closed and deleted here

            using Func = int(*)(double, double);
        });

        it("handles multiple closing brackets without spaces between them", [&] {
            vector<vector<int>> v;
            map<int, vector<vector<int>>> m;
        });

        it("allows local types as template arguments", [&] {
            struct A
            {
                string name() { return "I'm A!"; }
            };

            vector<A> v(10);
            cout << v[0].name() << endl;
        });

        it("allows unnamed types as template arguments", [&] {
            struct 
            {
                int x = 10;
                void print() const
                {
                    cout << x;
                }
            } a;

            print(a);

            enum { North, South } e;
            
            output(North);  // anonymous enums are allowed too
        });

        it("allows arbitrary expressions in template deduction context", [&] {
            AssertThat((calc_size(1).size()), Equals(4));

            X x1, x2;
            X x3 = add(x1, x2);  // deduction fails on templated add() (cannot add X + X), 
                                 // so compiler picks version taking Y args instead
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}

