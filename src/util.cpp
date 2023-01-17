// Display images inside a terminal
// Copyright (C) 2023  JustKidding
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "util.hpp"
#include "process_info.hpp"
#include "free_delete.hpp"

#include <xcb/xcb.h>
#include <memory>
#include <xcb/xproto.h>

auto util::str_split(std::string const& str, std::string const& delim) -> std::vector<std::string>
{
    auto start = 0U;
    auto end = str.find(delim);

    std::vector<std::string> res;

    while (end != std::string::npos) {
        res.push_back(str.substr(start, end - start));
        start = end + delim.length();
        end = str.find(delim, start);
    }

    return res;
}

auto util::get_parent_pids(int const& pid) -> std::vector<int>
{
    std::vector<int> res;
    ProcessInfo proc(pid);
    while (proc.ppid != 1) {
        res.push_back(proc.ppid);
        proc = ProcessInfo(proc.ppid);
    }
    return res;
}

auto util::window_has_property(
        xcb_connection_t *connection,
        xcb_window_t window,
        xcb_atom_t property,
        xcb_atom_t type) -> bool
{
    auto cookie = xcb_get_property(connection, false, window, property, type, 0, 4);
    auto reply = std::unique_ptr<xcb_get_property_reply_t, free_delete> {
        xcb_get_property_reply(connection, cookie, nullptr)
    };
    if (!reply.get()) return false;
    return xcb_get_property_value_length(reply.get()) != 0;
}
