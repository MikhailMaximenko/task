#include "src/computer_club.h"

#include <fstream>
#include <iostream>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "wrong arguments number\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    
    if (input.fail()) {
        std::cerr << "couldn't open file: " << argv[1] << '\n';
        return -2;
    }
    
    computer_club::computer_club club(input, std::cout);
    club.init_club();
    club.run_club();
    club.close_club();
    club.make_day_results();

    return 0;
}