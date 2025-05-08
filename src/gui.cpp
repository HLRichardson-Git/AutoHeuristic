#include "gui.h"
#include "gui_histogram/gui_histogram.h"
#include "gui_platform/gui_platform.h"
#include "imgui.h"
#include "file_utils.h"
#include <windows.h>
#include <array>
#include <iostream>

int runGui(const std::string& byteFilename, const std::string& decimalFilename) {
    HWND hwnd;
    if (!InitPlatformBackend(hwnd))
        return 1;

    std::vector<int> mainHistogram = readIntegerTextFile(decimalFilename);
    std::vector<ImPlotRect> dragRects;
    std::vector<ImVec4> dragRectsColors;
    
    // Initialize precomputed histogram with default values
    PrecomputedHistogram precomputedMain;
    precomputedMain.binCount = 1000;
    precomputedMain.minValue = 0;
    precomputedMain.maxValue = 30000;
    
    // Compute histogram bins once at startup
    precomputedMain = computeHistogramBins(mainHistogram, 
                                         precomputedMain.binCount, 
                                         precomputedMain.minValue, 
                                         precomputedMain.maxValue);

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
        
        // Bin count slider
        ImGui::SliderInt("Bins", &precomputedMain.binCount, 100, 2000);
        
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

        // Use the optimized histogram drawing function with precomputed data
        drawMainHistogram("Main Histogram", mainHistogram, dragRects, dragRectsColors, precomputedMain);

        // Reset columns to prepare for sub-histograms
        ImGui::Columns(1);
        ImGui::Separator();

        if (!dragRects.empty()) {
            ImGui::Text("Sub-Histograms");
            ImGui::Columns(2, nullptr, false);  // 2 columns, no border

            for (size_t i = 0; i < dragRects.size(); ++i) {
                ImGui::PushID(static_cast<int>(i)); // Ensure unique ID
            
                int dragRectMin = static_cast<int>(dragRects[i].X.Min);
                int dragRectMax = static_cast<int>(dragRects[i].X.Max);
            
                if (ImGui::Button("Test Section")) {
                    std::string outputFile = "../../data/u32_output_range_" + std::to_string(dragRectMin) +
                                             "_" + std::to_string(dragRectMax);
                    std::string outputFileBinary = outputFile + ".bin";
            
                    std::string cmd = "wsl u32-selectrange /mnt/c/Projects/AutoHeuristic/data/u32_output.bin " +
                                      std::to_string(dragRectMin) + " " + std::to_string(dragRectMax) +
                                      " > " + outputFileBinary;
            
                    std::string output = execCommand(cmd);
                    std::cout << "u32-selectrange Output:\n" << output << std::endl;

                    // Masking step
                    std::string hexMaskStr = "000000FF";
                    std::string maskedOutputFile = outputFile + "_masked.bin";
                    convertAndMaskLittleEndianBinaryToBigEndian(outputFileBinary, maskedOutputFile, hexMaskStr);

                    std::cout << "Masked big-endian output written to: " << maskedOutputFile << "\n";

                    cmd = "wsl ea_non_iid -v " + maskedOutputFile;
                    output = execCommand(cmd);
                    std::cout << "ea_non_iid Results:\n" << output << std::endl;
                }
            
                ImGui::NextColumn();
            
                std::string subTitle = "Selection " + std::to_string(i + 1) + " [" + 
                                       std::to_string(dragRectMin) + " - " + 
                                       std::to_string(dragRectMax) + "]";
                
                drawSubHistogram(subTitle, mainHistogram, dragRects[i], dragRectsColors[i]);
                
                ImGui::Separator();
                ImGui::NextColumn();

                ImGui::PopID(); // Match PushID
            }
        }

        ImGui::End();

        // Display FPS in the title bar
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                       ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | 
                                       ImGuiWindowFlags_NoSavedSettings)) {
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();
        }

        PresentFrame();
    }

    ShutdownPlatformBackend();
    return 0;
}