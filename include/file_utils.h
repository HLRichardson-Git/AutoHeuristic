
#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <array>

void convertDecimalFileToBinaryBigEndian(const std::string& inputPath, const std::string& binaryOutputPath, unsigned int& count);
void extractMaskedBytesFromDecimals(const std::string& inputPath, const std::string& outputPath, const std::string& hexMaskStr);
void convertAndMaskLittleEndianBinaryToBigEndian(const std::string& inputPath, const std::string& outputPath, const std::string& hexMaskStr);
std::string execCommand(const std::string& cmd);