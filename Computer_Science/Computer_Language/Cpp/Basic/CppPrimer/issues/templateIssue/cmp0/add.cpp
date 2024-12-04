// template <typename T> T tAdd(const T &v0, const T &v1);
#include "tAdd.hpp"
extern template int tAdd(const int &a, const int &b);
int add(const int &a, const int &b) { return tAdd<int>(a, b); }