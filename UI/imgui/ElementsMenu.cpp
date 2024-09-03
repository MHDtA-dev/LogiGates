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

#include "ElementsMenu.h"

namespace LogiGates::UI {

    ElementsMenu::ElementsMenu() {
        uiElements = {
                {
                        "not",
                        Images::icons["not"],
                },

                {
                        "and",
                        Images::icons["and"],
                },

                {
                        "or",
                        Images::icons["or"],
                },

                {
                        "xor",
                        Images::icons["xor"],
                },


                {
                        "implication",
                        Images::icons["implication"],
                },

                {
                        "equivalent",
                        Images::icons["equivalent"],
                },

                {
                        "splitter",
                        Images::icons["splitter"],
                },

                {
                        "lamp",
                        Images::icons["lamp_off"],
                },

                {
                        "switch",
                        Images::icons["switch_off"],
                },

                {
                        "fivebitnumenc",
                        Images::icons["fivebitnumenc"],
                },

                {
                        "fivebitnumdisp",
                        Images::icons["fivebitnumdisp"],
                }
        };
    }


    void ElementsMenu::render() {
        ImGui::PushFont(Fonts::openSans20);
        ImGui::Begin(Localization::localization[Localization::currentLocalization]["elements"].c_str());
        ImGui::PopFont();

        float padding = 16.f;
        float thumbSize = 128.f;
        float cellSize = thumbSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int) (panelWidth / cellSize);

        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, 0, false);


        for (Element uiElement: uiElements) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton(uiElement.name.c_str(), (ImTextureID) uiElement.texture->texture, {thumbSize, thumbSize},
                               {0, 0}, {1, 1});
            ImGui::PopStyleColor(3);

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                const char *elementName = uiElement.name.c_str();
                ImGui::SetDragDropPayload("ELEMENT", elementName, sizeof(elementName) * uiElement.name.size(),
                                          ImGuiCond_Once);
                ImGui::PushFont(Fonts::openSans20);
                ImGui::Text("%s",
                            Localization::localization[Localization::currentLocalization][uiElement.name].c_str());
                ImGui::PopFont();
                ImGui::EndDragDropSource();
            }

            ImGui::PushFont(Fonts::openSans20);
            ImGui::Text(Localization::localization[Localization::currentLocalization][uiElement.name].c_str());
            ImGui::PopFont();

            ImGui::NextColumn();
        }

        ImGui::Columns(1);

        ImGui::Dummy({70, 140});

        ImGui::End();
    }

}

