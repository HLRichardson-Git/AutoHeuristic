
#include "file_utils.h"
#include "mask_utils.h"

#include <fstream>
#include <iostream>

void writeBigEndian32(uint32_t value, std::ostream& out) {
    out.put((value >> 24) & 0xFF);
    out.put((value >> 16) & 0xFF);
    out.put((value >> 8) & 0xFF);
    out.put(value & 0xFF);
}

void convertDecimalFileToBinaryBigEndian(const std::string& inputPath, const std::string& binaryOutputPath, unsigned int& count) {
    std::ifstream input(inputPath);
    std::ofstream output(binaryOutputPath, std::ios::binary);

    if (!input || !output) {
        std::cerr << "Error: failed to open input or output file.\n";
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        try {
            uint32_t value = std::stoul(line);
            //writeBigEndian32(value, output);
            output.put(static_cast<char>(value & 0xFF));
            output.put(static_cast<char>((value >> 8) & 0xFF));
            output.put(static_cast<char>((value >> 16) & 0xFF));
            output.put(static_cast<char>((value >> 24) & 0xFF));
            ++count;
        } catch (...) {
            std::cerr << "Error parsing line: " << line << "\n";
        }
    }

    std::cout << "Big-endian binary output written to " << binaryOutputPath << "\n";
}


void extractMaskedBytesFromDecimals(const std::string& inputPath, const std::string& outputPath, const std::string& hexMaskStr) {
    uint64_t mask = parseHexMask(hexMaskStr);

    std::ifstream input(inputPath);
    std::ofstream output(outputPath, std::ios::binary);

    if (!input || !output) {
        std::cerr << "Error: failed to open input or output file.\n";
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        try {
            uint64_t value = std::stoull(line);
            writeMaskedBytes(value, mask, output);
        } catch (...) {
            std::cerr << "Error parsing line: " << line << "\n";
        }
    }

    input.close();
    output.close();

    std::cout << "Masked output written to " << outputPath << "\n";
}

void convertAndMaskLittleEndianBinaryToBigEndian(const std::string& inputPath,
    const std::string& outputPath,
    const std::string& hexMaskStr) {
    std::ifstream input(inputPath, std::ios::binary);
    std::ofstream output(outputPath, std::ios::binary);

    if (!input || !output) {
    std::cerr << "Error: failed to open input or output file.\n";
    return;
    }

    uint64_t mask = parseHexMask(hexMaskStr);

    while (true) {
    uint8_t bytes[4];
    input.read(reinterpret_cast<char*>(bytes), 4);
    if (input.gcount() < 4) break;

    // Little-endian to uint32
    uint32_t value = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);

    // Apply mask
    uint32_t masked = value & mask;

    // Write only meaningful bytes
    if (mask == 0x000000FF) {
        output.put(static_cast<uint8_t>(masked));
    } else {
        // Otherwise, write full big-endian 32-bit
        output.put((masked >> 24) & 0xFF);
        output.put((masked >> 16) & 0xFF);
        output.put((masked >> 8) & 0xFF);
        output.put(masked & 0xFF);
    }
    }

    std::cout << "Masked binary output written to " << outputPath << "\n";
}

std::string execCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;

    // Open pipe to file
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read output of command line by line
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}