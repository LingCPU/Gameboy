#pragma once

#include <string>
#include <vector>
#include "address.h"

const int entryPoint = 0x100;
const int logo = 0x104;
const int title = 0x134;
const int manufacturerCode = 0x13F;
const int cgbFlag = 0x143;
const int newLicenseCode = 0x144;
const int sgbFlag = 0x146;
const int cartridgeType = 0x147;
const int romSize = 0x148;
const int ramSize = 0x149;
const int destinationCode = 0x14A;
const int oldLicenseCode = 0x14B;
const int versionNumber = 0x14C;
const int headerChecksum = 0x14D;
const int globalChecksum = 0x14E;

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