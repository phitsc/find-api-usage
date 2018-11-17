#include "TestClass.hpp"

namespace
{

void someFunction()
{
    TestClass t;

    t.fn1();

    t.fn2();
    t.fn2(true);
    t.fn2("Hello, world");

    t.fn3();
}

}

int main()
{
    return 0;
}