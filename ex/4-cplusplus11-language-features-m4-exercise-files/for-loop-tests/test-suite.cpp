#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

// example of a user-defined type which can be used in a range-based for loop
class MyContainer
{
    list<int> _values {111, 222, 333};
public:
    friend list<int>::iterator begin(MyContainer& cont);
    friend list<int>::iterator end(MyContainer& cont);
    // ... 
};

list<int>::iterator begin(MyContainer& cont)
{
    return cont._values.begin();
}

list<int>::iterator end(MyContainer& cont)
{
    return cont._values.end();
}

go_bandit([] {

    describe("compiler support for range-based for loops:", [] {

        it("allows loop variable to be defined in different ways", [&] {        
            vector<int> v;
            // ... populate the vector ...
            for (auto elem : v) 
                cout << elem << endl;
            for (auto& elem : v) 
                elem *= 2;

            for (int elem : v) 
                cout << elem << endl;
        });

        it("allows iteration over arrays", [&] {
            int arr[] = {10, 20, 30, 40};
            for (auto elem : arr) 
                cout << elem << endl;
        });

        it("allows iteration over initializer_list instances", [&] {
            auto list = {100, 200, 300, 400};
            for (auto elem : list) 
                cout << elem << endl;
        });

        it("allows iteration over user defined classes", [&] {
            MyContainer cont;
            for (auto& elem : cont)
                cout << elem << endl; 
        });
    });

});


int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}