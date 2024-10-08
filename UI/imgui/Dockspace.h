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

#ifdef __APPLE__
#define LOGIGATES_FILE_EXTENSION ".logigates"
#else
#define LOGIGATES_FILE_EXTENSION ""
#endif

#ifndef LOGIGATES_DOCKSPACE_H
#define LOGIGATES_DOCKSPACE_H

#include <iostream>
#include "Drawable.h"
#include "../Images.h"
#include "../Localization.h"

#include "../../thirdparty/tinyfiledialogs/tinyfiledialogs.h"

#include "PreferencesWindow.h"
#include "AboutWindow.h"
#include "Workspace.h"
#include "Welcome.h"

namespace LogiGates::UI {

    class Dockspace : public Drawable {
        public:
            Dockspace();

            void render() override;

            void setActiveWorkspace(Workspace* workspace);

        private:
            PreferencesWindow* preferencesWindow;
            AboutWindow* aboutWindow;
            Welcome* welcomeWindow;
            Workspace* activeWorkspace = nullptr;

            std::vector<Workspace*> workspaces;

            bool newWorkspaceWindow = false;
            std::string newWorkspaceName = "";

            bool isWorkspaceExists(std::string name);
    };

}

#endif //LOGIGATES_DOCKSPACE_H
