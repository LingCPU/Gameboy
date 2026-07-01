#ifndef REGISTER_H
#define REGISTER_H

#include <cstdint>

class Register{
public:
    Register();
    
    void set(const uint8_t byte);
    uint8_t value() const;

    void increment();
    void decrement();

private:
    uint8_t val;
};

void setPair(Register& high, Register& low, uint16_t word);
uint16_t pairVal(const Register& high, const Register& low);
void incrementPair(Register& high, Register& low);
void decrementPair(Register& high, Register& low);

#endif