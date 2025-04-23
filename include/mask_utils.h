
#pragma once

#include <string>
#include <cstdint>
#include <fstream>

uint64_t parseHexMask(const std::string& hexStr);
void writeMaskedBytes(uint64_t value, uint64_t mask, std::ofstream& out);
