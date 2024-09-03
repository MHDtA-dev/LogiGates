/*
    Copyright (C) 2024 Alexander Blinov

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "Dockspace.h"

namespace LogiGates::UI {

    Dockspace::Dockspace(Workspace* workspace) : workspace(workspace) {
        preferencesWindow = new PreferencesWindow();
        aboutWindow = new AboutWindow();
    }

    void Dockspace::render() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, 0));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 70));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 5));

        ImGui::Begin("Bar", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                                     | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar);
        ImGui::PopStyleVar(3);

        ImGui::SetCursorPosY(5);

        ImGui::Image((ImTextureID) Images::icons["logo"]->texture, {60, 60});

        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();

        ImGui::PushFont(Fonts::orbitron24);
        ImGui::SetCursorPosY(23);
        ImGui::Text("LogiGates");
        ImGui::PopFont();

        ImGui::End();


        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, 70));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushFont(Fonts::openSans20);
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);
        ImGui::PopFont();

        ImGuiID dockspace_id = ImGui::GetID("DockS");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::PushFont(Fonts::openSans20);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::PopFont();

        ImGui::PushFont(Fonts::openSans24);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu(Localization::localization[Localization::currentLocalization]["file"].c_str())) {
                if (ImGui::MenuItem(Localization::localization[Localization::currentLocalization]["save"].c_str())) {
                    const char* filter[1] = {"*.logigates"};
                    const char* selection = tinyfd_saveFileDialog("Save file", nullptr, 1, filter, nullptr);

                    if (selection != nullptr) {
                        workspace->saveWorkspace(std::string(selection) + ".logigates");
                    }
                }

                if (ImGui::MenuItem(Localization::localization[Localization::currentLocalization]["load"].c_str())) {
                    const char* filter[1] = {"*.logigates"};
                    const char* selection = tinyfd_openFileDialog("Save file", nullptr, 1, filter, nullptr, false);

                    if (selection != nullptr) {
                        workspace->loadWorkspace(std::string(selection));
                    }
                }

                ImGui::Separator();

                if (ImGui::MenuItem(Localization::localization[Localization::currentLocalization]["preferences"].c_str())) {
                    preferencesWindow->visible = true;
                }

                if (ImGui::MenuItem(Localization::localization[Localization::currentLocalization]["about"].c_str())) {
                    aboutWindow->visible = true;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::PopFont();

        ImGui::End();

        preferencesWindow->render();
        aboutWindow->render();
    }
}