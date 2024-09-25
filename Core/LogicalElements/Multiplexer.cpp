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

#include "Multiplexer.h"
#include "../../UI/imgui/Workspace.h"

LogiGates::Core::LogicalElements::Multiplexer::Multiplexer(LogiGates::UI::Workspace *workspace) : Base(workspace) {
    this->typeName = "multiplexer";

    inputPins.push_back(new Pin(this, PinType::INPUT));
    inputPins.push_back(new Pin(this, PinType::INPUT));
    numberPins.push_back(new Pin(this, PinType::INPUT));
    pins.push_back(new Pin(this, PinType::OUTPUT));
}

void LogiGates::Core::LogicalElements::Multiplexer::render() {
    ImNodes::BeginNode(id);
    ImGui::Image((ImTextureID) UI::Images::icons["multiplexer"]->texture, {70, 70});

    ImGui::PushFont(UI::Fonts::openSans24);

    if (ImGui::Button("+", {40, 40})) {
        numberPins.push_back(new Pin(this, PinType::INPUT));

        while (inputPins.size() != std::pow(2, numberPins.size())) {
            inputPins.push_back(new Pin(this, PinType::INPUT));
        }
        perform();
    }

    ImGui::SameLine();

    if (ImGui::Button("-", {40, 40})) {
        if (numberPins.size() - 1 != 0) {
            if (numberPins[numberPins.size() - 1]->getConnectedWith() != -1) {
                workspace->removeConnection(numberPins[numberPins.size() - 1]->getConnectedWith(), numberPins[numberPins.size() - 1]->getID());
            }

            delete numberPins[numberPins.size() - 1];
            numberPins.erase(std::find(numberPins.begin(), numberPins.end(), numberPins[numberPins.size() - 1]));

            while (inputPins.size() != std::pow(2, numberPins.size())) {
                if (inputPins[inputPins.size() - 1]->getConnectedWith() != -1) {
                    workspace->removeConnection(inputPins[inputPins.size() - 1]->getConnectedWith(), inputPins[inputPins.size() - 1]->getID());
                }

                delete inputPins[inputPins.size() - 1];
                inputPins.erase(std::find(inputPins.begin(), inputPins.end(), inputPins[inputPins.size() - 1]));
            }
        }

        perform();
    }

    ImGui::PopFont();

    pins[0]->render();

    ImGui::PushFont(UI::Fonts::openSans18);
    ImGui::Text("%s:", UI::Localization::localization[UI::Localization::currentLocalization]["inputnumber"].c_str());
    ImGui::PopFont();
    for (Pin *p: numberPins) {
        p->render();
    }

    ImGui::Text(" ");
    ImGui::PushFont(UI::Fonts::openSans18);
    ImGui::Text("%s:", UI::Localization::localization[UI::Localization::currentLocalization]["inputs"].c_str());
    ImGui::PopFont();
    for (Pin *p: inputPins) {
        p->render();
    }

    ImNodes::EndNode();
}

void LogiGates::Core::LogicalElements::Multiplexer::perform(std::set<int> performedIDs) {
    if (this->checkRecursion(performedIDs)) return;
    performedIDs.emplace(this->id);

    for (int i = 0; i < inputPins.size(); i++) {
        if (inputPins[i]->getConnectedWith() != -1) inputPins[i]->setState(workspace->globalPinMap[inputPins[i]->getConnectedWith()]->getState());
    }

    int number = 0;

    for (int i = numberPins.size() - 1; i != -1; i--) {
        if (numberPins[i]->getConnectedWith() != -1) numberPins[i]->setState(workspace->globalPinMap[numberPins[i]->getConnectedWith()]->getState());
        number += numberPins[i]->getState() * pow(2, i);
    }

    pins[0]->setState(inputPins[number]->getState());
    pins[0]->performNext(performedIDs);

}

LogiGates::Core::LogicalElements::Multiplexer::~Multiplexer() {
    for (Pin* pin : inputPins) {
        workspace->removeConnection(pin->getType() == PinType::OUTPUT ? pin->getID() : pin->getConnectedWith(), pin->getType() == PinType::OUTPUT ? pin->getConnectedWith() : pin->getID());
        delete pin;
    }

    for (Pin* pin : numberPins) {
        workspace->removeConnection(pin->getType() == PinType::OUTPUT ? pin->getID() : pin->getConnectedWith(), pin->getType() == PinType::OUTPUT ? pin->getConnectedWith() : pin->getID());
        delete pin;
    }

    for (Pin* pin : pins) {
        workspace->removeConnection(pin->getType() == PinType::OUTPUT ? pin->getID() : pin->getConnectedWith(), pin->getType() == PinType::OUTPUT ? pin->getConnectedWith() : pin->getID());
        delete pin;
    }
}

