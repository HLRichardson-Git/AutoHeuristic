
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

    convertDecimalFileToBinaryBigEndian(inputFile, binFile);
    std::string cmd1 = "wsl awk '{print $2;}' < /mnt/c/Projects/AutoHeuristic/data/entropy_output.data | dec-to-u32 > large-u32.bin";
    std::string testOutput = execCommand(cmd1);
    extractMaskedBytesFromDecimals(inputFile, outputFile, maskHex);
    //extractMaskedBytesFromDecimals(inputFile, outputFile, maskHex);

    std::string cmd = "wsl perl /home/user/find-first-passing-decimation.pl /mnt/c/Projects/JENT_Heuristic/data/masked_output.bin 5";
    //std::string output = execCommand(cmd);

    //std::cout << "Script Output:\n" << output << std::endl;

    runGui(outputFile, inputFile);

    return 0;
}