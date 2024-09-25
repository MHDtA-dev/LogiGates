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

#include "Workspace.h"

namespace LogiGates::UI {

    Workspace::Workspace(std::string name) : name(name) {
        addElementFuncs["and"] = [=]() {
            addElement(new Core::LogicalElements::And(this));
        };

        addElementFuncs["or"] = [=]() {
            addElement(new Core::LogicalElements::Or(this));
        };

        addElementFuncs["not"] = [=]() {
            addElement(new Core::LogicalElements::Not(this));
        };

        addElementFuncs["xor"] = [=]() {
            addElement(new Core::LogicalElements::Xor(this));
        };

        addElementFuncs["lamp"] = [=]() {
            addElement(new Core::LogicalElements::Lamp(this));
        };

        addElementFuncs["switch"] = [=]() {
            addElement(new Core::LogicalElements::Switch(this));
        };

        addElementFuncs["implication"] = [=]() {
            addElement(new Core::LogicalElements::Implication(this));
        };

        addElementFuncs["splitter"] = [=]() {
            addElement(new Core::LogicalElements::Splitter(this));
        };

        addElementFuncs["equivalent"] = [=]() {
            addElement(new Core::LogicalElements::Equivalent(this));
        };

        addElementFuncs["fivebitnumenc"] = [=]() {
            addElement(new Core::LogicalElements::FiveBitNumberEncoder(this));
        };

        addElementFuncs["fivebitnumdisp"] = [=]() {
            addElement(new Core::LogicalElements::FiveBitNumberDisplay(this));
        };

        addElementFuncs["multiplexer"] = [=]() {
            addElement(new Core::LogicalElements::Multiplexer(this));
        };

        imnodesCtx = ImNodes::EditorContextCreate();

    }

    void LogiGates::UI::Workspace::render() {
        if (this->recursionWarning) {
            ImGui::PushFont(Fonts::openSans20);
            ImGui::Begin(Localization::localization[Localization::currentLocalization]["recursionWarnTitle"].c_str(), &recursionWarning, ImGuiWindowFlags_NoDocking);
            ImGui::PopFont();

            ImGui::PushFont(Fonts::openSans20);
            ImGui::Text("%s", Localization::localization[Localization::currentLocalization]["recursionWarn"].c_str());
            ImGui::PopFont();

            ImGui::End();
        }

        if (showWindow) {

            ImGui::PushFont(Fonts::openSans20);
            bool beginResult = ImGui::Begin(this->name.c_str(), &showWindow);
            ImGui::PopFont();


            ImNodes::EditorContextSet(imnodesCtx);
            ImNodes::BeginNodeEditor();

            bool editorFocused = ImNodes::IsEditorHovered() and ImGui::IsMouseClicked(ImGuiMouseButton_Left);

            for (Core::LogicalElements::Base *element: elements) {
                element->render();
            }

            for (int i = 0; i < connections.size(); i++) {
                ImNodes::Link(i, connections[i].first, connections[i].second);
            }

            ImGui::PushFont(Fonts::openSans24);
            if (ImGui::BeginPopupContextWindow()) {

                if (ImGui::MenuItem(
                        Localization::localization[Localization::currentLocalization]["delete"].c_str())) {
                    deleteAction = true;
                }

                ImGui::EndPopup();
            }

            ImGui::PopFont();

            if (*activeWorkspace == this) {
                ImVec2 panning = ImNodes::EditorContextGetPanning();

                if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
                    panning.x += (float) editorMoveSpeed;
                }

                if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
                    panning.x -= (float) editorMoveSpeed;
                }

                if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) {
                    panning.y += (float) editorMoveSpeed;
                }

