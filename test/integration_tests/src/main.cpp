#include "TypeDefinitions.hpp"

int main()
{
    TestClass t;

    t.fn1();

    t.fn2();
    t.fn2(true);
    t.fn2("Hello, world");

    t.fn3();

    freeFunction();

    EnumClass anEnum;

    return 0;
}