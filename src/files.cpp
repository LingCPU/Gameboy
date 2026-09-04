#include "files.h"
#include <fstream>

std::vector<char> readBytes(const std::string filename){
    std::ifstream stream(filename.c_str(), std::ios::binary|std::ios::ate);
    std::ifstream::pos_type position = stream.tellg();
    std::vector<char> result(position);
    stream.seekg(0, std::ios::beg);
    stream.read(&result[0], position);
    return result;
}