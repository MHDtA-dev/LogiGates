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

#define WEBGPU_CPP_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "Renderer.h"

namespace LogiGates::UI {


    Renderer::Renderer(Window* window) : window(window) {
        createInstance();
        createSurface();
        requestAdapter();
        requestDevice();
        requestQueue();
        reconfigureSurface(window->getWidth(), window->getHeight());
        initImGui();

        windowWidth = window->getWidth();
        windowHeight = window->getHeight();
    }

    Renderer::~Renderer() {
        ImGui_ImplWGPU_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImNodes::DestroyContext();
        ImGui::DestroyContext();
        queue.release();
        device.release();
        adapter.release();
        surface.release();
        instance.release();
    }

    void Renderer::createInstance() {
        wgpu::InstanceDescriptor desc = {};
        desc.nextInChain = nullptr;

        instance = wgpu::createInstance(desc);

        if (!instance) {
            throw std::runtime_error("Can not create rendering instance");
        }

    }

    void Renderer::createSurface() {
        surface = SDL_GetWGPUSurface(instance, window->getNativeHandle());
    }

    void Renderer::requestAdapter() {
        wgpu::RequestAdapterOptions opts = {};
        opts.compatibleSurface = surface;
        opts.nextInChain = nullptr;

        adapter = instance.requestAdapter(opts);

        if (!adapter) {
            throw std::runtime_error("Can not find suitable GPU");
        }
    }


    void Renderer::requestDevice() {
        wgpu::DeviceDescriptor desc = {};
        device = adapter.requestDevice(desc);

        if (!device) {
            throw std::runtime_error("Can not create device");
        }
    }

    void Renderer::requestQueue() {
        queue = device.getQueue();

        if (!queue) {
            throw std::runtime_error("Can not get command queue");
        }
    }


    void Renderer::reconfigureSurface(int width, int height) {
        wgpu::SurfaceConfiguration config = {};
        config.nextInChain = nullptr;
        config.width = width;
        config.height = height;
        config.format = surface.getPreferredFormat(adapter);
        config.viewFormatCount = 0;
        config.viewFormats = nullptr;
        config.usage = wgpu::TextureUsage::RenderAttachment;
        config.device = device;
        config.presentMode = wgpu::PresentMode::Fifo;
        config.alphaMode = wgpu::CompositeAlphaMode::Auto;

        surface.configure(config);
    }

