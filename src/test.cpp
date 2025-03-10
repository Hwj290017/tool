#include "smartPtr.h"
#include <iostream>
using hwj::SmartPtr;
class A
{
  public:
    int a;
};
// void fun(SmartPtr<A> a)
// {
// }
int main()
{
    SmartPtr<A> s = new A();
    SmartPtr<A> a = s;
    std::cout << a->a;
}