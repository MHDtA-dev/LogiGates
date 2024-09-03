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

#ifndef LOGIGATES_FONTS_H
#define LOGIGATES_FONTS_H

#include "../thirdparty/imgui/imgui.h"

namespace LogiGates::UI {

    class Fonts {
        public:
            inline static ImFont* openSans16;
            inline static ImFont* openSans18;
            inline static ImFont* openSans20;
            inline static ImFont* openSans24;
            inline static ImFont* openSans30;

            inline static ImFont* orbitron18;
            inline static ImFont* orbitron24;
            inline static ImFont* orbitron36;
    };

}

#endif //LOGIGATES_FONTS_H
