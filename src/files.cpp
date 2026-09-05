#include "files.h"
#include <fstream>

std::vector<char> readBytes(const std::string filename){
    std::ifstream stream(filename.c_str(), std::ios::binary|std::ios::ate); // read in binary bytes and point at end
    std::ifstream::pos_type position = stream.tellg(); // file size
    std::vector<char> result(position); 
    stream.seekg(0, std::ios::beg); // point back to beginning
    stream.read(&result[0], position);
    return result;
}