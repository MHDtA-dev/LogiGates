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

    App::App(int argc, char* argv[]) {
        std::cout << argv[0] << std::endl;

        window = new UI::Window("LogiGates");
        renderer = new UI::Renderer(window);

        UI::Images::load(renderer);

        dockspace = new UI::Dockspace();
        elementsMenu = new UI::ElementsMenu();

        UI::Localization::init();

    }

    void App::run() {
        while (!this->window->shouldClose()) {
            window->pollEvents([=] (SDL_Event e) {
                ImGui_ImplSDL2_ProcessEvent(&e);

                if (e.type == SDL_WINDOWEVENT) {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED or e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        renderer->windowResized(e.window.data1, e.window.data2);
                    }
                }
            });

            if (renderer->begin()) {

                dockspace->render();
                elementsMenu->render();

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