#include <Windows.h>

#include <format>
#include <iostream>

#include "dry/enum_name.h"
#include "dry/types.h"

int main() {
  enum Status { kOpening, kOpened, kClosing, kClosed };
  Status status{kOpening};
  // std::cout << std::format("status = {}\n", status);
  std::cout << std::format("status = {}\n", dry::EnumToInt(status));
  std::cout << std::format("status = {}\n", dry::EnumName(status));

  int i{};
  int* p = &i;
  // std::cout << std::format("p = {}\n", p);
  std::cout << std::format("p = {}\n", dry::Pointer(p));

  HRESULT hr = E_FAIL;
  std::cout << std::format("hr = {:#x}\n", hr);
  std::cout << std::format("hr = {:#x}\n", dry::Unsign(hr));
}
