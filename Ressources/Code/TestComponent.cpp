#include "TestComponent.h"

#include "Core/Application.h"

#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"
#include "rlImGui.h"
#endif // _EDITOR

#ifdef _EDITOR
    void TestComponent::ShowEditorControl(const unsigned int p_indexComponent)
    {
        IComponent::ShowEditorControl(p_indexComponent);

        static int64_t TestComponentDnDID = 0L;

        ImGui::Text("TestRessource :");
        ImGui::SameLine();
        ImGui::InputText(("##TestRessourceOfTestComponent_" + std::to_string(p_indexComponent)).c_str(), &m_TestRessourceName, ImGuiInputTextFlags_ReadOnly);
        if (Ressources::RessourcesManager::RessourceDnDTarget(&TestComponentDnDID))
            if (Core::Application::GetRessourcesManager().TryGetAndLoadRessource(TestComponentDnDID, &m_TestRessource))
            {
                m_TestRessourceName = m_TestRessource->GetName() + " (" + std::to_string(m_TestRessource->GetUUID()) + ")";
                TestComponentDnDID = 0L;
            }
        ImGui::SameLine();
        if (ImGui::Button(("X##TestRessourceOfTestComponent_" + std::to_string(p_indexComponent)).c_str()))
        {
            m_TestRessource = nullptr;
            m_TestRessourceName = "Empty";
        }
    }
#endif // _EDITOR