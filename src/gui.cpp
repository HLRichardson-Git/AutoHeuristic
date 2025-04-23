#include "gui.h"
#include "gui_histogram/gui_histogram.h"
#include "gui_platform/gui_platform.h"
#include "imgui.h"
#include <windows.h>
#include <array>

int runGui(const std::string& byteFilename, const std::string& decimalFilename) {
    HWND hwnd;
    if (!InitPlatformBackend(hwnd))
        return 1;

    std::vector<int> mainHistogram = readIntegerTextFile(decimalFilename);
    std::vector<ImPlotRect> dragRects;
    std::vector<ImVec4> dragRectsColors;

    bool done = false;

    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }

        if (done)
            break;

        StartImGuiFrame();
        
        ImGui::Begin("Histogram");

        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        float columnWidthFourth = availableSize.x / 4;

        // Set up two columns
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidthFourth);
        ImGui::SetColumnWidth(1, availableSize.x - columnWidthFourth);

        ImGui::Text("Range");
        if (ImGui::Button("Add Selection")) {
            dragRects.push_back(ImPlotRect(0, 30000, 0, 100));
            dragRectsColors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 0.25f)); // Red with 25% opacity
        }
        
        for (size_t i = 0; i < dragRects.size(); i++) {
            ImGui::PushID(static_cast<int>(i)); // Push unique ID for widgets
            
            // Create int variables to hold the values
            int xMin = static_cast<int>(dragRects[i].X.Min);
            int xMax = static_cast<int>(dragRects[i].X.Max);
            int intInputs[2] = {xMin, xMax};
            
            // Ensure we have a color for this rectangle
            if (i >= dragRectsColors.size()) {
                dragRectsColors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
            }
            
            // Use InputInt2 with pointers to these values
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6f);
            if (ImGui::InputInt2("##Range", intInputs)) {
                // If values changed, update the rect
                dragRects[i].X.Min = static_cast<double>(intInputs[0]);
                dragRects[i].X.Max = static_cast<double>(intInputs[1]);
            }
            
            // Add color picker on the same line
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.9f);
            ImGui::ColorEdit4("##Color", (float*)&dragRectsColors[i], 
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::PopID();
        }

        ImGui::NextColumn();

        //drawHistogram(mainHistogram, selectedRange);
        drawMainHistogram("Main Histogram", mainHistogram, dragRects, dragRectsColors);

        // Reset columns to prepare for sub-histograms
        ImGui::Columns(1);

        if (!dragRects.empty()) {
            ImGui::Separator();
            
            for (size_t i = 0; i < dragRects.size(); i++) {
                // Create title for the sub-histogram
                std::string subTitle = "Selection " + std::to_string(i + 1) + " [" + 
                                      std::to_string(static_cast<int>(dragRects[i].X.Min)) + 
                                      " - " + 
                                      std::to_string(static_cast<int>(dragRects[i].X.Max)) + "]";
                
                // Draw the sub-histogram directly using the range
                drawSubHistogram(subTitle, mainHistogram, dragRects[i], dragRectsColors[i]);
            }
        }

        ImGui::End();

        PresentFrame();
    }

    ShutdownPlatformBackend();
    return 0;
}