                if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) {
                    panning.y -= (float) editorMoveSpeed;
                }

                ImNodes::EditorContextResetPanning(panning);

                if ((ImGui::IsKeyPressed(ImGuiKey_Backspace) or ImGui::IsKeyPressed(ImGuiKey_Delete)) and !deleteKeyPressed) {
                    deleteAction = true;
                    deleteKeyPressed = true;
                }

                if ((ImGui::IsKeyReleased(ImGuiKey_Backspace) or ImGui::IsKeyReleased(ImGuiKey_Delete)) and deleteKeyPressed) {
                    deleteKeyPressed = false;
                }

            }

            ImNodes::EndNodeEditor();

            contextMenuDelete();

            if (ImGui::BeginDragDropTarget()) {
                auto payload = ImGui::AcceptDragDropPayload("ELEMENT");
                if (payload != nullptr) {
                    editorFocused = true;
                    addElementFuncs[std::string((const char *) payload->Data)]();
                    ImNodes::SetNodeScreenSpacePos(elements[elements.size() - 1]->getID(), ImGui::GetMousePos());
                    elements[elements.size() - 1]->perform();
                }

                ImGui::EndDragDropTarget();
            }

            if (beginResult and (editorFocused or ImGui::IsWindowFocused())) {
                *this->activeWorkspace = this;
            }


            int startID, endID;
            if (ImNodes::IsLinkCreated(&startID, &endID)) {
                connect(startID, endID);
            }

            int linkDestroyed;
            if (ImNodes::IsLinkDestroyed(&linkDestroyed)) {
                std::cout << linkDestroyed << std::endl;
            }


            ImGui::End();
        }

        if (!firstRender) {
            ImGui::DockBuilderDockWindow(this->name.c_str(), dockID);

            firstRender = true;
        }
    }

    void LogiGates::UI::Workspace::addElement(Core::LogicalElements::Base* element) {
        elements.push_back(element);
    }

    LogiGates::UI::Workspace::~Workspace() {
        ImNodes::EditorContextFree(this->imnodesCtx);
        for (Core::LogicalElements::Base* element: elements) {
            delete element;
        }
    }

    void LogiGates::UI::Workspace::connect(int start, int end) {
        if (globalPinMap[start]->getConnectedWith() != -1) {
            disconnect(start);
        }

        if (globalPinMap[end]->getConnectedWith() != -1) {
            disconnect(end);
        }

        globalPinMap[start]->connect(end);
        connections.push_back({start, end});
    }

    void LogiGates::UI::Workspace::disconnect(int pin) {
        for (auto i = connections.begin(); i != connections.end(); i++) {
            if (i->first == pin or i->second == pin) {
                globalPinMap[pin]->disconnect();
                connections.erase(i);
                break;
            }
        }
    }

    void LogiGates::UI::Workspace::removeConnection(int start, int end) {
        for (auto i = connections.begin(); i != connections.end(); i++) {
            if (i->first == start and i->second == end) {
                connections.erase(i);
                break;
            }
        }
    }

    void LogiGates::UI::Workspace::contextMenuDelete() {
        if (deleteAction) {
            int selectedConnectionsCount = ImNodes::NumSelectedLinks();
            int selectedNodesCount = ImNodes::NumSelectedNodes();

            if (selectedNodesCount != 0) {
                int *selectedNodes = (int*) malloc(selectedNodesCount * sizeof(int));
                ImNodes::GetSelectedNodes(selectedNodes);

                for (int i = 0; i < selectedNodesCount; i++) {
                    for (auto j = elements.begin(); j != elements.end(); j++) {
                        if ((*j)->getID() == selectedNodes[i]) {
                            delete (*j);
                            elements.erase(j);
                            break;
                        }
                    }
                }

                free(selectedNodes);

            } else if (selectedConnectionsCount != 0) {
                int *selectedConnections = (int*) malloc(selectedConnectionsCount * sizeof(int));
                ImNodes::GetSelectedLinks(selectedConnections);

                std::vector<int> linksToDisconnect;

                for (int i = 0; i < selectedConnectionsCount; i++) {
                    linksToDisconnect.push_back(connections[selectedConnections[i]].first);
                }

                for (int pin : linksToDisconnect) {
                    disconnect(pin);
                }


                free(selectedConnections);
            }

            deleteAction = false;
        }
    }

    void LogiGates::UI::Workspace::save(std::string filename) {
        std::vector<Core::LogicalElements::SaveInfo> saves;

        for (Core::LogicalElements::Base* element: elements) {
            saves.push_back(element->getSaveInfo());
        }

        Lunada::SerializeVector<Core::LogicalElements::SaveInfo>(saves, filename);
    }

    void LogiGates::UI::Workspace::load(std::string filename) {
        this->clear();

        std::vector<Core::LogicalElements::SaveInfo> saves;

        Lunada::DeserializeVector<Core::LogicalElements::SaveInfo>(saves, filename);

        for (Core::LogicalElements::SaveInfo saveInfo: saves) {
            addElementFuncs[std::string(saveInfo.elementType)]();
            elements[elements.size() - 1]->restoreFromSaveInfo(saveInfo, this->imnodesCtx);
        }

        globalPinMap = globalPinMapOnLoad;

        for (auto connection: connectionQueue) {
            connect(connection.first, connection.second);
        }

        connectionQueue.clear();
        globalPinMapOnLoad.clear();

    }

    void Workspace::clear() {
        for (auto& connection : connections) {
            disconnect(connection.first);
        }

        for (Core::LogicalElements::Base* element : elements) {
            delete element;
        }

        elements.clear();

    }

    void Workspace::enableRecursionWarning() {
        this->recursionWarning = !deleteAction;
    }

    std::string Workspace::getName() {
        return this->name;
    }

    void Workspace::setName(std::string name) {
        this->name = name;
    }


}
