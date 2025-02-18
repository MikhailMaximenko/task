#include "src/computer_club.h"
#include "src/parser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>


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
    try {
        club.init_club();
        club.run_club();
        club.close_club();
        club.make_day_results();
    } catch(computer_club::parse_error const& e) {
        std::cerr << "parse error occurred with message: " << e.what() << "\n";
        return -3;
    } catch(std::domain_error const& e) {
        std::cerr << "domain error occurred with message: " << e.what() << "\n";
        return -4;
    } catch(std::invalid_argument const& e) {
        std::cerr << "invalid arguement exception occurred with message: " << e.what() << "\n";
        return -5;
    } catch(std::logic_error const& e) {
        std::cerr << "logic error occurred with message: " << e.what() << "\n";
        return -6;
    }
    return 0;
}