#include "parser.h"

#pragma once

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

parser::parser(std::istream &in) : _in(in) {}

bool parser::eof() const noexcept {
    return _in.eof();
}

base_event parser::parse_event() {
    std::string str;
    std::getline(_in, str);
    if (str.empty()) {
        throw std::runtime_error("event expected, foound: EOF");
    }
    std::stringstream line(str);
    std::tm time;
    line >> std::get_time(&time, util_values::time_format);
    if (line.fail()) {
        throw std::runtime_error("couldn't parse event time in line: " + str);
    }
    int id;
    line >> id;
    if (line.fail()) {
        throw std::runtime_error("couldn't parse event id in line: " + str);
    }
    if (!base_event::is_correct_id(id)) {
        throw std::runtime_error("unknown event met in line: " + str);
    }
    std::vector<std::string> body;
    while (!line.eof()) {
        line >> str;
        body.push_back(str);
    }

    return base_event(std::move(time), static_cast<base_event::event_id>(id), std::move(body));
}

}