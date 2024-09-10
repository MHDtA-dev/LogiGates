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

#include "AboutWindow.h"

namespace LogiGates::UI {

    void AboutWindow::render() {
        if (visible) {
            ImGui::PushFont(Fonts::openSans20);
            ImGui::Begin(UI::Localization::localization[UI::Localization::currentLocalization]["about"].c_str(),
                         &visible, ImGuiWindowFlags_NoDocking);
            ImGui::PopFont();

            float curXBefore = ImGui::GetCursorPosX();

            ImGui::Image((ImTextureID) Images::icons["logo"]->texture, {150, 150});
            ImGui::SameLine();

            float curX = ImGui::GetCursorPosX();

            ImGui::PushFont(Fonts::orbitron36);
            ImGui::Text("LogiGates");
            ImGui::PopFont();

            ImGui::SetCursorPos({curX, 75});

            ImGui::PushFont(Fonts::openSans30);
            ImGui::Text("v1.1");
            ImGui::PopFont();

            ImGui::SetCursorPos({curXBefore - 25, 160});

            ImGui::PushFont(Fonts::openSans24);

            ImGui::Text(R"(
        Copyright (C) 2024 Alexander Blinov (mhdtadev@gmail.com)

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
        )");

            ImGui::PopFont();

            ImGui::End();
        }
    }

}