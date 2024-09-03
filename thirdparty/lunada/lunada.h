/*
(c) Copyright 2023 MHDtA

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LUNADA_LUNADA_H
#define LUNADA_LUNADA_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace fs = std::filesystem;

namespace Lunada {

    template<typename T>
    void SerializeStruct(const T& data, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File write error (" << filename << ")" << std::endl;
            return;
        }

        file.write(reinterpret_cast<const char*>(&data), sizeof(T));
        file.close();
    }

    template<typename T>
    void DeserializeStruct(T& data, const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File read error (" << filename << ")" << std::endl;
            return;
        }

        file.read(reinterpret_cast<char*>(&data), sizeof(T));
        file.close();
    }

    inline void* ReadRawDataFromFile(const std::string& filePath) {
        std::ifstream fileStream(filePath, std::ios::binary);
        if (!fileStream) {
            std::cerr << "[Lunada] File read error (" << filePath << ")" << std::endl;
            return nullptr;
        }

        fileStream.seekg(0, std::ios::end);
        std::streampos fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        void* data = new char[fileSize];
        fileStream.read(static_cast<char*>(data), fileSize);
        fileStream.close();

        return data;
    }


    inline void SerializeStringMap(const std::unordered_map<std::string, std::string>& data, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File write error (" << filename << ")" << std::endl;
            return;
        }

        size_t mapSize = data.size();
        file.write(reinterpret_cast<const char*>(&mapSize), sizeof(size_t));

        for (const auto& pair : data) {
            size_t keyLength = pair.first.size();
            size_t valueLength = pair.second.size();

            file.write(reinterpret_cast<const char*>(&keyLength), sizeof(size_t));
            file.write(pair.first.c_str(), keyLength);

            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(size_t));
            file.write(pair.second.c_str(), valueLength);
        }

        file.close();
    }

    inline void DeserializeStringMap(std::unordered_map<std::string, std::string>& data, const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File read error (" << filename << ")" << std::endl;
            return;
        }

        size_t mapSize;
        file.read(reinterpret_cast<char*>(&mapSize), sizeof(size_t));

        for (size_t i = 0; i < mapSize; ++i) {
            size_t keyLength, valueLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(size_t));
            std::string key(keyLength, '\0');
            file.read(&key[0], keyLength);

            file.read(reinterpret_cast<char*>(&valueLength), sizeof(size_t));
            std::string value(valueLength, '\0');
            file.read(&value[0], valueLength);

            data[key] = value;
        }

        file.close();
    }


    template <typename T>
    void SerializeVector(const std::vector<T>& data, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File write error (" << filename << ")" << std::endl;
            return;
        }

        size_t vectorSize = data.size();
        file.write(reinterpret_cast<const char*>(&vectorSize), sizeof(size_t));

        for (const T& item : data) {
            file.write(reinterpret_cast<const char*>(&item), sizeof(T));
        }

        file.close();
    }

    template <typename T>
    void DeserializeVector(std::vector<T>& data, const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File read error (" << filename << ")" << std::endl;
            return;
        }

        size_t vectorSize;
        file.read(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));

        data.clear();
        data.reserve(vectorSize);

        for (size_t i = 0; i < vectorSize; ++i) {
            T item;
            file.read(reinterpret_cast<char*>(&item), sizeof(T));
            data.push_back(item);
        }

        file.close();
    }


    template <typename KeyType, typename ValueType>
    void SerializeUnorderedMap(const std::unordered_map<KeyType, ValueType>& data, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File write error (" << filename << ")" << std::endl;
            return;
        }


        size_t mapSize = data.size();
        file.write(reinterpret_cast<const char*>(&mapSize), sizeof(size_t));


        for (const auto& pair : data) {
            size_t keyLength = pair.first.size();
            size_t valueLength = sizeof(ValueType);

            file.write(reinterpret_cast<const char*>(&keyLength), sizeof(size_t));
            file.write(pair.first.c_str(), keyLength);

            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(size_t));
            file.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        file.close();
    }

    template <typename KeyType, typename ValueType>
    void DeserializeUnorderedMap(std::unordered_map<KeyType, ValueType>& data, const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "[Lunada] File read error (" << filename << ")" << std::endl;
            return;
        }


        size_t mapSize;
        file.read(reinterpret_cast<char*>(&mapSize), sizeof(size_t));


        for (size_t i = 0; i < mapSize; ++i) {
            size_t keyLength, valueLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(size_t));
            std::string key(keyLength, '\0');
            file.read(&key[0], keyLength);

            file.read(reinterpret_cast<char*>(&valueLength), sizeof(size_t));
            ValueType value;
            file.read(reinterpret_cast<char*>(&value), valueLength);

            data[key] = value;
        }

        file.close();
    }


}

#endif //LUNADA_LUNADA_H