    bool Renderer::begin() {

        windowWidth = window->getWidth();
        windowHeight = window->getHeight();

        wgpu::SurfaceTexture surfaceTexture;
        surface.getCurrentTexture(&surfaceTexture);
        targetTexture = surfaceTexture.texture;
        targetView = targetTexture.createView();

        if (resizedX != -1 and resizedY != -1) {
            targetTexture.release();
            reconfigureSurface(window->getWidth(), window->getHeight());

            resizedX = -1;
            resizedY = -1;
            return false;
        }


        if (surfaceTexture.status == wgpu::SurfaceGetCurrentTextureStatus::Outdated) {
            targetTexture.release();

            reconfigureSurface(window->getWidth(), window->getHeight());

            windowWidth = window->getWidth();
            windowHeight = window->getHeight();

        }

        commandEncoder = device.createCommandEncoder();



        wgpu::RenderPassDescriptor renderPassDesc = {};
        renderPassDesc.nextInChain = nullptr;

        wgpu::RenderPassColorAttachment colorAtt = {};
        colorAtt.view = targetView;
        colorAtt.resolveTarget = nullptr;
        colorAtt.loadOp = wgpu::LoadOp::Clear;
        colorAtt.storeOp = wgpu::StoreOp::Store;
        colorAtt.clearValue = wgpu::Color{0, 0, 0, 1};


        wgpu::TextureDescriptor depthTextureDesc = {};
        depthTextureDesc.dimension = wgpu::TextureDimension::_2D;
        depthTextureDesc.format = wgpu::TextureFormat::Depth24Plus;
        depthTextureDesc.mipLevelCount = 1;
        depthTextureDesc.sampleCount = 1;
        depthTextureDesc.size = {(uint32_t) windowWidth, (uint32_t) windowHeight, 1};
        depthTextureDesc.usage = wgpu::TextureUsage::RenderAttachment;
        depthTextureDesc.viewFormatCount = 1;
        depthTextureDesc.viewFormats = (WGPUTextureFormat*) &depthFormat;

        depthTexture = device.createTexture(depthTextureDesc);

        wgpu::TextureViewDescriptor depthViewDescriptor = {};
        depthViewDescriptor.aspect = wgpu::TextureAspect::DepthOnly;
        depthViewDescriptor.baseArrayLayer = 0;
        depthViewDescriptor.arrayLayerCount = 1;
        depthViewDescriptor.baseMipLevel = 0;
        depthViewDescriptor.mipLevelCount = 1;
        depthViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
        depthViewDescriptor.format = depthFormat;
        depthTextureView = depthTexture.createView(depthViewDescriptor);

        wgpu::RenderPassDepthStencilAttachment depthAttachment = {};
        depthAttachment.depthClearValue = 1.0f;
        depthAttachment.depthLoadOp = wgpu::LoadOp::Clear;
        depthAttachment.depthStoreOp = wgpu::StoreOp::Store;
        depthAttachment.depthReadOnly = false;
        depthAttachment.stencilClearValue = 0;
        depthAttachment.stencilLoadOp = wgpu::LoadOp::Clear;
        depthAttachment.stencilStoreOp = wgpu::StoreOp::Store;
        depthAttachment.stencilReadOnly = true;
        depthAttachment.view = depthTextureView;

        renderPassDesc.colorAttachmentCount = 1;
        renderPassDesc.colorAttachments = &colorAtt;
        renderPassDesc.depthStencilAttachment = nullptr;
        renderPassDesc.timestampWrites = nullptr;
        renderPassDesc.depthStencilAttachment = &depthAttachment;

        renderPass = commandEncoder.beginRenderPass(renderPassDesc);

        ImGui_ImplWGPU_NewFrame();
        ImGui_ImplWGPU_PushRealWindowSize(windowWidth, windowHeight);
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        return true;
    }

