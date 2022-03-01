#include "cl-parser.hpp"

int main(int argc, char const *argv[]) {
    cl_parser::my_settings settings = cl_parser::parse_settings(argc, argv);

    cl_parser::print_settings(settings);
}
