#include "foo.hpp"
#include <iostream>

namespace FOO {
#ifdef _VERSION_1_
inline
#endif
    namespace VERSION_1 {
void foo() { std::cout << "version 1 foo\n"; }
} // namespace VERSION_1

#ifdef _VERSION_2_
inline
#endif
    namespace VERSION_2 {
void foo() { std::cout << "version 2 foo\n"; }
} // namespace VERSION_2

#ifdef _VERSION_3_
inline
#endif
    namespace VERSION_3 {
void foo() { std::cout << "version 3.1.1 foo\n"; }
} // namespace VERSION_3

} // namespace FOO
