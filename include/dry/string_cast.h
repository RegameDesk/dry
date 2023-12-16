//*********************************************************
//
//  Copyright (c) RegameDesk. All rights reserved.
//  This code is licensed under the MIT License.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
//  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//*********************************************************

#pragma once

namespace dry {

inline std::string_view CastToStringView(const char* ptr) {
  if (nullptr == ptr) {
    return std::string_view{ptr, 0};
  }
  return std::string_view{ptr};
}

inline std::string_view CastToStringView(std::u8string_view utf8) {
  return std::string_view{reinterpret_cast<const char*>(utf8.data()),
                          utf8.size()};
}

inline std::u8string_view CastToU8stringView(std::string_view sv) {
  return std::u8string_view{reinterpret_cast<const char8_t*>(sv.data()),
                            sv.size()};
}

}  // namespace dry
