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

#ifndef LOGIGATES_BASE_H
#define LOGIGATES_BASE_H

#include <iostream>
#include <set>
#include <functional>

#include "../Pin.h"
#include "../../UI/Localization.h"

#include "../../thirdparty/imgui/imgui.h"
#include "../../thirdparty/imnodes/imnodes.h"

namespace LogiGates::UI {
    class Workspace;
}

namespace LogiGates::Core::LogicalElements {

    struct SaveInfo {
        int id;
        float positionX, positionY;
        int connections[255];
        int pinIDs[255];
        bool pinStates[255];
        int additionalInfo[20];
        char elementType[30];
    };

    class Base {
        public:
            Base(UI::Workspace* workspace);
            ~Base();

            virtual void perform(std::set<int> performedIDs = {});
            virtual void render();

            int getID();

            virtual SaveInfo getSaveInfo();
            virtual void restoreFromSaveInfo(SaveInfo info);

            std::string getTypeName();

        protected:
            UI::Workspace* workspace;

            int id = -1;
            std::vector<Pin*> pins;
            inline static int nodeIDCounter = 1;

            std::string typeName;

            bool checkRecursion(std::set<int> performedIDs);

    };

}

#endif //LOGIGATES_BASE_H
