#include "state.h"
#include "event.h"
#include "types.h"
#include <cassert>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
namespace computer_club {

state::state(std::size_t comp_number, club_time &&start, club_time &&end, std::size_t cost) 
    : _tables_number(comp_number)
    , _open_time(std::move(start))
    , _close_time(std::move(end))
    , _hour_cost(cost)
    , _waiting_clients_count(0)
    , _tables()
    , _clients()
    , _free_tables()
    , _waiting_clients()
{
    for (std::size_t i = 0; i < _tables_number; ++i) {
        _tables.emplace_back(i + 1);
        _free_tables.push_back(_tables.back());
    }
}

club_time const& state::start_day() const noexcept {
    return _open_time;
}
club_time const& state::end_day() const noexcept {
    return _close_time;
}

void state::take_table(table & _table, club_time const& time) noexcept {
    _table.take(time);
    _free_tables.erase(_free_tables.iterator_to(_table));
}

void state::free_table(table & _table, club_time const& time) noexcept {
    _table.free(time, _hour_cost);
    _free_tables.push_back(_table);
}

std::vector<base_event> state::close() noexcept {
    std::vector<base_event> result;
    std::vector<std::string> tmp;
    _waiting_clients.clear();
    for (auto it = _clients.begin(); it != _clients.end();) {
        if (it->second != nullptr && it->second->is_busy()) {
            free_table(*it->second, _close_time);
        }
        tmp.push_back(it->first.get_name());
        result.emplace_back(std::move(_close_time), base_event::event_id::CLIENT_LEFT_OUT, std::move(tmp));
        tmp.clear();
        it = _clients.erase(it);
    }
    return result;
}

std::vector<std::pair<std::size_t, club_time>> state::calculate_results() const {
    std::vector<std::pair<std::size_t, club_time>> results;
    for (auto const& table : _tables) {
        results.push_back(std::make_pair(table.get_money(), table.total_time()));
    }
    return results;
}

std::optional<base_event> state::generate_error(club_time time, const char * error) noexcept {
    std::vector<std::string> body;
    body.emplace_back(error);
    return std::optional<base_event>(std::in_place, std::move(time), base_event::event_id::ERROR_OUT, std::move(body));
}


std::optional<base_event> state::proccess_came(base_event const& event) {
    client cl(event[0]);
    if (_clients.find(cl) != _clients.end()) {
        return generate_error(event.get_time(), "YouShallNotPass");
    }
    club_time time(event.get_time());
    if (time < _open_time || time >= _close_time) {
        return generate_error(event.get_time(), "NotOpenYet");
    }
    _waiting_clients.push_back(cl);
    _clients.insert(std::make_pair(std::move(cl), nullptr));
    ++_waiting_clients_count;
    return std::nullopt;
}


std::optional<base_event> state::proccess_awaits(base_event const& event) {
    client cl(event[0]);
    auto cl_iter = _clients.find(cl);
    if (cl_iter == _clients.end()) {
        return generate_error(event.get_time(), "ClientUnknown");
    }
    if (!_free_tables.empty()) {
        return generate_error(event.get_time(), "ICantWaitNoLonger!");
    }
    if (_waiting_clients_count > _tables_number) {
        std::vector<std::string> body;
        body.push_back(event[0]);
        --_waiting_clients_count;
        auto cl_iter = _clients.find(client(event[0]));
        _waiting_clients.erase(_waiting_clients.iterator_to(cl_iter->first));
        _clients.erase(cl_iter);
        return std::optional<base_event>(std::in_place, std::move(event.get_time()), base_event::event_id::CLIENT_LEFT_OUT, std::move(body));
    }
    return std::nullopt;
}


std::optional<base_event> state::proccess_set(base_event const& event) {
    int place = std::stoi(event[1]) - 1;
    if (_tables[place].is_busy()) {
        return generate_error(event.get_time(), "PlaceIsBusy");
    }
    client cl(event[0]);
    auto cl_iter = _clients.find(cl);
    if (cl_iter == _clients.end()) {
        return generate_error(event.get_time(), "ClientUnknown");
    }
    if (cl_iter->second != nullptr) {
        free_table(*cl_iter->second, event.get_time());
    } else {
        _waiting_clients.erase(_waiting_clients.iterator_to(cl_iter->first));
        --_waiting_clients_count;
    }
    take_table(_tables[place], event.get_time());
    cl_iter->second = &_tables[place];
    return std::nullopt;
}


std::optional<base_event> state::proccess_left(base_event const& event) {
    client cl(event[0]);
    auto cl_iter = _clients.find(cl);
    if (cl_iter == _clients.end()) {
        return generate_error(event.get_time(), "ClientUnknown");
    }

    if (cl_iter->second != nullptr) {
        free_table(*cl_iter->second, event.get_time());
        if (!_waiting_clients.empty()) {
            _clients[_waiting_clients.front()] = cl_iter->second;
            std::vector<std::string> body;
            body.push_back(_waiting_clients.front().get_name());
            body.emplace_back(std::to_string(cl_iter->second->get_id()));

            _waiting_clients.pop_front();
            --_waiting_clients_count;
            take_table(*cl_iter->second, event.get_time());
            _clients.erase(cl_iter);
            
            return std::optional<base_event>(
                std::in_place, std::move(event.get_time()), 
                base_event::event_id::CLIENT_SET_OUT, 
                std::move(body));
        }
    } else {
        --_waiting_clients_count;
        _waiting_clients.erase(_waiting_clients.iterator_to(cl_iter->first));
    }
    _clients.erase(cl_iter);
    return std::nullopt;
}


std::optional<base_event> state::proccess_event(base_event const& event) {
    base_event::event_id id = event.get_id();

    if (id == base_event::event_id::CLIENT_CAME_IN) {
        return proccess_came(event);
    } else if (id == base_event::event_id::CLIENT_SET_IN) {
        return proccess_set(event);
    } else if (id == base_event::event_id::CLIENT_AWAITS_IN) {
        return proccess_awaits(event);
    } else if (id == base_event::event_id::CLIENT_LEFT_IN) {
        return proccess_left(event);
    } else {
        throw std::invalid_argument("unexpected id found while proccessing event: " + event.to_string());
    }
    assert(false);
    __builtin_unreachable();
}

}