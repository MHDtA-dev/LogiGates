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

#ifndef LOGIGATES_PREFERENCESWINDOW_H
#define LOGIGATES_PREFERENCESWINDOW_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <functional>

#include "Drawable.h"
#include "../Renderer.h"

#include "../../Core/LogicalElements/Base.h"
#include "../../Core/Pin.h"

#include "../../thirdparty/imnodes/imnodes.h"
#include "../../thirdparty/lunada/lunada.h"

namespace LogiGates::UI {

    struct Preferences {
        char language[40];
        int editorMoveSpeed = 5;
    };

    class PreferencesWindow : public Drawable {
        public:
            PreferencesWindow();
            void render() override;

            bool visible = false;

            Preferences preferences;

            std::unordered_map<std::string, std::string> langNamesFullToShort;

    };

}

#endif //LOGIGATES_PREFERENCESWINDOW_H
