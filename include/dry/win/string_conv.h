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

#include <Windows.h>

#include <string>

namespace dry {

inline namespace win {

inline std::wstring AToW(std::string_view mbcs,
                         _In_ std::uint32_t codepage = CP_THREAD_ACP) noexcept {
  std::wstring wcs{};
  if (mbcs.empty()) {
    return wcs;
  }

  constexpr DWORD kFlags = MB_ERR_INVALID_CHARS;
  int a_length = static_cast<int>(mbcs.size());
  const int w_length = ::MultiByteToWideChar(codepage, kFlags, mbcs.data(),
                                             a_length, nullptr, 0);
  if (0 == w_length) {
    return wcs;
  }

  wcs.resize(w_length);
  bool failed = (0 == ::MultiByteToWideChar(codepage, kFlags, mbcs.data(),
                                            a_length, wcs.data(), w_length));
  if (failed) {
    wcs.clear();
  }
  return wcs;
}

inline std::string WToA(
    std::wstring_view wcs,
    _In_ std::uint32_t convert_codepage = CP_THREAD_ACP) noexcept {
  std::string mbcs{};
  if (wcs.empty()) {
    return mbcs;
  }

  DWORD flags = 0;
  constexpr DWORD kGB18030 = 54936;
  if (CP_UTF8 == convert_codepage || kGB18030 == convert_codepage) {
    flags = WC_ERR_INVALID_CHARS;
  }
  int w_length = static_cast<int>(wcs.size());
  const int a_length =
      ::WideCharToMultiByte(convert_codepage, flags, wcs.data(), w_length,
                            nullptr, 0, nullptr, nullptr);
  if (0 == a_length) {
    return mbcs;
  }

  mbcs.resize(a_length);
  bool failed =
      (0 == ::WideCharToMultiByte(convert_codepage, flags, wcs.data(), w_length,
                                  mbcs.data(), a_length, nullptr, nullptr));
  if (failed) {
    mbcs.clear();
  }
  return mbcs;
}

}  // namespace win

inline std::string ConvertToAnsi(std::wstring_view wcs) noexcept {
  return WToA(wcs, CP_ACP);
}

inline std::string ConvertToUtf8(std::wstring_view wcs) noexcept {
  return WToA(wcs, CP_UTF8);
}

inline std::wstring ConvertToWide(std::u8string_view utf8) noexcept {
  return AToW(CastToStringView(utf8), CP_UTF8);
}

inline std::string ConvertToAnsi(std::u8string_view utf8) noexcept {
  return WToA(ConvertToWide(utf8), CP_ACP);
}

}  // namespace dry
