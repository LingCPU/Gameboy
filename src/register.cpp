#include "register.h"

Register::Register() : val(0){

}

void Register::set(const uint8_t byte){
    val = byte;
}

uint8_t Register::value() const{
    return val;
}

void Register::increment(){
    val++;
}
void Register::decrement(){
    val--;
}

void setPair(Register& high, Register& low, uint16_t word){
    high.set(static_cast<uint8_t>(word >> 8));  // Discard lower byte
    low.set(static_cast<uint8_t>(word & 0xFF)); // Discard upper byte
}

uint16_t pairVal(const Register& high, const Register& low){
    return static_cast<uint16_t>(high.value()) << 8 | low.value();
}

void incrementPair(Register& high, Register& low){
    uint16_t pair = pairVal(high, low);
    pair++;
    setPair(high, low, pair);
}

void decrementPair(Register& high, Register& low){
    uint16_t pair = pairVal(high, low);
    pair--;
    setPair(high, low, pair);
}