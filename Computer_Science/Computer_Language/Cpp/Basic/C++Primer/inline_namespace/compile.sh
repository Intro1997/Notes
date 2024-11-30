g++ --shared -fPIC -std=c++11 foo.cpp -o libfoo1_0.so
# g++ -L. -lfoo1_0  main.cpp -std=c++11 -o a.out