
#include "gui_histogram.h"
#include "implot.h"
#include <fstream>
#include <algorithm>

std::vector<int> readIntegerTextFile(const std::string& filename) {
    std::vector<int> data;
    std::ifstream file(filename);
    int value;
    while (file >> value) {
        data.push_back(value);
    }
    return data;
}

void drawMainHistogram(const std::string& title, const std::vector<int>& histogram, std::vector<ImPlotRect>& rects, const std::vector<ImVec4>& rectColors) {
    static ImPlotHistogramFlags histFlags = ImPlotHistogramFlags_None;
    ImPlotRange range(0, 30000);
    static int bins = 1000;

    if (ImPlot::BeginPlot(title.c_str())) {
        ImPlot::SetupAxes("Value", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotHistogram("Data", histogram.data(), histogram.size(), bins, 1.0, range, histFlags);

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

void drawSubHistogram(const std::string& title, const std::vector<int>& mainHistogram, const ImPlotRect& rect, const ImVec4& color) {
    static ImPlotHistogramFlags histFlags = ImPlotHistogramFlags_None;
    static int bins = 500; // Use fewer bins for sub-histograms

    // Create a style to match the selection color
    ImPlot::PushStyleColor(ImPlotCol_Fill, color);

    if (ImPlot::BeginPlot(title.c_str())) {
        ImPlot::SetupAxes("Value", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotHistogram("Subset", mainHistogram.data(), mainHistogram.size(), bins, 1.0, ImPlotRange(rect.X.Min, rect.X.Max), histFlags);
        ImPlot::EndPlot();
    }

    ImPlot::PopStyleColor();
}
