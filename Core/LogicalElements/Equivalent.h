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

#ifndef LOGIGATES_EQUIVALENT_H
#define LOGIGATES_EQUIVALENT_H

#include <iostream>
#include "Base.h"
#include "../../UI/Images.h"

namespace LogiGates::Core::LogicalElements {

    class Equivalent : public Base {
        public:
            Equivalent(UI::Workspace* workspace);

            void render() override;
            void perform(std::set<int> performedIDs = {}) override;
    };

}


#endif //LOGIGATES_EQUIVALENT_H
