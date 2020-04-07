#include <iostream>

using namespace std;

class Foo
{
public:
  Foo() : _x(3) {}
  void func()
  {
    [this]() {
      // member veriable can be used in lambda Closures 
      // just as same as how to access member variables as usual
      cout << _x << endl;
    } ();
  }

private:
  int _x;
};

int main()
{
  Foo f;
  f.func();
}