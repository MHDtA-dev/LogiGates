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

#include "PreferencesWindow.h"

namespace LogiGates::UI {

    PreferencesWindow::PreferencesWindow() {
        langNamesFullToShort["English"] = "en";
        langNamesFullToShort["Русский"] = "rus";

        if (!std::filesystem::exists("preferences.lgpref")) {
            strcpy(preferences.language, "English");

            Lunada::SerializeStruct<Preferences>(preferences, "preferences.lgpref");
        }

        Lunada::DeserializeStruct<Preferences>(preferences, "preferences.lgpref");

        Localization::currentLocalization = langNamesFullToShort[preferences.language];

    }

    void PreferencesWindow::render() {
        if (visible) {
            ImGui::PushFont(Fonts::openSans20);
            ImGui::Begin(UI::Localization::localization[UI::Localization::currentLocalization]["preferences"].c_str(),
                         &visible, ImGuiWindowFlags_NoDocking);
            ImGui::PopFont();

            ImGui::PushFont(Fonts::openSans24);
            const char *items[] = {"English", "Русский"};

            ImGui::Text(Localization::localization[Localization::currentLocalization]["language"].c_str());

            ImGui::SameLine();

            if (ImGui::BeginCombo("##languageCombo", preferences.language)) {

                for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                    bool isSelected = (preferences.language == items[n]);

                    if (ImGui::Selectable(items[n], isSelected)) {
                        strcpy(preferences.language, items[n]);
                        Lunada::SerializeStruct<Preferences>(preferences, "preferences.lgpref");
                        Localization::currentLocalization = langNamesFullToShort[preferences.language];
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::PopFont();

            ImGui::End();
        }
    }

}
