
#include <iostream> // printf, ...

struct Quux {
    int i = 0;
    Quux() {
      printf("Quux born (%x)\n", this);
    }
    ~Quux() {
      printf("Quux died (%x)\n", this);
    }
};

// class *declaration*
class Foo;

// class *definition*
class Foo : public Quux { // Quux is the parent class of Foo
  float f = 2.1;
 public:
  Foo() {
      // class *constructor*
      // this happens first thing when a new Foo is made
      printf("Foo born (%x)\n", this);
  }
  ~Foo() {
      // class *destructor*
      // this happens ust before the Foo is deleted from memory
      printf("Foo died (%x)\n", this);
  }
  float getf() { return f; }    // getter
  void setf(float v) { f = v; } // setter
};

float add(float a, float b) {
  return a + b;
}

int main() {
    printf("sizeof(Foo) = %d bytes\n", sizeof(Foo));
    printf("sizeof(int) = %d bytes\n", sizeof(int));
    printf("sizeof(float) = %d bytes\n", sizeof(float));


    auto fp = new Foo(); // memory allocation on the HEAP
    float h = 8;
    Foo foo; // Foo *instantiation* (on the STACK)
    foo.setf(3.2);
    printf("foo.f = %f\n", foo.getf());
    printf("a + b = %f\n", add(2.3, 7.8));
    delete fp; // deletes (frees) the memory at fp and the destructor is called
    return 0;
}
