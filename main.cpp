
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "file_utils.h"
#include "mask_utils.h"
#include "gui.h"

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

int main() {
    std::string maskHex = "000000FF";
    std::string inputFile = "../../data/entropy_output.data-0001.data";
    std::string outputFile = "../../data/masked_output.bin";

    //extractMaskedBytesFromDecimals(inputFile, outputFile, maskHex);

    std::string cmd = "wsl perl /home/user/find-first-passing-decimation.pl /mnt/c/Projects/JENT_Heuristic/data/masked_output.bin 5";
    //std::string output = execCommand(cmd);

    //std::cout << "Script Output:\n" << output << std::endl;

    runGui(outputFile, inputFile);

    return 0;
}