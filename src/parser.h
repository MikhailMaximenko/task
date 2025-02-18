#pragma once

#include "event.h"
#include "types.h"

#include <ctime>
#include <exception>
#include <iomanip>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace computer_club {

class parse_error : public std::exception {
    std::string message;
public:
    parse_error(const char *);
    parse_error(std::string const&);
    const char * what() const throw() override;

};

class parser {
    std::istream &_in;
public:
    parser(std::istream &in);

    bool eof() const noexcept;

    base_event parse_event();

};


}