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

#ifndef LOGIGATES_IMAGES_H
#define LOGIGATES_IMAGES_H

#include <iostream>
#include <unordered_map>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imnodes/imnodes.h"

#include "Renderer.h"

namespace LogiGates::UI {

    class Images {
        public:
            static void load(Renderer* renderer);

            inline static std::unordered_map<std::string, Texture*> icons;
    };

}


#endif //LOGIGATES_IMAGES_H
