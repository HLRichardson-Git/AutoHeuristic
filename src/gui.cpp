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

    int selectedRange[2] = {0, 30000};
    std::vector<std::array<int, 2>> selectedRanges;

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
            selectedRanges.push_back(std::array<int, 2>{0, 30000});
        }
        
        for (std::array<int, 2>& pair : selectedRanges) {
            ImGui::InputInt2("Range", pair.data());
        }

        ImGui::NextColumn();

        renderHistogram(decimalFilename, selectedRange);

        ImGui::End();

        PresentFrame();
    }

    ShutdownPlatformBackend();
    return 0;
}
