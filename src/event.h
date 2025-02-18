#pragma once
#include "types.h"
#include <cstddef>
#include <ctime>
#include <string>
#include <array>
#include <vector>
namespace computer_club {

class base_event {
public:
    static constexpr std::array<int, 7> possible_ids{1, 2, 3, 4, 11, 12, 13};
    enum class event_id : int {
        CLIENT_CAME_IN = 1,
        CLIENT_SET_IN = 2,
        CLIENT_AWAITS_IN = 3,
        CLIENT_LEFT_IN = 4,

        CLIENT_LEFT_OUT = 11,
        CLIENT_SET_OUT = 12,
        ERROR_OUT = 13
    };
private:
    club_time _time;
    event_id _id; 
    std::vector<std::string> _body;
public:
    static bool is_correct_id(int) noexcept;
    base_event(club_time &&, event_id, std::vector<std::string> &&);
    base_event(club_time const&, event_id, std::vector<std::string> const&);
    std::string to_string() const;

    event_id get_id() const noexcept;

    void validate_event() const;
    std::vector<std::string> const& get_body() const noexcept;
    std::string const& operator[](std::size_t) const; 
    club_time const& get_time() const noexcept;
};


}