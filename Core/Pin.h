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

#ifndef LOGIGATES_PIN_H
#define LOGIGATES_PIN_H

#include <iostream>
#include <set>
#include <functional>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imnodes/imnodes.h"

#include "../UI/Fonts.h"

namespace LogiGates::Core::LogicalElements {
    class Base;
}

namespace LogiGates::Core {

    enum PinType {
        INPUT, OUTPUT
    };

    class Pin {
        public:
            Pin(LogicalElements::Base* element, PinType type, std::string label = "");
            ~Pin();

            virtual void render();

            void connect(int id);
            void disconnect(bool callInOther = true);
            int getID();
            int getConnectedWith();
            void setID(int id);

            PinType getType();
            LogicalElements::Base* getElement();

            bool getState();
            void setState(bool state);

            void performNext(std::set<int> performedIDs = {}); // Only for output

            inline static std::unordered_map<int, Pin*> globalPinMap;

        private:
            LogicalElements::Base* element;
            LogicalElements::Base* nextElement = nullptr; // Only for output
            PinType type;
            std::string label;

            int connectedWith = -1;
            int id = -1;
            bool state = false;

            inline static int idCounter = 1;

    };

}

#endif //LOGIGATES_PIN_H
