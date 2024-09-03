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

#ifndef LOGIGATES_WORKSPACE_H
#define LOGIGATES_WORKSPACE_H

#include <iostream>
#include <vector>
#include <functional>

#include "Drawable.h"
#include "../Renderer.h"
#include "../Localization.h"

#include "../../Core/LogicalElements/Base.h"
#include "../../Core/Pin.h"
#include "../../Core/LogicalElements/LogicalElements.h"

#include "../../thirdparty/imnodes/imnodes.h"
#include "../../thirdparty/lunada/lunada.h"

namespace LogiGates::UI {

    class Workspace : public Drawable {
        public:
            Workspace();
            ~Workspace();

            void render() override;
            void connect(int start, int end);
            void disconnect(int pin);
            void removeConnection(int start, int end);
            void addElement(Core::LogicalElements::Base* element);

            void saveWorkspace(std::string filename);
            void loadWorkspace(std::string filename);

            bool deleteAction = false;
            void contextMenuDelete();

            std::vector<std::pair<int, int>> connectionQueue;
            std::unordered_map<int, Core::Pin*> globalPinMapOnLoad;

        private:
            std::vector<Core::LogicalElements::Base*> elements;
            std::vector<std::pair<int, int>> connections;

            std::unordered_map<std::string, std::function<void()>> addElementFuncs;

    };

}

#endif //LOGIGATES_WORKSPACE_H
