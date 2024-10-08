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

#include "Switch.h"
#include "../../UI/imgui/Workspace.h"

namespace LogiGates::Core::LogicalElements {

    Switch::Switch(UI::Workspace *workspace) : Base(workspace) {
        this->typeName = "switch";

        pins.push_back(new Pin(this, PinType::OUTPUT,
                               UI::Localization::localization[UI::Localization::currentLocalization]["out"]));
    }

    void Switch::render() {
        ImNodes::BeginNode(id);
        if (ImGui::ImageButton(std::string("switch_" + std::to_string(this->id)).c_str(),
                               (ImTextureID) (pins[0]->getState() ? UI::Images::icons["switch_on"]->texture
                                                                  : UI::Images::icons["switch_off"]->texture), {70, 70},
                               {0, 0}, {1, 1})) {
            pins[0]->setState(!pins[0]->getState());
            this->perform();
        }

        for (Pin *p: pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void Switch::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;
        performedIDs.emplace(this->id);
        pins[0]->performNext(performedIDs);
    }

}
