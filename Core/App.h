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

#ifndef LOGIGATES_APP_H
#define LOGIGATES_APP_H

#include <iostream>
#include "../thirdparty/imgui/imgui_impl_sdl2.h"

#include "../UI/UI.h"
#include "LogicalElements/And.h"

#include "../UI/Localization.h"

namespace LogiGates::Core {

    class App {
        public:
            App(int argc, char* argv[]);
            ~App();

            void run();

        private:
            UI::Window* window;
            UI::Renderer* renderer;

            UI::Dockspace* dockspace;
            UI::ElementsMenu* elementsMenu;
    };


}

#endif //LOGIGATES_APP_H
