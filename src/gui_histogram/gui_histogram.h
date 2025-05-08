#pragma once

#include "implot.h"
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

struct PrecomputedHistogram {
    std::vector<double> binCounts;
    double binWidth;
    double minValue;
    double maxValue;
    int binCount;
};

// Pre-compute histogram data once
PrecomputedHistogram computeHistogramBins(const std::vector<int>& data, int binCount, double minRange, double maxRange);

// Efficiently draw a histogram using pre-computed bin data
void drawPrecomputedHistogram(const std::string& label, const PrecomputedHistogram& hist);

std::vector<int> readIntegerTextFile(const std::string& filename);

void drawMainHistogram(const std::string& title, const std::vector<int>& histogram, 
                       std::vector<ImPlotRect>& rects, const std::vector<ImVec4>& rectColors, 
                       PrecomputedHistogram& precomputed);

// The subset histogram can also use pre-computation for better performance
PrecomputedHistogram computeSubsetHistogram(const std::vector<int>& data, double minRange, double maxRange, int binCount);

void drawSubHistogram(const std::string& title, const std::vector<int>& mainHistogram,
                      const ImPlotRect& rect, const ImVec4& color);