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

#include "And.h"
#include "../../UI/imgui/Workspace.h"

namespace LogiGates::Core::LogicalElements {

    And::And(UI::Workspace* workspace) : Base(workspace) {
        this->typeName = "and";

        pins.push_back(new Pin(this, PinType::INPUT, "A"));
        pins.push_back(new Pin(this, PinType::INPUT, UI::Localization::localization[UI::Localization::currentLocalization]["b"]));
        pins.push_back(new Pin(this, PinType::OUTPUT, "A " + UI::Localization::localization[UI::Localization::currentLocalization]["and"] + " " + UI::Localization::localization[UI::Localization::currentLocalization]["b"]));
    }

    void And::render() {
        ImNodes::BeginNode(id);
        ImGui::Image((ImTextureID) UI::Images::icons["and"]->texture, {70, 70});

        for (Pin* p : pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void And::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;
        if (pins[0]->getConnectedWith() != -1) pins[0]->setState(workspace->globalPinMap[pins[0]->getConnectedWith()]->getState());
        if (pins[1]->getConnectedWith() != -1) pins[1]->setState(workspace->globalPinMap[pins[1]->getConnectedWith()]->getState());
        pins[2]->setState(pins[0]->getState() and pins[1]->getState());
        performedIDs.emplace(this->id);
        pins[2]->performNext(performedIDs);
    }


}