    void Renderer::end() {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), renderPass);

        renderPass.setViewport(0, 0, windowWidth, windowHeight, 0, 1);

        renderPass.end();
        renderPass.release();

        wgpu::CommandBuffer commandBuffer = commandEncoder.finish();
        queue.submit(commandBuffer);
        commandBuffer.release();
        commandEncoder.release();

        targetView.release();
        depthTextureView.release();

        surface.present();
        depthTexture.release();
        targetTexture.release();

    }

    void Renderer::initImGui() {
        ImGui::CreateContext();
        ImNodes::CreateContext();
        ImGui_ImplSDL2_InitForOther(window->getNativeHandle());
        ImGui_ImplWGPU_InitInfo info;
        info.Device = device;
        info.NumFramesInFlight = 3;
        info.RenderTargetFormat = surface.getPreferredFormat(adapter);
        info.DepthStencilFormat = wgpu::TextureFormat::Depth24Plus;

        ImGui_ImplWGPU_Init(&info);


        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImFontConfig fontConf;
        fontConf.OversampleH = 1;
        fontConf.OversampleV = 1;
        fontConf.PixelSnapH = 1;

        io.Fonts->AddFontDefault();
        Fonts::openSans16 = io.Fonts->AddFontFromFileTTF("fonts/OpenSans.ttf", 16, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        Fonts::openSans18 = io.Fonts->AddFontFromFileTTF("fonts/OpenSans.ttf", 18, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        Fonts::openSans20 = io.Fonts->AddFontFromFileTTF("fonts/OpenSans.ttf", 20, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        Fonts::openSans24 = io.Fonts->AddFontFromFileTTF("fonts/OpenSans.ttf", 24, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
        Fonts::openSans30 = io.Fonts->AddFontFromFileTTF("fonts/OpenSans.ttf", 30, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        Fonts::orbitron18 = io.Fonts->AddFontFromFileTTF("fonts/Orbitron.ttf", 18);
        Fonts::orbitron24 = io.Fonts->AddFontFromFileTTF("fonts/Orbitron.ttf", 24);
        Fonts::orbitron36 = io.Fonts->AddFontFromFileTTF("fonts/Orbitron.ttf", 36);


        // Theme setup
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha = 1.0f;
        style.DisabledAlpha = 1.0f;
        style.WindowPadding = ImVec2(12.0f, 12.0f);
        style.WindowRounding = 11.5f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize = ImVec2(20.0f, 20.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Right;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 0.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
        style.FrameRounding = 11.89999961853027f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
        style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
        style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
        style.IndentSpacing = 0.0f;
        style.ColumnsMinSpacing = 4.900000095367432f;
        style.ScrollbarSize = 11.60000038146973f;
        style.ScrollbarRounding = 15.89999961853027f;
        style.GrabMinSize = 3.700000047683716f;
        style.GrabRounding = 20.0f;
        style.TabRounding = 0.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7960784435272217f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8823529481887817f, 0.7960784435272217f, 0.5607843399047852f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
        style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
        style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);


    }

    Texture* Renderer::loadTexture(std::string path) {
        int width, height, channels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            throw std::runtime_error("Can not load texture " + path);
        }

        Texture* ret = new Texture();

        wgpu::TextureDescriptor textureDesc;
        textureDesc.dimension = wgpu::TextureDimension::_2D;
        textureDesc.format = wgpu::TextureFormat::RGBA8Unorm;
        textureDesc.mipLevelCount = 1;
        textureDesc.sampleCount = 1;
        textureDesc.size = { (unsigned int)width, (unsigned int)height, 1 };
        textureDesc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
        textureDesc.viewFormatCount = 0;
        textureDesc.viewFormats = nullptr;
        ret->texture = device.createTexture(textureDesc);

        writeMipMaps(ret->texture, textureDesc.size, textureDesc.mipLevelCount, data);

        wgpu::TextureViewDescriptor textureViewDesc;
        textureViewDesc.aspect = wgpu::TextureAspect::All;
        textureViewDesc.baseArrayLayer = 0;
        textureViewDesc.arrayLayerCount = 1;
        textureViewDesc.baseMipLevel = 0;
        textureViewDesc.mipLevelCount = textureDesc.mipLevelCount;
        textureViewDesc.dimension = wgpu::TextureViewDimension::_2D;
        textureViewDesc.format = textureDesc.format;
        ret->view = ret->texture.createView(textureViewDesc);

        return ret;
    }

    void Renderer::writeMipMaps(wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount,
                                const unsigned char* pixelData) {

        wgpu::ImageCopyTexture destination;
        destination.texture = texture;
        destination.origin = { 0, 0, 0 };
        destination.aspect = wgpu::TextureAspect::All;


        wgpu::TextureDataLayout source;
        source.offset = 0;


        wgpu::Extent3D mipLevelSize = textureSize;
        std::vector<unsigned char> previousLevelPixels;
        wgpu::Extent3D previousMipLevelSize;
        for (uint32_t level = 0; level < mipLevelCount; ++level) {
            std::vector<unsigned char> pixels(4 * mipLevelSize.width * mipLevelSize.height);
            if (level == 0) {
                memcpy(pixels.data(), pixelData, pixels.size());
            } else {
                for (uint32_t i = 0; i < mipLevelSize.width; i++) {
                    for (uint32_t j = 0; j < mipLevelSize.height; j++) {
                        unsigned char* p = &pixels[4 * (j * mipLevelSize.width + i)];

                        unsigned char* p00 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 0))];
                        unsigned char* p01 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 1))];
                        unsigned char* p10 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 0))];
                        unsigned char* p11 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 1))];

                        p[0] = (p00[0] + p01[0] + p10[0] + p11[0]) / 4;
                        p[1] = (p00[1] + p01[1] + p10[1] + p11[1]) / 4;
                        p[2] = (p00[2] + p01[2] + p10[2] + p11[2]) / 4;
                        p[3] = (p00[3] + p01[3] + p10[3] + p11[3]) / 4;
                    }
                }
            }


            destination.mipLevel = level;
            source.bytesPerRow = 4 * mipLevelSize.width;
            source.rowsPerImage = mipLevelSize.height;
            queue.writeTexture(destination, pixels.data(), pixels.size(), source, mipLevelSize);

            previousLevelPixels = std::move(pixels);
            previousMipLevelSize = mipLevelSize;
            mipLevelSize.width /= 2;
            mipLevelSize.height /= 2;
        }
    }

    void Renderer::windowResized(int width, int height) {
        resizedX = width;
        resizedY = height;
    }


}