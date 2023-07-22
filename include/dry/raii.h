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

// gsl::final_action allows you to ensure something gets run at the end of a
// scope. FinalAction is a more powerful version of gsl::final_action, with the
// following aditional ability:
// 1. Dismiss: cancel the action.
// Should use gsl::final_action as possible.
template <class F>
class FinalAction {
 public:
  explicit FinalAction(const F& f) noexcept : f_{f} {}
  explicit FinalAction(F&& f) noexcept : f_{std::move(f)} {}

  ~FinalAction() noexcept {
    if (invoke_) {
      f_();
    }
  }

  FinalAction(FinalAction&& other) noexcept
      : f_(std::move(other.f_)), invoke_(std::exchange(other.invoke_, false)) {}

  FinalAction(const FinalAction&) = delete;
  void operator=(const FinalAction&) = delete;
  void operator=(FinalAction&&) = delete;

  void Dismiss() noexcept { invoke_ = false; }

 private:
  const F f_;
  bool invoke_{true};
};

}  // namespace dry
