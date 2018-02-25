#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// example of using initializer_list in a return statement
vector<int> extract_core_points(const vector<int>& v)
{
    return {v.front(), v[v.size() / 2], v.back()};
}

template<typename T> 
void f(const vector<T>&)
{}

go_bandit([] {

    describe("compiler support for uniform initialization:", [] {

        it("didn't allow brace initialization in some cases before C++11", [&] {        
            class Point 
            {
            public:
                int _x, _y;
                Point(int x, int y) : _x(x), _y(y) 
                {}
            };

            Point p1 {10, 20};
            Point p = {10, 20};
            
            int values[] = {1, 2, 3};
            AssertThat(values[2], Equals(3));
            
            int* p_values = new int[3] {1, 2, 3}; // not going to happen

            class Hexagon 
            {
                int _points[6];

                Hexagon()  // no way to initialize _points in initialization list
                {}
            };


            // variations of initialization syntax
            {
                int x(10);
                int y = 20;
                int values[] = {1, 2, 3};
            }

            {
                int x {5};
                int* p_values = new int[3] {1, 2, 3};
            }
        });


        it("allows brace initialization in most cases", [&] {        
            class Point 
            {
            public:
                int _x, _y;
                Point(int x, int y) : _x(x), _y(y) 
                {}
            };

            Point p = {10, 20};

            int* p_values = new int[3] {1, 2, 3}; 

            class Hexagon 
            {
                int _points[6];

                Hexagon() : _points {1, 2, 3, 4, 5, 6}
                {}

            };

            vector<int> v {1, 2, 3, 4};
            AssertThat(v[2], Equals(3));

            vector<int> core_points = extract_core_points({1, 2, 3, 4, 5});
            AssertThat(core_points[0], Equals(1));
            AssertThat(core_points[1], Equals(3));
            AssertThat(core_points[2], Equals(5));

            core_points.insert(core_points.end(), {7, 9, 11});
            AssertThat(core_points.size(), Equals(6));

            #if COMPILE_ERROR 
                int16_t w {0};
                int16_t x = {w + 1};   // error

                unsigned int a {true ? 1 : 2}; // OK

                bool flag {true};
                unsigned int b {flag ? 1 : 2}; // error
            #endif

        });

        it("supports initializer_list", [&] {
            #if COMPILE_ERROR 
                vector<unique_ptr<int>> pointers {unique_ptr<int>(new int(1))};  // error
                pointers.push_back(unique_ptr<int>(new int(1)));  // OK
            #endif

            initializer_list<int> list = {1, 2, 3};
            vector<int> values(list);
            AssertThat(values.size(), Equals(3));

            int x[] =  {1, 2.5, 3};  // produces narrowing warning


            using Error = string;  // fake it
            using Point = int;     // fake it

            struct Lookup
            {
                static Point point(int index) { return 0; }
            };

            class Polygon
            {
                vector<Point> _points;
            public:
                Polygon(initializer_list<int> point_indexes) 
                {
                    if (point_indexes.size() < 3)
                        throw Error("Polygons require 3 or more points");
                    for_each(point_indexes.begin(), point_indexes.end(),
                        [=] (int index) { _points.push_back(Lookup::point(index)); });

                    for_each(begin(point_indexes), end(point_indexes),
                        [=] (int index) { _points.push_back(Lookup::point(index)); });

                    const int* p = point_indexes.begin(); 
                    cout << p[1] << endl;
                }
            };
        });

        it("doesn't deduce the type when using a brace list with templates", [&] {
            #if COMPILE_ERROR
                f({1, 2, 3});
            #endif

            // unless you are specific enough
            f(vector<int>{1, 2, 3});
	        f<int>({1, 2, 3});            
        });

    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
