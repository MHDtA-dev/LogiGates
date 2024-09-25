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

#define DECTOBASE_IMPLEMENTATION
#include "FiveBitNumberEncoder.h"

namespace LogiGates::Core::LogicalElements {

    FiveBitNumberEncoder::FiveBitNumberEncoder(LogiGates::UI::Workspace *workspace) : Base(
            workspace) {
        this->typeName = "fivebitnumenc";

        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
        pins.push_back(new Pin(this, PinType::OUTPUT));
    }

    void FiveBitNumberEncoder::render() {
        ImNodes::BeginNode(id);

        ImGui::Image((ImTextureID) UI::Images::icons["fivebitnumenc"]->texture, {120, 120});

        ImGui::PushFont(UI::Fonts::openSans20);
        ImGui::PushItemWidth(120);
        ImGui::InputInt(std::string("##number_" + std::to_string(this->id)).c_str(), &number);
        ImGui::PopItemWidth();
        ImGui::PopFont();

        if (number > 31 or number < 0) {
            ImGui::PushFont(UI::Fonts::openSans18);
            ImGui::PushStyleColor(ImGuiCol_Text, {1, 0, 0, 1});
            ImGui::Text(UI::Localization::localization[UI::Localization::currentLocalization]["invalidnumber"].c_str());
            ImGui::PopStyleColor();
            ImGui::PopFont();
        } else {
            if (last != number) {

                perform();

                last = number;
            }
        }

        for (Pin *p: pins) {
            p->render();
        }


        ImNodes::EndNode();
    }

    void FiveBitNumberEncoder::perform(std::set<int> performedIDs) {
        if (this->checkRecursion(performedIDs)) return;

        std::string binary = decToBaseReversed(number, 2);
        performedIDs.emplace(this->id);

        for (int i = 0; i < pins.size(); i++) {
            pins[i]->setState(i < binary.size() and binary[i] == '1');
            pins[i]->performNext(performedIDs);
        }
    }

    SaveInfo FiveBitNumberEncoder::getSaveInfo() {
        SaveInfo ret = Base::getSaveInfo();
        ret.additionalInfo[0] = number;

        return ret;
    }

    void FiveBitNumberEncoder::restoreFromSaveInfo(SaveInfo info, ImNodesEditorContext* editorCtx) {
        this->number = info.additionalInfo[0];
        Base::restoreFromSaveInfo(info, editorCtx);
    }

}
