#pragma once

#include <string>

class TestClass
{
public:
    void fn1() {}
    void fn2() {}
    void fn2(bool) {}
    void fn2(std::string) {}
    void fn2(int) {}
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
public:
    void fnTestClass(TestClass aTest) {}
    void fnTestClassRef(const TestClass& aTest) {}
    void fnEnumClass(EnumClass anEnum) {}

private:
    TestClass m_test;
    EnumClass m_enum;

    void getRidOfUnusedPrivateField()
    {
        m_test.fn1();
        m_enum = EnumClass::Enum1;
    }
};

template<typename T>
class TemplateClass
{

};

typedef TestClass TypedefedClass;
typedef EnumClass TypedefedEnum;

using UsingClass = TestClass;
using UsingEnum = EnumClass;
