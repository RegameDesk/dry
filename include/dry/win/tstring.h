#pragma once

#include <tchar.h>

#include <string>
#include <string_view>

namespace dry::win {

using tstring = std::basic_string<TCHAR>;
using tstring_view = std::basic_string_view<TCHAR>;

}  // namespace dry::win
