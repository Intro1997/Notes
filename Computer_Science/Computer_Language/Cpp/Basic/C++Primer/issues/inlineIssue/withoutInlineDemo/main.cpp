#include <cstdio>
int add(const int &a, const int &b) { return a + b; }

int main() {
  int a = add(1, 2);
  if (a) {
    std::puts("a");
  }
  return 0;
}