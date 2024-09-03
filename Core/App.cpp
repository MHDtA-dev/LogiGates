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

#include "App.h"

namespace LogiGates::Core {

    App::App() {
        window = new UI::Window("LogiGates");
        renderer = new UI::Renderer(window);

        UI::Images::load(renderer);

        workspace = new UI::Workspace();
        dockspace = new UI::Dockspace(workspace);
        elementsMenu = new UI::ElementsMenu();

        UI::Localization::init();

    }

    void App::run() {
        while (!this->window->shouldClose()) {
            window->pollEvents([] (SDL_Event e) {
                ImGui_ImplSDL2_ProcessEvent(&e);
            });

            if (renderer->begin()) {

                dockspace->render();
                elementsMenu->render();
                workspace->render();

                renderer->end();
            }
        }
    }

    App::~App() {
        delete window;
        delete renderer;
        delete dockspace;
        delete elementsMenu;
    }

}