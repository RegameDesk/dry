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

#include <type_traits>

namespace dry {

// to print via std::format
template <typename T>
inline int EnumToInt(T e) noexcept {
  static_assert(std::is_enum<T>());
  return static_cast<int>(e);
}

// to print via std::format
template <typename T>
inline void* Pointer(T p) {
  return reinterpret_cast<void*>(p);
}

// for print HRESULT as unsigned via std::format
template <typename T>
inline auto Unsign(const T& hr) {
  return std::make_unsigned_t<T>(hr);
}

}  // namespace dry
