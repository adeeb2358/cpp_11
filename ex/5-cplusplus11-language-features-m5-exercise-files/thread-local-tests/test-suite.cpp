#include "precompiled.h"

#include <thread>

using namespace bandit;
using namespace std;

class B
{};

thread_local B b1;

namespace {
    thread_local B b2;
}

class C
{
    // each thread gets a separate instance counter
    static thread_local int _instance_counter; 
};

void f()
{
    // each thread invoking f() has a copy of run_count
    static thread_local int run_count;  
}

extern int i;
#if COMPILE_ERROR
    int thread_local i;           // error
#endif

#if COMPILE_ERROR
    int thread_local i;           // error
#endif

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
