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

class parser {
    std::istream &_in;
public:
    parser(std::istream &in);

    bool eof() const noexcept;

    base_event parse_event();

};


}