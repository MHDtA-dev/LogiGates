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

#ifndef LOGIGATES_DRAWABLE_H
#define LOGIGATES_DRAWABLE_H

#include <iostream>
#include <vector>
#include "../Fonts.h"
#include "../Localization.h"
#include "../Images.h"

#include "../../thirdparty/imgui/imgui.h"

namespace LogiGates::UI {

    class Drawable {
        public:
            virtual void render() {};

            void addChild(Drawable* d);

            ~Drawable();

        protected:
            void renderChildren();

        private:
            std::vector<Drawable*> children;
    };

}

#endif //LOGIGATES_DRAWABLE_H
