#include <functional>
#include <iostream>

class IntAddition {
public:
  IntAddition(int a = 0, int b = 0) : a_(a), b_(b) {}

  int GetAddition() { return a_ + b_; }

private:
  int a_;
  int b_;
};

int main() {
  IntAddition addition0(1, 1);
  IntAddition addition1(2, 2);

  std::function<int(void)> get_addition_func =
      std::bind(&IntAddition::GetAddition, &addition0);

  int (IntAddition::*get_addition_func_pointer)() = &IntAddition::GetAddition;

  std::cout << "get_addition_func() = " << get_addition_func() << "\n";
  std::cout << "(addition1.*get_addition_func_pointer)() = "
            << (addition1.*get_addition_func_pointer)() << "\n";

  return 0;
}


// github ->
// cloudflare -> vercel 
// 