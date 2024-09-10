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

#include "Equivalent.h"

namespace LogiGates::Core::LogicalElements {

    Equivalent::Equivalent(LogiGates::UI::Workspace *workspace) : Base(workspace) {
        this->typeName = "equivalent";

        pins.push_back(new Pin(this, PinType::INPUT, "A"));
        pins.push_back(new Pin(this, PinType::INPUT,
                               UI::Localization::localization[UI::Localization::currentLocalization]["b"]));
        pins.push_back(new Pin(this, PinType::OUTPUT,
                               "A == " + UI::Localization::localization[UI::Localization::currentLocalization]["b"]));
    }

    void Equivalent::render() {
        ImNodes::BeginNode(id);
        ImGui::Image((ImTextureID) UI::Images::icons["equivalent"]->texture, {70, 70});

        for (Pin *p: pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void Equivalent::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;
        if (pins[0]->getConnectedWith() != -1)
            pins[0]->setState(Pin::globalPinMap[pins[0]->getConnectedWith()]->getState());
        if (pins[1]->getConnectedWith() != -1)
            pins[1]->setState(Pin::globalPinMap[pins[1]->getConnectedWith()]->getState());
        pins[2]->setState(pins[0]->getState() == pins[1]->getState());
        performedIDs.emplace(this->id);
        pins[2]->performNext(performedIDs);
    }

}