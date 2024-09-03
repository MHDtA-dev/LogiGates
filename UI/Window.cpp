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

#include "Window.h"

namespace LogiGates::UI {

    Window::Window(int width, int height, std::string name) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "Can not init SDL2: " << SDL_GetError() << std::endl;
        }

        handle = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                                  SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(handle, SDL_TRUE);
    }

    Window::Window(std::string name) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "Can not init SDL2: " << SDL_GetError() << std::endl;
        }

        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);

        handle = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h,
                                  SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(handle, SDL_TRUE);
    }

    int Window::getWidth() {
        int w, h;
        SDL_GetWindowSize(handle, &w, &h);

        return w;
    }

    int Window::getHeight() {
        int w, h;
        SDL_GetWindowSize(handle, &w, &h);

        return h;
    }

    void Window::close() {
        closeFlag = true;
    }

    SDL_Window* Window::getNativeHandle() {
        return handle;
    }

    void Window::pollEvents(std::function<void(SDL_Event)> onEvent) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            onEvent(event);

            switch (event.type) {
                case SDL_QUIT:
                    this->close();
                    break;

                default:
                    break;
            }
        }
    }

    bool Window::shouldClose() {
        return this->closeFlag;
    }

    Window::~Window() {
        SDL_DestroyWindow(handle);
    }

}