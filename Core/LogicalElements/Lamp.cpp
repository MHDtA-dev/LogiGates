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

#include "Lamp.h"
#include "../../UI/imgui/Workspace.h"

namespace LogiGates::Core::LogicalElements {

    Lamp::Lamp(UI::Workspace *workspace) : Base(workspace) {
        this->typeName = "lamp";

        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
    }

    void Lamp::render() {
        ImNodes::BeginNode(id);
        ImGui::Image((ImTextureID) (pins[0]->getState() ? UI::Images::icons["lamp_on"]->texture
                                                        : UI::Images::icons["lamp_off"]->texture), {70, 70});

        for (Pin *p: pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void Lamp::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;
        if (pins[0]->getConnectedWith() != -1)
            pins[0]->setState(Pin::globalPinMap[pins[0]->getConnectedWith()]->getState());
        pins[1]->setState(pins[0]->getState());
        performedIDs.emplace(this->id);
        pins[1]->performNext(performedIDs);
    }

}
