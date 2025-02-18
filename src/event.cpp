#include "event.h"
#include <cassert>
#include <ctime>
#include <stdexcept>
#include <string>

namespace computer_club {

void base_event::validate_event() const {
    if (!is_correct_id(static_cast<int>(_id))) {
        throw std::domain_error("unknown id");
    }
    if (_id == event_id::CLIENT_AWAITS_IN || 
        _id == event_id::CLIENT_LEFT_IN || 
        _id == event_id::CLIENT_CAME_IN || 
        _id == event_id::ERROR_OUT || 
        _id == event_id::CLIENT_LEFT_OUT) {
        if (_body.size() != 1) {
            throw std::domain_error("expected 1 element in event body: " + to_string());
        }
    } else {
        if (_body.size() != 2) {
            throw std::domain_error("expected 2 elements in event body: " + to_string());
        }
    }
}

base_event::base_event(club_time &&time, event_id id, std::vector<std::string> &&body)
    : _time(std::move(time))
    , _id(id)
    , _body(std::move(body))
{
    validate_event();
}

base_event::base_event(club_time const&time, event_id id, std::vector<std::string> const&body)
    : _time(time)
    , _id(id)
    , _body(body)
{
    validate_event();
}

bool base_event::is_correct_id(int id) noexcept {
    for (auto e : possible_ids) {
        if (e == id) {
            return true;
        }
    }
    return false;
}

std::string base_event::to_string() const {
    std::string res(_time.to_string());
    int id = static_cast<int>(_id);
    res += " " + std::to_string(id);
    for (auto const& elem : _body) {
        res += " " + elem;
    }
    return res;
}


base_event::event_id base_event::get_id() const noexcept {
    return _id;
}

std::vector<std::string> const& base_event::get_body() const noexcept {
    return _body;
}

std::string const& base_event::operator[](std::size_t ind) const {
    return _body[ind];
}

club_time const& base_event::get_time() const noexcept {
    return _time;
}

}