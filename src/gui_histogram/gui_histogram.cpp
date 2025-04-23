
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

void drawDecimalHistogram(const std::vector<int>& histogram, int selectedRange[2]) {
    static ImPlotHistogramFlags histFlags = ImPlotHistogramFlags_None;
    static int bins = 1000;
    static double x1 = selectedRange[0], x2 = selectedRange[1], y1 = 0.0, y2 = 0.0;
    const int minWidth = 1;

    if (ImPlot::BeginPlot("U32 Histogram")) {
        ImPlot::SetupAxes("Value", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotHistogram("Data", histogram.data(), histogram.size(), bins, 1.0, ImPlotRange(0, 30000), histFlags);

        ImPlotRect plotLimits = ImPlot::GetPlotLimits();
        y1 = plotLimits.Y.Min;
        y2 = plotLimits.Y.Max;

        double prevX1 = x1, prevX2 = x2;

        if (ImPlot::DragRect(0, &x1, &y1, &x2, &y2, ImVec4(1, 1, 0, 0.25f), ImPlotDragToolFlags_None)) {
            if ((x2 - x1) < minWidth) {
                x1 = (x1 != prevX1) ? x2 - minWidth : x1;
                x2 = (x2 != prevX2) ? x1 + minWidth : x2;
            }

            selectedRange[0] = static_cast<int>(x1);
            selectedRange[1] = static_cast<int>(x2);
        }

        ImPlot::EndPlot();
    }

    if ((selectedRange[1] - selectedRange[0]) < minWidth)
        selectedRange[1] = selectedRange[0] + minWidth;

    x1 = static_cast<double>(selectedRange[0]);
    x2 = static_cast<double>(selectedRange[1]);

    if (!histogram.empty() && ImPlot::BeginPlot("Zoomed In")) {
        ImPlot::SetupAxes("Value (Zoomed)", "Frequency", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotHistogram("Filtered", histogram.data(), histogram.size(), bins, 1.0, ImPlotRange(selectedRange[0], selectedRange[1]), histFlags);
        ImPlot::EndPlot();
    }
}

void renderHistogram(const std::string& filename, int selectedRange[2]) {
    static std::vector<int> histogram;
    static bool histogramLoaded = false;

    if (!histogramLoaded) {
        histogram = readIntegerTextFile(filename);
        histogramLoaded = true;
    }

    drawDecimalHistogram(histogram, selectedRange);
}
