
#pragma once

#include <vector>
#include <string>

#include "implot.h"

std::vector<int> readIntegerTextFile(const std::string& filename);

void drawMainHistogram(const std::string& title, const std::vector<int>& histogram, std::vector<ImPlotRect>& rects, const std::vector<ImVec4>& rectColors = {});
void drawSubHistogram(const std::string& title, const std::vector<int>& mainHistogram, const ImPlotRect& rect, const ImVec4& color);