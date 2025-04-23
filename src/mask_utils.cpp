
#include "mask_utils.h"
#include <sstream>

uint64_t parseHexMask(const std::string& hexStr) {
    if (hexStr.length() > 16) throw std::invalid_argument("Mask exceeds 64 bits (max 16 hex digits).");

    uint64_t mask = 0;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> mask;
    return mask;
}

void writeMaskedBytes(uint64_t value, uint64_t mask, std::ofstream& out) {
    value &= mask;
    for (int i = 7; i >= 0; --i) {
        uint8_t maskByte = (mask >> (i * 8)) & 0xFF;
        if (maskByte) {
            uint8_t byte = (value >> (i * 8)) & 0xFF;
            out.write(reinterpret_cast<const char*>(&byte), 1);
        }
    }
}
