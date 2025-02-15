#include <ctime>
#include <cstddef>
#include <fstream>
#include <iomanip>
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
    
    std::size_t comp_number;
    std::tm open_time, close_time;
    std::size_t hour_cost;
    input >> comp_number;
    input >> std::get_time(&open_time, "%H:%M");
    input >> std::get_time(&close_time, "%H:%M");
    input >> hour_cost;
    if (input.fail()) {
        std::cerr << "couldn't read initial information\n";
        return -3;    
    }

    return 0;
}