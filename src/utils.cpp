#include "utils.h"

double Utils::time() {
    return glutGet(GLUT_ELAPSED_TIME);
}

#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <fstream>

std::string Utils::load(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) return std::string((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
    else throw(errno);
}
