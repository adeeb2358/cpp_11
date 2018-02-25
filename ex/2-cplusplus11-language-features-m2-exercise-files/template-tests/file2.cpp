#include "file1.h"

// file2.cpp
using namespace std;

extern template int templated_func(const int&);

void g()
{
    cout << templated_func(1234);
}

extern template vector<int>;

extern template vector<int>::size_type vector<int>::size() const;

int main()
{
    g();
    return 0;
}