#include <iostream> // printf

class Base {
 public:
  virtual void onInit() {
    printf("Base::onInit() called\n");
  }
};

class Foo : public Base {
 public:
  virtual void onInit() override {
    printf("Foo::onInit() called\n");
  }
};

int main() {
    Foo foo;
    foo.onInit();
    Base& base = foo;
    base.onInit();
}