LogiGates::Core::LogicalElements::SaveInfo LogiGates::Core::LogicalElements::Multiplexer::getSaveInfo() {
    SaveInfo ret = Base::getSaveInfo();

    ret.additionalInfo[0] = numberPins.size();
    ret.additionalInfo[1] = inputPins.size();

    for (int i = 0; i < numberPins.size(); i++) {
        ret.connections[i] = numberPins[i]->getConnectedWith();
        ret.pinIDs[i] = numberPins[i]->getID();
        ret.pinStates[i] = numberPins[i]->getState();
    }

    for (int i = numberPins.size(); i < numberPins.size() + inputPins.size(); i++) {
        ret.connections[i] = inputPins[i - numberPins.size()]->getConnectedWith();
        ret.pinIDs[i] = inputPins[i - numberPins.size()]->getID();
        ret.pinStates[i] = inputPins[i - numberPins.size()]->getState();
    }

    ret.connections[numberPins.size() + inputPins.size()] = pins[0]->getConnectedWith();
    ret.pinIDs[numberPins.size() + inputPins.size()] = pins[0]->getID();
    ret.pinStates[numberPins.size() + inputPins.size()] = pins[0]->getState();

    return ret;
}

void LogiGates::Core::LogicalElements::Multiplexer::restoreFromSaveInfo(LogiGates::Core::LogicalElements::SaveInfo info, ImNodesEditorContext *editorCtx) {
    numberPins.resize(info.additionalInfo[0]);
    inputPins.resize(info.additionalInfo[1]);

    this->id = info.id;

    if (this->id > workspace->nodeIDCounter) {
        workspace->nodeIDCounter = this->id + 1;
    }

    for (int i = 0; i < info.additionalInfo[0]; i++) {
        if (numberPins[i] == nullptr) {
            numberPins[i] = new Pin(this, PinType::INPUT);
        }

        numberPins[i]->setID(info.pinIDs[i]);
        numberPins[i]->setState(info.pinStates[i]);
        workspace->globalPinMapOnLoad[numberPins[i]->getID()] = numberPins[i];
        if (info.connections[i] != -1) workspace->connectionQueue.push_back({numberPins[i]->getType() == PinType::OUTPUT ? numberPins[i]->getID() : info.connections[i], numberPins[i]->getType() == PinType::OUTPUT ? info.connections[i] : numberPins[i]->getID()});
    }

    for (int i = info.additionalInfo[0]; i < info.additionalInfo[1] + info.additionalInfo[0]; i++) {
        if (inputPins[i - info.additionalInfo[0]] == nullptr) {
            inputPins[i - info.additionalInfo[0]] = new Pin(this, PinType::INPUT);
        }

        inputPins[i - info.additionalInfo[0]]->setID(info.pinIDs[i]);
        inputPins[i - info.additionalInfo[0]]->setState(info.pinStates[i]);
        workspace->globalPinMapOnLoad[inputPins[i - info.additionalInfo[0]]->getID()] = inputPins[i - info.additionalInfo[0]];
        if (info.connections[i] != -1) workspace->connectionQueue.push_back({inputPins[i - info.additionalInfo[0]]->getType() == PinType::OUTPUT ? inputPins[i - info.additionalInfo[0]]->getID() : info.connections[i], inputPins[i - info.additionalInfo[0]]->getType() == PinType::OUTPUT ? info.connections[i] : inputPins[i - info.additionalInfo[0]]->getID()});
    }

    if (pins[0] == nullptr) {
        pins[0] = new Pin(this, PinType::OUTPUT);
    }

    pins[0]->setID(info.pinIDs[info.additionalInfo[1] + info.additionalInfo[0]]);
    pins[0]->setState(info.pinStates[info.additionalInfo[1] + info.additionalInfo[0]]);
    workspace->globalPinMapOnLoad[pins[0]->getID()] = pins[0];
    if (info.connections[info.additionalInfo[1] + info.additionalInfo[0]] != -1) workspace->connectionQueue.push_back({pins[0]->getType() == PinType::OUTPUT ? pins[0]->getID() : info.connections[info.additionalInfo[1] + info.additionalInfo[0]], pins[0]->getType() == PinType::OUTPUT ? info.connections[info.additionalInfo[1] + info.additionalInfo[0]] : pins[0]->getID()});


    ImNodes::EditorContextSet(editorCtx);
    ImNodes::SetNodeEditorSpacePos(this->id, {info.positionX, info.positionY});
}
