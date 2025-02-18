#pragma once

#include "intrusive_list.h"
#include "event.h"
#include "types.h"


#include <cstddef>
#include <ctime>
#include <map>
#include <optional>
#include <utility>
#include <vector>
#include <list>
namespace computer_club {

class state {
    std::size_t _tables_number;
    club_time _open_time, _close_time;
    std::size_t _hour_cost;
    std::size_t _waiting_clients_count;
    std::vector<table> _tables;
    intrusive::list<table, free_table_tag> _free_tables;
    std::map<client, table*> _clients;
    intrusive::list<client, waiting_client_tag> _waiting_clients;


    static std::optional<base_event> generate_error(club_time, const char *) noexcept;

    void take_table(table &, club_time const&) noexcept;
    void free_table(table &, club_time const&) noexcept;

    std::optional<base_event> proccess_came(base_event const&);
    std::optional<base_event> proccess_awaits(base_event const&);
    std::optional<base_event> proccess_set(base_event const&);
    std::optional<base_event> proccess_left(base_event const&);

    // returns pointer to place that was held by client
    table * client_left(client const&, club_time const&);
public:
    state() = default;
    state(state const&) = delete;
    state(state &&) = default;
    state& operator=(state const&) = delete;
    state& operator=(state &&) = default;
    ~state() = default;

    state(std::size_t, club_time &&, club_time &&, std::size_t);

    club_time const& start_day() const noexcept;
    club_time const& end_day() const noexcept;

    std::vector<base_event> close() noexcept;
    std::vector<std::pair<std::size_t, club_time>> calculate_results() const;

    std::optional<base_event> proccess_event(base_event const&);

};


}