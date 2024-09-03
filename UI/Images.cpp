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

#include "Images.h"

namespace LogiGates::UI {

    void Images::load(LogiGates::UI::Renderer *renderer) {
        Images::icons["and"] = renderer->loadTexture("icons/and.png");
        Images::icons["or"] = renderer->loadTexture("icons/or.png");
        Images::icons["xor"] = renderer->loadTexture("icons/xor.png");
        Images::icons["not"] = renderer->loadTexture("icons/not.png");
        Images::icons["lamp_off"] = renderer->loadTexture("icons/lamp_off.png");
        Images::icons["lamp_on"] = renderer->loadTexture("icons/lamp_on.png");
        Images::icons["switch_off"] = renderer->loadTexture("icons/switch_off.png");
        Images::icons["switch_on"] = renderer->loadTexture("icons/switch_on.png");
        Images::icons["implication"] = renderer->loadTexture("icons/implication.png");
        Images::icons["equivalent"] = renderer->loadTexture("icons/equivalent.png");
        Images::icons["splitter"] = renderer->loadTexture("icons/splitter.png");
        Images::icons["logo"] = renderer->loadTexture("icons/logo.png");
        Images::icons["fivebitnumenc"] = renderer->loadTexture("icons/fivebitnumenc.png");
        Images::icons["fivebitnumdisp"] = renderer->loadTexture("icons/fivebitnumdisp.png");
    }

}
