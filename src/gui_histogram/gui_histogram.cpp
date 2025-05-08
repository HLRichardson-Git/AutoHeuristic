#include "gui_histogram.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

PrecomputedHistogram computeHistogramBins(const std::vector<int>& data, int binCount, double minRange, double maxRange) {
    PrecomputedHistogram result;
    result.binCount = binCount;
    result.minValue = minRange;
    result.maxValue = maxRange;
    result.binWidth = (maxRange - minRange) / binCount;
    result.binCounts.resize(binCount, 0);
    
    // Count values in each bin
    for (int value : data) {
        if (value >= minRange && value < maxRange) {
            int binIndex = static_cast<int>((value - minRange) / result.binWidth);
            if (binIndex >= 0 && binIndex < binCount) {
                result.binCounts[binIndex]++;
            }
        }
    }
    
    return result;
}

void drawPrecomputedHistogram(const std::string& label, const PrecomputedHistogram& hist) {
    // Create x-values at the center of each bin
    std::vector<double> xs(hist.binCount);
    for (int i = 0; i < hist.binCount; i++) {
        xs[i] = hist.minValue + (i + 0.5) * hist.binWidth;
    }
    
    // Plot the histogram as a bar chart
    ImPlot::PlotBars(label.c_str(), xs.data(), hist.binCounts.data(), hist.binCount, hist.binWidth);
}


std::vector<int> readIntegerTextFile(const std::string& filename) {
    std::vector<int> data;
    std::ifstream file(filename);
    int value;
    while (file >> value) {
        data.push_back(value);
    }
    return data;
}

void drawMainHistogram(const std::string& title, const std::vector<int>& histogram, 
                       std::vector<ImPlotRect>& rects, const std::vector<ImVec4>& rectColors,
                       PrecomputedHistogram& precomputed) {
    static bool histogramComputed = false;
    static int lastBinCount = 1000;
    static double lastMinRange = 0;
    static double lastMaxRange = 30000;
    
    // Only recompute when parameters change
    if (!histogramComputed || lastBinCount != precomputed.binCount || 
        lastMinRange != precomputed.minValue || lastMaxRange != precomputed.maxValue) {
        
        precomputed = computeHistogramBins(histogram, precomputed.binCount, 
                                          precomputed.minValue, precomputed.maxValue);
        
        histogramComputed = true;
        lastBinCount = precomputed.binCount;
        lastMinRange = precomputed.minValue;
        lastMaxRange = precomputed.maxValue;
    }
    
    if (ImPlot::BeginPlot(title.c_str())) {
        ImPlot::SetupAxes("Value", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        
        // Draw precomputed histogram as bars
        drawPrecomputedHistogram("Data", precomputed);
        
        // Draw rectangles if provided
        for (size_t i = 0; i < rects.size(); i++) {
            // Make sure the DragRect is as tall as the Histogram
            ImPlotRect plotLimits = ImPlot::GetPlotLimits();
            rects[i].Y.Min = plotLimits.Y.Min;
            rects[i].Y.Max = plotLimits.Y.Max;
            ImPlot::DragRect(i, 
                            &rects[i].X.Min, &rects[i].Y.Min,
                            &rects[i].X.Max, &rects[i].Y.Max,
                            rectColors[i], 
                            ImPlotDragToolFlags_None);
        }
        
        ImPlot::EndPlot();
    }
}

PrecomputedHistogram computeSubsetHistogram(const std::vector<int>& data, double minRange, double maxRange, int binCount) {
    // Create a histogram specifically for the subset of data within the range
    std::vector<int> filteredData;
    filteredData.reserve(data.size() / 10); // Reserve some space as optimization
    
    for (const int& value : data) {
        if (value >= minRange && value < maxRange) {
            filteredData.push_back(value);
        }
    }
    
    return computeHistogramBins(filteredData, binCount, minRange, maxRange);
}

void drawSubHistogram(const std::string& title, const std::vector<int>& mainHistogram,
                      const ImPlotRect& rect, const ImVec4& color) {
    static std::unordered_map<std::string, PrecomputedHistogram> cachedSubHistograms;
    
    // Create a unique key for this sub-histogram
    std::string cacheKey = title + "_" + std::to_string(rect.X.Min) + "_" + std::to_string(rect.X.Max);
    
    // Check if we already computed this sub-histogram
    if (cachedSubHistograms.find(cacheKey) == cachedSubHistograms.end()) {
        // If not in cache, compute it
        cachedSubHistograms[cacheKey] = computeSubsetHistogram(mainHistogram, rect.X.Min, rect.X.Max, 500);
    }
    
    // Push style colors
    ImPlot::PushStyleColor(ImPlotCol_Fill, color);
    
    if (ImPlot::BeginPlot(title.c_str())) {
        ImPlot::SetupAxes("Value", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        
        // Draw the cached sub-histogram
        drawPrecomputedHistogram("Subset", cachedSubHistograms[cacheKey]);
        
        ImPlot::EndPlot();
    }
    
    ImPlot::PopStyleColor();
}