#include "interface.h"

#include <iostream>

class Impl {
 public:
  void Method() noexcept { std::cout << "Ok!\n"; }
};

Interface::Interface() = default;
Interface::~Interface() = default;

void Interface::Method() noexcept {
  impl_->Method();
}
