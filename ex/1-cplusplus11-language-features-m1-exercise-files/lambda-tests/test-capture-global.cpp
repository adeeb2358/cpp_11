#include <iostream>

auto x = 12;
auto f = [&x]() { return x; };

int main() 
{
    std::cout << f() << std::endl;
}
