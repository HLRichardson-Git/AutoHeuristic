
#include "file_utils.h"
#include "mask_utils.h"

#include <fstream>
#include <iostream>

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
