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

#ifndef LOGIGATES_ELEMENTSMENU_H
#define LOGIGATES_ELEMENTSMENU_H

#include <iostream>
#include <unordered_map>

#include "Drawable.h"
#include "../Renderer.h"
#include "../Images.h"
#include "../Localization.h"

namespace LogiGates::UI {

    struct Element {
        std::string name;
        Texture* texture;
    };

    class ElementsMenu : public Drawable {
        public:
            ElementsMenu();
            void render() override;

        private:
            std::vector<Element> uiElements;

    };

}

#endif //LOGIGATES_ELEMENTSMENU_H
