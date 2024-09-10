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

#ifndef LOGIGATES_RENDERER_H
#define LOGIGATES_RENDERER_H

#include <iostream>
#include <webgpu/webgpu.hpp>
#include <sdl2webgpu.h>

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_sdl2.h"
#include "../thirdparty/imgui/imgui_impl_wgpu.h"
#include "../thirdparty/imnodes/imnodes.h"
#include "../thirdparty/stb_image.h"

#include "Fonts.h"
#include "Window.h"

namespace LogiGates::UI {

    struct Texture {
        wgpu::Texture texture;
        wgpu::TextureView view;

        ~Texture() {
            texture.release();
            view.release();
        }
    };

    class Renderer {
    public:
        Renderer(Window* window);
        ~Renderer();

        void reconfigureSurface(int width, int height);
        void windowResized(int width, int height);
        bool begin();
        void end();

        Texture* loadTexture(std::string path);

    private:
        Window* window;

        wgpu::Instance instance;
        wgpu::Adapter adapter;
        wgpu::Device device;
        wgpu::Queue queue;
        wgpu::CommandEncoder commandEncoder;
        wgpu::Surface surface;

        wgpu::TextureFormat depthFormat = wgpu::TextureFormat::Depth24Plus;

        wgpu::Texture targetTexture;
        wgpu::TextureView targetView;
        wgpu::RenderPassEncoder renderPass;
        wgpu::Texture depthTexture;
        wgpu::TextureView depthTextureView;

        void createInstance();
        void requestAdapter();
        void requestDevice();
        void requestQueue();
        void createSurface();
        void initImGui();

        int windowWidth, windowHeight;
        int resizedX = -1, resizedY = -1;


        void writeMipMaps(wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount, const unsigned char* pixelData);

    };

}


#endif //LOGIGATES_RENDERER_H