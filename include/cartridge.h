#pragma once

#include <string>
#include <vector>
#include "address.h"

namespace cartridgeHeader{
    inline constexpr std::size_t entryPoint = 0x100;
    inline constexpr std::size_t logo = 0x104;
    inline constexpr std::size_t title = 0x134;
    inline constexpr std::size_t manufacturerCode = 0x13F;
    inline constexpr std::size_t cgbFlag = 0x143;
    inline constexpr std::size_t newLicenseCode = 0x144;
    inline constexpr std::size_t sgbFlag = 0x146;
    inline constexpr std::size_t cartridgeType = 0x147;
    inline constexpr std::size_t romSize = 0x148;
    inline constexpr std::size_t ramSize = 0x149;
    inline constexpr std::size_t destinationCode = 0x14A;
    inline constexpr std::size_t oldLicenseCode = 0x14B;
    inline constexpr std::size_t versionNumber = 0x14C;
    inline constexpr std::size_t headerChecksum = 0x14D;
    inline constexpr std::size_t globalChecksum = 0x14E;
}

class Cartridge{
public:
    Cartridge(std::string filename);

    uint8_t read(const Address address) const;

    // Header info from Cartridge
    std::string gameTitle() const;
    std::string gameDesignation() const;

private:
    std::vector<char> data;
};