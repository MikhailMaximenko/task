#pragma once

#include "parser.h"
#include "state.h"
#include <iostream>

namespace computer_club {

class computer_club {
    std::istream &_in;
    std::ostream &_out;
    state _st;
    parser _parser;
public:
    computer_club(std::istream &, std::ostream &);
    void init_club();
    void run_club();
    void close_club();
    void make_day_results();
};

}