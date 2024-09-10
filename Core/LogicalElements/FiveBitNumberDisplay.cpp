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

#include "FiveBitNumberDisplay.h"

namespace LogiGates::Core::LogicalElements {

    FiveBitNumberDisplay::FiveBitNumberDisplay(UI::Workspace* workspace) : Base(workspace) {
        this->typeName = "fivebitnumdisp";

        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::INPUT));
        pins.push_back(new Pin(this, PinType::INPUT));

        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
    }

    void FiveBitNumberDisplay::render() {
        ImNodes::BeginNode(id);
        ImGui::Image((ImTextureID) UI::Images::icons["fivebitnumdisp"]->texture, {120, 120});

        ImGui::PushFont(UI::Fonts::openSans24);
        ImGui::Text(std::to_string(number).c_str());
        ImGui::PopFont();

        for (Pin* p : pins) {
            p->render();
        }

        ImNodes::EndNode();
    }

    void FiveBitNumberDisplay::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;
        Pin* inputs[5] = {pins[0], pins[1], pins[2], pins[3], pins[4]};

        number = 0;

        for (int i = 0; i < 5; i++) {
            if (inputs[4 - i]->getConnectedWith() != -1) inputs[4 - i]->setState(Pin::globalPinMap[inputs[4 - i]->getConnectedWith()]->getState());
            number += inputs[4 - i]->getState() * pow(2, i);
        }

        performedIDs.emplace(this->id);

        for (int i = 5; i < pins.size(); i++) {
            pins[i]->setState(pins[i - 5]->getState());
            pins[i]->performNext(performedIDs);
        }
    }

    SaveInfo FiveBitNumberDisplay::getSaveInfo() {
        SaveInfo ret = Base::getSaveInfo();
        ret.additionalInfo[0] = number;

        return ret;
    }

    void FiveBitNumberDisplay::restoreFromSaveInfo(SaveInfo info) {
        this->number = info.additionalInfo[0];
        Base::restoreFromSaveInfo(info);
    }

}