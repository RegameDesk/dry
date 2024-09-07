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

#include <cstddef>
#include <type_traits>
#include <utility>

namespace dry {

// Implementation of "Stack-allocated Pimpl" Idiom.
template <typename T, size_t Size, size_t Alignment>
class Pimpl {
 public:
  template <typename... TArgs>
  explicit Pimpl(TArgs&&... args) {
    new (Ptr()) T(std::forward<TArgs>(args)...);
  }

  explicit Pimpl(const Pimpl<T, Size, Alignment>& rhs) { new (Ptr()) T(*rhs); }
  explicit Pimpl(Pimpl<T, Size, Alignment>&& rhs) {
    new (Ptr()) T(std::move(*rhs));
  }
  Pimpl<T, Size, Alignment>& operator=(Pimpl<T, Size, Alignment>&& rhs) {
    *Ptr() = std::move(*rhs);
    return *this;
  }
  Pimpl<T, Size, Alignment>& operator=(const Pimpl<T, Size, Alignment>& rhs) {
    *Ptr() = *rhs;
    return *this;
  }

  ~Pimpl() {
    Validate<sizeof(T), alignof(T)>();
    Ptr()->~T();
  }

  T* operator->() noexcept { return Ptr(); }
  const T* operator->() const noexcept { return Ptr(); }
  T& operator*() noexcept { return *Ptr(); }
  const T& operator*() const noexcept { return *Ptr(); }

 private:
  T* Ptr() noexcept { return reinterpret_cast<T*>(&storage_); }
  const T* Ptr() const { return reinterpret_cast<const T*>(&storage_); }

  template <std::size_t ActualSize, std::size_t ActualAlignment>
  static void Validate() noexcept {
    static_assert(Size == ActualSize, "Size and sizeof(T) mismatch.");
    static_assert(Alignment == ActualAlignment,
                  "Alignment and alignof(T) mismatch.");
  }

 private:
  alignas(Alignment) std::byte storage_[Size];
};

}  // namespace dry
