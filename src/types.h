#pragma once

#include <cstddef>
#include <ctime>
#include <stdexcept>
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
    std::size_t _total_money;
    std::tm _total_time;
    std::tm _start_time;
public:
    table() : _is_busy(false) {}
    table(table const&) = delete;
    table(table &&) = default;
    table& operator=(table const&) = delete;
    table& operator=(table &&) = default;
    ~table() = default;

    bool is_busy() const noexcept {
        return _is_busy;
    }

    std::size_t get_money() const noexcept {
        return _total_money;
    }

    std::tm const* total_time() const noexcept {
        return &_total_time;
    }

    void take(std::tm const& time) {
        if (_is_busy) {
            throw std::runtime_error("place is already busy");
        }
        _is_busy = true;
        _start_time = time;
    }

    static std::size_t count_mins(std::tm const& start, std::tm const& end) {
        int busy_in_mins = end.tm_hour * 60 + end.tm_min - start.tm_hour * 60 - start.tm_min;
        if (busy_in_mins < 0) {
            throw std::runtime_error("travels to the past are not allowed here");
        }
        return static_cast<std::size_t>(busy_in_mins);
    }


    void free(std::tm const& time, std::size_t hour_cost) {
        if (!_is_busy) {
            throw std::runtime_error("place is free");
        }
        std::size_t busy_in_mins = count_mins(_start_time, time);
        _is_busy = false;
        _total_money = hour_cost * (busy_in_mins / 60 + (busy_in_mins % 60 ? 1 : 0));
        _total_time.tm_hour += busy_in_mins / 60 + (_total_time.tm_min + (busy_in_mins % 60)) / 60; 
        _total_time.tm_min += (_total_time.tm_min + busy_in_mins) % 60;
        if (_total_time.tm_hour >= 24) {
            throw std::runtime_error("there is only 24 hours a day");
        } 
    }
};

}