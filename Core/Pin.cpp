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

#include "Pin.h"
#include "LogicalElements/Base.h"

namespace LogiGates::Core {

    Pin::Pin(LogicalElements::Base* element, LogiGates::Core::PinType type, std::string label) : element(
            element), type(type), label(label) {
        id = idCounter;
        idCounter++;

        globalPinMap[id] = this;
    }

    PinType Pin::getType() {
        return this->type;
    }

    void Pin::render() {
        if (this->type == PinType::INPUT) {
            ImNodes::BeginInputAttribute(id);
            ImGui::PushFont(UI::Fonts::openSans18);
            ImGui::Text(label.c_str());
            ImGui::PopFont();
            ImNodes::EndInputAttribute();
        } else {
            ImNodes::BeginOutputAttribute(id);
            ImGui::PushFont(UI::Fonts::openSans18);
            ImGui::Text(label.c_str());
            ImGui::PopFont();
            ImNodes::EndOutputAttribute();
        }
    }

    void Pin::connect(int id) {
        if (this->type == PinType::OUTPUT) {
            connectedWith = id;
            this->nextElement = globalPinMap[id]->element;
            globalPinMap[id]->connectedWith = this->id;
            this->element->perform();
        } else {
            connectedWith = id;
        }

    }

    int Pin::getID() {
        return this->id;
    }

    int Pin::getConnectedWith() {
        return this->connectedWith;
    }

    bool Pin::getState() {
        return this->state;
    }

    void Pin::setState(bool state) {
        this->state = state;
    }

    void Pin::performNext() {
        if (this->type == PinType::OUTPUT and this->nextElement != nullptr) {
            this->nextElement->perform();
        }
    }

    LogicalElements::Base* LogiGates::Core::Pin::getElement() {
        return this->element;
    }

    Pin::~Pin() {
        disconnect();
    }

    void Pin::disconnect(bool callInOther) {
        if (this->type == PinType::OUTPUT) {
            if (this->connectedWith != -1 and callInOther) globalPinMap[this->connectedWith]->disconnect(false);
            this->nextElement = nullptr;
            this->connectedWith = -1;
        } else {
            if (this->connectedWith != -1) {
                if (callInOther) globalPinMap[this->connectedWith]->disconnect(false);
                this->connectedWith = -1;
                this->state = false;
                this->element->perform();
            }
        }
    }

    void Pin::setID(int id) {
        this->id = id;

        if (id > idCounter) {
            idCounter = id + 1;
        }
    }

}
