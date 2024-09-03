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

#ifndef LOGIGATES_DECTOBASE_H
#define LOGIGATES_DECTOBASE_H

#include <iostream>

std::string decToBase(int a, int base);
std::string decToBaseReversed(int a, int base);

#ifdef DECTOBASE_IMPLEMENTATION

std::string decToBase(int a, int base) {
    char nums[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    std::string buf;

    int digit = a;

    while (digit != 0) {
        buf = nums[digit % base] + buf;
        digit /= base;
    }

    return buf;

}

std::string decToBaseReversed(int a, int base) {
    char nums[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    std::string buf;

    int digit = a;

    while (digit != 0) {
        buf += nums[digit % base];
        digit /= base;
    }

    return buf;

}

#endif

#endif //LOGIGATES_DECTOBASE_H
