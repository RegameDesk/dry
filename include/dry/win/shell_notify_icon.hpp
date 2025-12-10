/*
 * Copyright 2024-present umu618@hotmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <shellapi.h>

#include <wil/resource.h>

#include <boost/assert.hpp>
#include <gsl/gsl>

namespace dry {

inline namespace win {

template <class T>
class ShellNotifyIconImpl {
 public:
  ShellNotifyIconImpl(UINT message) noexcept : WM_SHELL_NOTIFY_ICON_{message} {
    WM_TASKBAR_CREATED_ = ::RegisterWindowMessageW(L"TaskbarCreated");
  }

  ~ShellNotifyIconImpl() {
    // Remove the icon
    DeleteNotifyIcon();
  }

  // Add a notify icon
  //  tooltip - The tooltip to display
  //  icon    - The icon to display
  //  id      - The resource ID of the context menu
  HRESULT AddNotifyIcon(gsl::cwzstring tooltip,
                        HICON icon,
                        UINT id,
                        HINSTANCE instance) noexcept {
    instance_ = instance;

    notify_icon_data_.cbSize = sizeof(notify_icon_data_);
    notify_icon_data_.hWnd = *static_cast<T*>(this);
    notify_icon_data_.uID = id;
    notify_icon_data_.hIcon = icon;
    notify_icon_data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    notify_icon_data_.uCallbackMessage = WM_SHELL_NOTIFY_ICON_;
    wcscpy_s(notify_icon_data_.szTip, std::size(notify_icon_data_.szTip),
             tooltip);
    if (!::Shell_NotifyIconW(NIM_ADD, &notify_icon_data_)) {
      notify_icon_data_.uFlags = 0;
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT DeleteNotifyIcon() noexcept {
    if (0 == notify_icon_data_.uFlags) {
      return S_FALSE;
    }
    // Remove
    notify_icon_data_.uFlags = 0;
    if (!::Shell_NotifyIcon(NIM_DELETE, &notify_icon_data_)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT ModifyNotifyIcon(HICON icon) noexcept {
    BOOST_ASSERT(notify_icon_data_.uFlags);

    if (0 == notify_icon_data_.uFlags) {
      return E_NOTIMPL;
    }
    // Fill in the data
    notify_icon_data_.hIcon = icon;
    notify_icon_data_.uFlags = NIF_ICON;
    if (!::Shell_NotifyIconW(NIM_MODIFY, &notify_icon_data_)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  HRESULT SetBalloonTooltipText(gsl::cwzstring info_text,
                                gsl::cwzstring info_title = nullptr,
                                UINT timeout = 1000) noexcept {
    if (!info_text) {
      return E_INVALIDARG;
    }
    // Fill the structure
    notify_icon_data_.uFlags = NIF_INFO;
    notify_icon_data_.dwInfoFlags = NIIF_INFO;
    notify_icon_data_.uTimeout = timeout;
    wcscpy_s(notify_icon_data_.szInfo, std::size(notify_icon_data_.szInfo),
             info_text);
    if (info_title) {
      wcscpy_s(notify_icon_data_.szInfoTitle,
               std::size(notify_icon_data_.szInfoTitle), info_title);
    }
    if (!::Shell_NotifyIconW(NIM_MODIFY, &notify_icon_data_)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

  // Set the default menu item ID
  void SetDefaultItem(UINT id) noexcept { menu_default_item_ = id; }

  // Set the icon tooltip text
  HRESULT SetTooltipText(gsl::cwzstring tooltip) noexcept {
    if (!tooltip) {
      return E_INVALIDARG;
    }
    // Fill the structure
    notify_icon_data_.uFlags = NIF_TIP;
    wcscpy_s(notify_icon_data_.szTip, _countof(notify_icon_data_.szTip),
             tooltip);
    if (!::Shell_NotifyIconW(NIM_MODIFY, &notify_icon_data_)) {
      return HRESULT_FROM_WIN32(::GetLastError());
    }
    return S_OK;
  }

 protected:
  // Call me in the window procedure
  std::optional<LRESULT> OnMessage(HWND window,
                                   UINT message,
                                   WPARAM w,
                                   LPARAM l) noexcept {
    if (WM_SHELL_NOTIFY_ICON_ == message) {
      return OnShellNotifyIcon(window, message, w, l);
    }
    if (WM_TASKBAR_CREATED_ == message) {
      return OnTaskbarCreated(window, message, w, l);
    }
    return std::nullopt;
  }

  // overridables
  void PrepareMenu(HMENU menu) noexcept = delete;

 private:
  std::optional<LRESULT> OnShellNotifyIcon(HWND window,
                                           UINT message,
                                           WPARAM w,
                                           LPARAM l) noexcept {
    BOOST_ASSERT(window == *static_cast<T*>(this));
    BOOST_ASSERT(w == notify_icon_data_.uID);

    // Is this the ID we want?
    if (w != notify_icon_data_.uID) {
      return 0;
    }

    switch (auto derived = static_cast<T*>(this); LOWORD(l)) {
      case WM_LBUTTONDBLCLK: {
        // Make app the foreground
        ::SetForegroundWindow(window);
        wil::unique_hmenu menu;
        menu.reset(
            ::LoadMenuW(instance_, MAKEINTRESOURCEW(notify_icon_data_.uID)));
        if (!menu) {
          return 0;
        }
        if (menu_default_item_) {
          ::PostMessageW(window, WM_COMMAND, menu_default_item_, 0);
        } else {
          HMENU popup_menu = ::GetSubMenu(menu.get(), 0);
          UINT item = ::GetMenuItemID(popup_menu, 0);
          ::PostMessageW(window, WM_COMMAND, item, 0);
        }
        return 0;
      }
      case WM_RBUTTONUP: {
        wil::unique_hmenu menu;
        menu.reset(
            ::LoadMenuW(instance_, MAKEINTRESOURCEW(notify_icon_data_.uID)));
        if (!menu) {
          return 0;
        }
        HMENU popup_menu = ::GetSubMenu(menu.get(), 0);
        derived->PrepareMenu(popup_menu);

        POINT pos;
        if (!::GetCursorPos(&pos)) {
          return 0;
        }
        ::SetForegroundWindow(window);
        // Set the default menu item
        if (menu_default_item_ == 0) {
          ::SetMenuDefaultItem(popup_menu, 0, TRUE);
        } else {
          ::SetMenuDefaultItem(popup_menu, menu_default_item_, FALSE);
        }
        // Ref: WTL - atluser.h - _FixTrackMenuPopupX
        int cmd = ::TrackPopupMenuEx(popup_menu, TPM_LEFTALIGN | TPM_RETURNCMD,
                                     pos.x, pos.y, window, nullptr);
        ::PostMessageW(window, WM_COMMAND, cmd,
                       reinterpret_cast<LPARAM>(window));
        // BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
        // ::PostMessage(window, WM_NULL, 0, 0L);
        return 0;
      }
    }

    return std::nullopt;
  }

  // The taskbar has been recreated, add the icon again
  std::optional<LRESULT> OnTaskbarCreated(HWND window,
                                          UINT message,
                                          WPARAM w,
                                          LPARAM l) noexcept {
    DeleteNotifyIcon();
    notify_icon_data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    ::Shell_NotifyIconW(NIM_ADD, &notify_icon_data_);
    return 0;
  }

 private:
  const UINT WM_SHELL_NOTIFY_ICON_;

  NOTIFYICONDATAW notify_icon_data_{};
  HINSTANCE instance_;
  UINT menu_default_item_{};
  UINT WM_TASKBAR_CREATED_{};
};

}

}
