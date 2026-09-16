#include "files.h"
#include <fstream>
#include <stdexcept>

std::vector<char> readBytes(const std::string filename){
    std::ifstream stream(filename.c_str(), std::ios::binary | std::ios::ate); // read in binary bytes and point at end
    if(!stream) throw std::runtime_error("Unable to open file: " + filename);

    std::ifstream::pos_type position = stream.tellg(); // file size
    if(position < 0) throw std::runtime_error("Unable to determine file size: " + filename);

    std::vector<char> result(position); 
    stream.seekg(0, std::ios::beg); // point back to beginning

    if(!result.empty()){
        stream.read(result.data(), static_cast<std::streamsize>(result.size()));
        if(!stream) throw std::runtime_error("Unable to read file: " + filename);
    }
    return result;
}

void writeBytes(const std::string& filename, const std::vector<uint8_t>& bytes){
    std::ofstream stream(filename.c_str(), std::ios::binary | std::ios::trunc);
    if(!stream) throw std::runtime_error("Unable to open file for writing: " + filename);

    if(!bytes.empty()){
        stream.write(
            reinterpret_cast<const char*>(bytes.data()),
            static_cast<std::streamsize>(bytes.size())
        );
        if(!stream) throw std::runtime_error("Unable to write file: " + filename);
    }
}

bool fileExists(const std::string& filename){
    std::ifstream stream(filename.c_str(), std::ios::binary);
    return stream.good();
}