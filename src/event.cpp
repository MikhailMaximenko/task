#include "event.h"
#include <cassert>
#include <ctime>
#include <string>

namespace computer_club {

base_event::base_event(club_time &&time, event_id id, std::vector<std::string> &&body)
    : _time(std::move(time))
    , _id(id)
    , _body(std::move(body))
{
    assert(is_correct_id(static_cast<int>(id)));    
}

base_event::base_event(club_time const&time, event_id id, std::vector<std::string> const&body)
    : _time(time)
    , _id(id)
    , _body(body)
{
    assert(is_correct_id(static_cast<int>(id)));    
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
        res += elem;
    }
    return res;
}


base_event::event_id base_event::get_id() const noexcept {
    return _id;
}

std::string const& base_event::operator[](std::size_t ind) const {
    return _body[ind];
}

club_time const& base_event::get_time() const noexcept {
    return _time;
}

}