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

#include "Welcome.h"

void LogiGates::UI::Welcome::render() {
    if (visible) {
        ImGui::PushFont(Fonts::openSans20);
        ImGui::Begin(UI::Localization::localization[UI::Localization::currentLocalization]["welcome"].c_str(), &visible);
        ImGui::PopFont();

        float halfWidth = ImGui::GetWindowWidth() / 2.f;
        float halfHeight = ImGui::GetWindowHeight() / 2.f;
        ImGui::SetCursorPosX(halfWidth - 80.f);
        ImGui::SetCursorPosY(50);
        ImGui::Image((ImTextureID) Images::icons["logo"]->texture, {160, 160});

        ImGui::PushFont(Fonts::orbitron36);
        ImGui::SetCursorPosX(halfWidth - (ImGui::CalcTextSize("LogiGates").x / 2.f));
        ImGui::Text("LogiGates");
        ImGui::PopFont();

        ImGui::PushFont(Fonts::openSans18);
        ImGui::SetCursorPosX(halfWidth - (ImGui::CalcTextSize("v1.2").x / 2.f));
        ImGui::Text("v1.2");
        ImGui::PopFont();

        ImGui::PushFont(Fonts::openSans24);
        ImGui::Text(" ");
        ImGui::SetCursorPosX(halfWidth - (ImGui::CalcTextSize(UI::Localization::localization[UI::Localization::currentLocalization]["welcomeText"].c_str()).x / 2.f));
        ImGui::Text("%s", UI::Localization::localization[UI::Localization::currentLocalization]["welcomeText"].c_str());
        ImGui::PopFont();



        ImGui::End();
    }
}
