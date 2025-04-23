
#pragma once

#include <vector>
#include <string>

void drawDecimalHistogram(const std::vector<int>& histogram, int selectedRange[2]);
void renderHistogram(const std::string& filename, int selectedRange[2]);
std::vector<int> readIntegerTextFile(const std::string& filename);
