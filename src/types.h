#pragma once

#include "intrusive_list.h"

#include <compare>
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace computer_club {

struct waiting_client_tag {};
struct free_table_tag {};

struct util_values {
    constexpr static const char* time_format = "%H:%M";
};

class club_time {
public:
    int t_hour;
    int t_min;
private:
    void agressive_check_correctness() const {
        if (t_hour >= 24 || t_min >= 60 || t_hour < 0 || t_min < 0) {
            throw std::domain_error(
                "hour should be a positive number less than 24\
                and min should be a positive number less then 60"
            );
        }
    }
public:
    club_time(std::tm time) 
        : t_hour(time.tm_hour)
        , t_min(time.tm_min)
    {
        agressive_check_correctness();
    }
    
    explicit club_time(int min) 
        : t_hour(min / 60)
        , t_min(min % 60)
    {
        agressive_check_correctness();
    }

    club_time() = default;
    club_time(club_time const&) = default;
    club_time(club_time &&) = default;
    club_time & operator=(club_time const&) = default;
    club_time & operator=(club_time &&) = default;
    ~club_time() = default;

    club_time(int hour, int min) 
        : t_hour(hour) 
        , t_min(min) 
    {
        agressive_check_correctness();
    }

    club_time& operator+=(club_time const& other) {
        t_hour += other.t_hour + (other.t_min + t_min) / 60; 
        t_min = (t_min + other.t_min) % 60;
        agressive_check_correctness();
        return *this;
    }

    club_time operator-(club_time const& other) const {
        return club_time(to_mins() - other.to_mins());
    }

    std::strong_ordering operator<=>(club_time const& other) const {
        if (t_hour == other.t_hour) {
            return t_min <=> other.t_min;
        }
        return t_hour <=> other.t_hour;
    }

    std::size_t to_mins() const noexcept {
        return t_hour * 60 + t_min;
    }

    std::string to_string() const noexcept {
        std::ostringstream res;
        std::tm time;
        time.tm_hour = t_hour;
        time.tm_min = t_min;
        res << std::put_time(&time, util_values::time_format);
        return res.str();
    }
};

class client : public intrusive::list_element<waiting_client_tag> {
    std::string _name;
public:
    client() = delete;
    client(client const&) = default;
    client(client&&) = default;
    client& operator=(client const&) = default;
    client& operator=(client&&) = default;
    ~client() = default;

    client(std::string &&name) 
        : _name(std::move(name)) 
    {}

    client(std::string const&name) 
        : _name(name) 
    {}

    std::string const& get_name() const noexcept {
        return _name;
    }

    auto operator<=>(client const& other) const noexcept {
        return _name <=> other._name;
    }
};

class table : public intrusive::list_element<free_table_tag> {
    bool _is_busy;
    int _id;
    std::size_t _total_money;
    club_time _total_time;
    club_time _start_time;
public:
    table(int id) : _is_busy(false) , _id(id) , _total_money(0) , _total_time(0) , _start_time(0) {}
    table(table const&) = delete;
    table(table &&) = default;
    table& operator=(table const&) = delete;
    table& operator=(table &&) = default;
    ~table() = default;

    bool is_busy() const noexcept {
        return _is_busy;
    }

    int get_id() const noexcept {
        return _id;
    }

    std::size_t get_money() const noexcept {
        return _total_money;
    }

    club_time const& total_time() const noexcept {
        return _total_time;
    }

    void take(club_time const& time) {
        if (_is_busy) {
            throw std::logic_error("place is already busy");
        }
        _is_busy = true;
        _start_time = time;
    }


    void free(club_time const& time, std::size_t hour_cost) {
        if (!_is_busy) {
            throw std::logic_error("place is free");
        }
        _is_busy = false;
        club_time time_busy = time - _start_time;
        std::size_t busy_in_mins = time_busy.to_mins();
        _total_money += hour_cost * (busy_in_mins / 60 + (busy_in_mins % 60 ? 1 : 0));
        _total_time += time_busy;
    }
};

}