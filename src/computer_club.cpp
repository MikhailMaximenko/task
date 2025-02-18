#include "computer_club.h"
#include "event.h"
#include "parser.h"
#include "types.h"
#include "state.h"
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <string>

namespace computer_club {

computer_club::computer_club(std::istream & in, std::ostream & out)
    : _in(in)
    , _out(out)
    , _st()
    , _parser(_in)
{}

void computer_club::init_club() {
    std::size_t comp_number;
    std::tm open_time, close_time;
    std::size_t hour_cost;
    _in >> comp_number;
    _in >> std::get_time(&open_time, "%H:%M");
    _in >> std::get_time(&close_time, "%H:%M");
    _in >> hour_cost;
    std::string dummy;
    std::getline(_in, dummy);
    if (_in.fail()) {
        throw parse_error("couldn't read initial information");
    }
    club_time open_tm(open_time);
    club_time close_tm(close_time);
    _st = state(comp_number, std::move(open_tm), std::move(close_tm), hour_cost);
}


void computer_club::run_club() {
    
    _out << _st.start_day().to_string() << '\n';
    while (!_parser.eof()) {
        base_event event = _parser.parse_event();
        _out << event.to_string() << '\n';
        auto res = _st.proccess_event(event);
        if (res) {
            _out << res->to_string() << '\n';
        }    
    }
    
}

void computer_club::close_club() {
    for (auto const& event: _st.close()) {
        _out << event.to_string() << '\n';
    }
    _out << _st.end_day().to_string() << '\n';
}

void computer_club::make_day_results() {
    auto results = _st.calculate_results();
    for (std::size_t i = 0; i < results.size(); ++i) {
        _out << (i + 1) << " " << results[i].first << " " << results[i].second.to_string() << '\n';
    }
}

}
