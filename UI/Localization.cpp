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

#include "Localization.h"

namespace LogiGates::UI {

    void Localization::init() {
        localization["rus"] = {
                {"elements",       "Элементы"},
                {"workspace",      "Рабочее пространство"},
                {"and",            "И"},
                {"or",             "ИЛИ"},
                {"xor",            "ИСКЛ. ИЛИ"},
                {"not",            "НЕ"},
                {"implication",    "ИМПЛИКАЦИЯ"},
                {"equivalent",     "ЭКВИВАЛЕНТНОСТЬ"},
                {"switch",         "ВЫКЛЮЧАТЕЛЬ"},
                {"lamp",           "ЛАМПОЧКА"},
                {"splitter",       "РАЗДЕЛИТЕЛЬ"},
                {"fivebitnumenc",  "5-ти битный\nкодировщик числа"},
                {"fivebitnumdisp", "5-ти битный\nдисплей числа"},
                {"invalidnumber",  "Число должно быть\nв интервале [0; 31]"},
                {"delete",         "Удалить"},
                {"file",           "Файл"},
                {"save",           "Сохранить"},
                {"load",           "Загрузить"},
                {"preferences",    "Настройки"},
                {"in",             "Вход"},
                {"out",            "Выход"},
                {"b",              "Б"},
                {"theme",          "Тема"},
                {"language",       "Язык"},
                {"about",          "О программе"}
        };

        localization["en"] = {
                {"elements",       "Elements"},
                {"workspace",      "Workspace"},
                {"and",            "AND"},
                {"or",             "OR"},
                {"xor",            "XOR"},
                {"not",            "NOT"},
                {"implication",    "IMPLICATION"},
                {"equivalent",     "EQUIVALENT"},
                {"switch",         "SWITCH"},
                {"lamp",           "LAMP"},
                {"splitter",       "SPLITTER"},
                {"fivebitnumenc",  "5-Bit Number Encoder"},
                {"fivebitnumdisp", "5-Bit Number Display"},
                {"invalidnumber",  "The number should\nbe in [0; 31]"},
                {"delete",         "Delete"},
                {"file",           "File"},
                {"save",           "Save"},
                {"load",           "Load"},
                {"preferences",    "Preferences"},
                {"in",             "In"},
                {"out",            "Out"},
                {"b",              "B"},
                {"theme",          "Theme"},
                {"language",       "Language"},
                {"about",          "About"}
        };
    }

}