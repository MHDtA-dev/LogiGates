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

#include "Base.h"
#include "../../UI/imgui/Workspace.h"

namespace LogiGates::Core::LogicalElements {

    Base::Base(UI::Workspace* workspace) : workspace(workspace) {
        id = nodeIDCounter;
        nodeIDCounter++;
    }

    void Base::perform(std::set<int> performedIDs) {

    }

    void Base::render() {
        ImNodes::BeginNode(id);
        ImGui::Dummy(ImVec2(80.0f, 45.0f));
        ImNodes::EndNode();
    }

    int Base::getID() {
        return this->id;
    }

    Base::~Base() {
        for (Pin* pin : pins) {
            workspace->removeConnection(pin->getType() == PinType::OUTPUT ? pin->getID() : pin->getConnectedWith(), pin->getType() == PinType::OUTPUT ? pin->getConnectedWith() : pin->getID());
            delete pin;
        }
    }

    SaveInfo Base::getSaveInfo() {
        SaveInfo ret;
        ret.id = this->id;

        for (int i = 0; i < pins.size(); i++) {
            ret.connections[i] = pins[i]->getConnectedWith();
            ret.pinIDs[i] = pins[i]->getID();
            ret.pinStates[i] = pins[i]->getState();
        }

        ImVec2 nodePos = ImNodes::GetNodeEditorSpacePos(this->id);
        ret.positionX = nodePos.x;
        ret.positionY = nodePos.y;

        strcpy(ret.elementType, this->typeName.c_str());

        return ret;
    }

    void Base::restoreFromSaveInfo(SaveInfo info) {
        this->id = info.id;

        if (this->id > nodeIDCounter) {
            nodeIDCounter = this->id + 1;
        }

        for (int i = 0; i < 255; i++) {
            if (i == pins.size()) break;

            pins[i]->setID(info.pinIDs[i]);
            pins[i]->setState(info.pinStates[i]);
            workspace->globalPinMapOnLoad[pins[i]->getID()] = pins[i];
            if (info.connections[i] != -1) workspace->connectionQueue.push_back({pins[i]->getType() == PinType::OUTPUT ? pins[i]->getID() : info.connections[i], pins[i]->getType() == PinType::OUTPUT ? info.connections[i] : pins[i]->getID()});
        }

        ImNodes::SetNodeEditorSpacePos(this->id, {info.positionX, info.positionY});
    }

    std::string Base::getTypeName() {
        return this->typeName;
    }

    bool Base::checkRecursion(std::set<int> performedIDs) {
        if (performedIDs.find(this->id) != performedIDs.end()) {
            this->workspace->enableRecursionWarning();
            return true;
        }

        return false;
    }

}