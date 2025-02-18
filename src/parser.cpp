#include "parser.h"

#include "event.h"
#include "types.h"

#include <ctime>
#include <iomanip>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace computer_club {

parse_error::parse_error(const char* msg) 
    : message(msg) 
{}

parse_error::parse_error(std::string const& msg) 
    : message(msg) 
{}


const char* parse_error::what() const throw() {
    return message.c_str();
}

parser::parser(std::istream &in) : _in(in) {}

bool parser::eof() const noexcept {
    return _in.eof();
}

base_event parser::parse_event() {
    std::string str;
    std::getline(_in, str);
    if (str.empty()) {
        throw parse_error("event expected, found: EOF");
    }
    std::stringstream line(str);
    std::tm time;
    line >> std::get_time(&time, util_values::time_format);
    if (line.fail()) {
        throw parse_error("couldn't parse event time in line: " + str);
    }
    int id;
    line >> id;
    if (line.fail()) {
        throw parse_error("couldn't parse event id in line: " + str);
    }
    if (!base_event::is_correct_id(id)) {
        throw std::domain_error("unknown event id met in line: " + str);
    }
    std::vector<std::string> body;
    while (!line.eof()) {
        line >> str;
        body.push_back(str);
    }

    return base_event(std::move(club_time(time.tm_hour, time.tm_min)), static_cast<base_event::event_id>(id), std::move(body));
}

}