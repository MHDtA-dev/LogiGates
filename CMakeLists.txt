cmake_minimum_required(VERSION 3.28)
project(LogiGates)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/imgui.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FetchContent_Declare(
        webgpu
        GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
        GIT_TAG        wgpu
)
FetchContent_MakeAvailable(webgpu)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
add_subdirectory(thirdparty/sdl2webgpu)

add_executable(LogiGates main.cpp
        UI/Window.cpp
        UI/Window.h
        Core/App.cpp
        Core/App.h
        UI/Renderer.cpp
        UI/Renderer.h
        UI/imgui/Drawable.cpp
        UI/imgui/Drawable.h
        UI/imgui/Dockspace.cpp
        UI/imgui/Dockspace.h
        UI/UI.h
        UI/imgui/ElementsMenu.cpp
        UI/imgui/ElementsMenu.h
        UI/Fonts.cpp
        UI/Fonts.h
        UI/imgui/Workspace.cpp
        UI/imgui/Workspace.h
        Core/LogicalElements/Base.cpp
        Core/LogicalElements/Base.h
        Core/Pin.cpp
        Core/Pin.h
        Core/LogicalElements/And.cpp
        Core/LogicalElements/And.h
        UI/Images.cpp
        UI/Images.h
        Core/LogicalElements/LogicalElements.h
        Core/LogicalElements/Or.cpp
        Core/LogicalElements/Or.h
        Core/LogicalElements/Not.cpp
        Core/LogicalElements/Not.h
        Core/LogicalElements/Xor.cpp
        Core/LogicalElements/Xor.h
        Core/LogicalElements/Lamp.cpp
        Core/LogicalElements/Lamp.h
        Core/LogicalElements/Switch.cpp
        Core/LogicalElements/Switch.h
        Core/LogicalElements/Implication.cpp
        Core/LogicalElements/Implication.h
        Core/LogicalElements/Splitter.cpp
        Core/LogicalElements/Splitter.h
        UI/imgui/PreferencesWindow.cpp
        UI/imgui/PreferencesWindow.h
        UI/Localization.cpp
        UI/Localization.h
        Core/LogicalElements/Equivalent.cpp
        Core/LogicalElements/Equivalent.h
        Core/LogicalElements/FiveBitNumberEncoder.cpp
        Core/LogicalElements/FiveBitNumberEncoder.h
        thirdparty/DecToBase.h
        UI/imgui/AboutWindow.cpp
        UI/imgui/AboutWindow.h
        Core/LogicalElements/FiveBitNumberDisplay.cpp
        Core/LogicalElements/FiveBitNumberDisplay.h
        UI/imgui/Welcome.cpp
        UI/imgui/Welcome.h
        Core/LogicalElements/Multiplexer.cpp
        Core/LogicalElements/Multiplexer.h)


target_sources(LogiGates PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp"


        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_wgpu.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_wgpu.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_sdl2.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes/imnodes.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes/imnodes_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes/imnodes.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyfiledialogs/tinyfiledialogs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyfiledialogs/tinyfiledialogs.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lunada/lunada.h"

)

target_link_libraries(LogiGates webgpu ${SDL2_LIBRARIES} sdl2webgpu)
target_copy_webgpu_binaries(LogiGates)
