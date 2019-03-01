#include "TypeDefinitions.hpp"

namespace {

inline void freeFunction(int n)
{
    AnotherClass another;
    another.fnTestClass({});
    another.fnTestClassRef({});

    EnumClass enumClass = EnumClass::Enum1;
    another.fnEnumClass(enumClass);

    TestClass test;
    test.fn2(n);
}

}

#define MACRO_CALLING_FUN(x) freeFunction(x)

int main()
{
    TestClass t;
    t.fn1();
    t.fn2();
    t.fn2(true);
    t.fn2(std::string("Hello, world"));
    t.fn3();

    freeFunction(0);
    MACRO_CALLING_FUN(2);

    EnumClass anEnum = EnumClass::Enum1;
    TypedefedClass tc;
    TypedefedEnum te;
    UsingClass uc;
    UsingEnum ue;
    TemplateClass<int> inttemp;

    return 0;
}
