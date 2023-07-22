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

#include <string>
#include <string_view>

namespace dry {

template <typename Char>
inline std::basic_string<Char> Clone(std::basic_string_view<Char> sv) {
  return std::string{sv};
}

template <typename Char>
inline std::basic_string<Char> Clone(const Char* sz) {
  return std::string{sz};
}

}  // namespace dry
