#pragma once

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
    std::tm _time;
    event_id _id; 
    std::vector<std::string> _body;
public:
    static bool is_correct_id(int) noexcept;
    base_event(std::tm &&, event_id, std::vector<std::string> &&);
    std::string to_string() const;

    event_id get_id() const noexcept;

    std::string const& operator[](std::size_t) const; 
    std::tm const& get_time() const noexcept;
};


}