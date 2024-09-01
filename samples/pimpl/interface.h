#include "dry/pimpl.hpp"

class Impl;

class Interface {
 public:
  Interface();
  ~Interface();

  void Method() noexcept;

 private:
  static constexpr size_t kImplSize = 1;
  static constexpr size_t kImplAlignment = 1;

  dry::Pimpl<Impl, kImplSize, kImplAlignment> impl_;
};
