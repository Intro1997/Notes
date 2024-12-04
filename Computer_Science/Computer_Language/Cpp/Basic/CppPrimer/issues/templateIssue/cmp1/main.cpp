#include "tAdd.hpp"
#include <iostream>
#include <unistd.h>

extern int add(const int &a, const int &b);

int mainAdd(const int &a, const int &b) { return tAdd<int>(a, b); }

int main() {
  std::cout << mainAdd(1, 2) << std::endl;
  std::cout << add(1, 2) << std::endl;
  sleep(-1);
  return 0;
}