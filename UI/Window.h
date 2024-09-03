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

#ifndef LOGIGATES_WINDOW_H
#define LOGIGATES_WINDOW_H

#include <iostream>
#include <functional>
#include <SDL2/SDL.h>

namespace LogiGates::UI {

    class Window {
        public:
            Window(int width, int height, std::string name);
            Window(std::string name);
            ~Window();

            int getWidth();
            int getHeight();
            bool shouldClose();

            void close();
            void pollEvents(std::function<void(SDL_Event)> onEvent = [] (SDL_Event e) {});

            SDL_Window* getNativeHandle();

        private:
            bool closeFlag = false;
            SDL_Window* handle;
    };

}

#endif //LOGIGATES_WINDOW_H
