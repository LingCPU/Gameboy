#include "address.h"

Address::Address(uint16_t loc) : addr(loc){
    
}

uint16_t Address::value() const{
    return addr;
}