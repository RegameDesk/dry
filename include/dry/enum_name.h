//*********************************************************
//
//  Copyright (c) 2025 RegameDesk. All rights reserved.
//
//  This code is licensed under the MIT License.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
//  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//*********************************************************

#pragma once

#include <string>

#include <magic_enum/magic_enum.hpp>

namespace dry {

template <typename E>
inline std::string EnumName(E e) {
  std::string name;
  name = magic_enum::enum_name(e);
  if (name.empty()) {
    name = std::to_string(static_cast<int>(e));
  }
  return name;
}

}  // namespace dry
