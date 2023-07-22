#include <iostream>
#include <locale>

#include "dry/string_conv.h"

int main() {
  std::locale::global(std::locale(".UTF-8"));

  std::cout << dry::CastToStringView(u8"RegameDesk 云桌面\n");
  std::cout << dry::ConvertToUtf8(L"RegameDesk 云桌面\n");
  std::wcout << dry::ConvertToWide(u8"RegameDesk 云桌面\n");
}
