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

#include <vector>

namespace dry {

// Similar to ATL::CHeapPtr
template <typename T>
class HeapPtr : public std::vector<std::byte> {
 public:
  T* operator->() noexcept { return Ptr(); }
  const T* operator->() const noexcept { return Ptr(); }
  T& operator*() noexcept { return *Ptr(); }
  const T& operator*() const noexcept { return *Ptr(); }

 private:
  T* Ptr() noexcept { return reinterpret_cast<T*>(__super::data()); }
  const T* Ptr() const noexcept {
    return reinterpret_cast<const T*>(__super::data());
  }
};

}  // namespace dry
