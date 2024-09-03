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

#include "Splitter.h"
#include "../../UI/imgui/Workspace.h"

namespace LogiGates::Core::LogicalElements {

    Splitter::Splitter(UI::Workspace *workspace) : Base(workspace) {
        this->typeName = "splitter";

        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
    }

    void Splitter::render() {
        ImNodes::BeginNode(id);
        ImGui::Image((ImTextureID) UI::Images::icons["splitter"]->texture, {70, 70});

        ImGui::PushFont(UI::Fonts::openSans24);

        if (ImGui::Button("+", {40, 40})) {
            pins.push_back(new Pin(this, PinType::OUTPUT));
            perform();
        }

        ImGui::SameLine();

        if (ImGui::Button("-", {40, 40})) {
            if (pins[pins.size() - 1]->getType() != PinType::INPUT and pins.size() - 1 != 1) {
                if (pins[pins.size() - 1]->getConnectedWith() != -1)
                    workspace->removeConnection(pins[pins.size() - 1]->getID(),
                                                pins[pins.size() - 1]->getConnectedWith());
                delete pins[pins.size() - 1];
                pins.erase(std::find(pins.begin(), pins.end(), pins[pins.size() - 1]));
            }

            perform();
        }

        ImGui::PopFont();

        for (Pin *p: pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void Splitter::perform() {
        if (pins[0]->getConnectedWith() != -1)
            pins[0]->setState(Pin::globalPinMap[pins[0]->getConnectedWith()]->getState());

        for (int i = 1; i < pins.size(); i++) {
            pins[i]->setState(pins[0]->getState());
            pins[i]->performNext();
        }
    }

    SaveInfo Splitter::getSaveInfo() {
        SaveInfo ret = Base::getSaveInfo();
        ret.additionalInfo[0] = pins.size() - 1;

        return ret;
    }

    void Splitter::restoreFromSaveInfo(SaveInfo info) {

        for (int i = 0; i < info.additionalInfo[0] - 1; i++) {
            pins.push_back(new Pin(this, PinType::OUTPUT));
        }

        Base::restoreFromSaveInfo(info);
    }

}
