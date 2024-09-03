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

namespace LogiGates::UI {

    class Dockspace : public Drawable {
        public:
            Dockspace(Workspace* workspace);

            void render() override;

        private:
            PreferencesWindow* preferencesWindow;
            AboutWindow* aboutWindow;
            Workspace* workspace;
    };

}

#endif //LOGIGATES_DOCKSPACE_H
