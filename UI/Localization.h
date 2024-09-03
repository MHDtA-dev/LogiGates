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

#ifndef LOGIGATES_LOCALIZATION_H
#define LOGIGATES_LOCALIZATION_H

#include <iostream>
#include <vector>
#include <unordered_map>

namespace LogiGates::UI {

    class Localization {
        public:
            static void init();

            inline static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> localization;
            inline static std::string currentLocalization = "en";
    };

}


#endif //LOGIGATES_LOCALIZATION_H
