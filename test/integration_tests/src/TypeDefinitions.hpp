#pragma once

#include <string>

class TestClass
{
public:
    void fn1() {}
    void fn2() {}
    void fn2(bool) {}
    void fn2(std::string) {}
    void fn3() {}
    void fn4() {}
};

enum class EnumClass
{
    Enum1,
    Enum2,
    Enum3
};

class AnotherClass
{
private:
    TestClass m_test;
    EnumClass m_enum;
};

inline void freeFunction() {}
