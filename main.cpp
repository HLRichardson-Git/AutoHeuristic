
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "file_utils.h"
#include "mask_utils.h"
#include "gui.h"

int main() {
    std::string maskHex = "000000FF";
    std::string inputFile = "../../data/entropy_output.data";
    std::string binFile = "../../data/u32_output.bin";
    std::string outputFile = "../../data/masked_output.bin";

    unsigned int totalSamples = 0, decimationUpperBound = 0;
    //convertDecimalFileToBinaryBigEndian(inputFile, binFile, totalSamples);
    decimationUpperBound = totalSamples / 1000000;
    std::cout << "Total Amount of Samples: " << totalSamples << std::endl;
    std::cout << "Decimation Upper Bound: " << decimationUpperBound << std::endl;

    //extractMaskedBytesFromDecimals(inputFile, outputFile, maskHex);

    //std::string cmd = "wsl perl /home/user/find-first-passing-decimation.pl /mnt/c/Projects/JENT_Heuristic/data/masked_output.bin 5";
    //std::cout << "Finding first passing decimation script..." << std::endl;
    //std::string output = execCommand(cmd);
    //std::cout << "Perl Script Output:\n" << output << std::endl;

    runGui(outputFile, inputFile);

    return 0;
}