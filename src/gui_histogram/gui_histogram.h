
#pragma once

#include <vector>
#include <string>

#include "implot.h"

std::vector<int> readIntegerTextFile(const std::string& filename);
//void drawHistogram(const std::vector<int>& histogram, int selectedRange[2]);
void drawMainHistogram(const std::string& title, const std::vector<int>& histogram, std::vector<ImPlotRect>& rects, const std::vector<ImVec4>& rectColors = {});
void drawHistogram(const std::string& title, const std::vector<int>& histogram, ImPlotRange range = ImPlotRange(0, 30000));