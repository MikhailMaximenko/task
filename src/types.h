#pragma once

#include <ctime>
#include <string>

namespace computer_club {

struct util_values {
    constexpr static const char* time_format = "%H:%M";
};

class client {
    std::string _name;
public:
    client() = delete;
    client(client const&) = delete;
    client(client&&) = default;
    client& operator=(client const&) = delete;
    client& operator=(client&&) = default;
    ~client() = default;

    client(std::string &&name) 
        : _name(std::move(name)) 
    {}
};

class table {
    bool _is_busy;
    std::tm _total_time;
    std::tm _start_time;
public:
    table() : _is_busy(false) {}
    table(table const&) = delete;
    table(table &&) = delete;
    table& operator=(table const&) = delete;
    table& operator=(table &&) = delete;
    ~table() = default;

    bool is_busy() const noexcept {
        return _is_busy;
    }
};

}