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
#include <set>
#include <functional>
#include <cstring>

#include "Drawable.h"
#include "../Renderer.h"
#include "../Localization.h"

#include "../../Core/LogicalElements/Base.h"
#include "../../Core/Pin.h"
#include "../../Core/LogicalElements/LogicalElements.h"

#include "../../thirdparty/imnodes/imnodes.h"
#include "../../thirdparty/lunada/lunada.h"
#include "../../thirdparty/imgui/imgui_internal.h"

namespace LogiGates::UI {

    class Dockspace;

    class Workspace : public Drawable {
        public:
            Workspace(std::string name);
            ~Workspace();

            void render() override;
            void connect(int start, int end);
            void disconnect(int pin);
            void removeConnection(int start, int end);
            void addElement(Core::LogicalElements::Base* element);

            void enableRecursionWarning();

            void save(std::string filename);
            void load(std::string filename);

            bool deleteAction = false;
            void contextMenuDelete();

            std::string getName();
            void setName(std::string name);

            void clear();

            bool showWindow = true;

            std::vector<std::pair<int, int>> connectionQueue;
            std::unordered_map<int, Core::Pin*> globalPinMapOnLoad;

            Workspace** activeWorkspace;

            int pinIdCounter = 1;
            int nodeIDCounter = 1;
            std::unordered_map<int, Core::Pin*> globalPinMap;

            inline static ImGuiID dockID;
            inline static int editorMoveSpeed = 5;

        private:
            std::vector<Core::LogicalElements::Base*> elements;
            std::vector<std::pair<int, int>> connections;

            std::unordered_map<std::string, std::function<void()>> addElementFuncs;

            bool deleteKeyPressed = 0;

            bool recursionWarning = false;

            bool firstRender = false;

            std::string name = "";

            ImNodesEditorContext* imnodesCtx;


    };

}

#endif //LOGIGATES_WORKSPACE_H
