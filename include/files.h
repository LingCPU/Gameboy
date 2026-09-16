#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>
#include <cstdint>

std::vector<char> readBytes(const std::string filename);
void writeBytes(const std::string& filename, const std::vector<uint8_t>& bytes);
bool fileExists(const std::string& filename);

#endif