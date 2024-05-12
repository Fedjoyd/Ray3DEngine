#include "Components/ComponentsManager.h"

#include "Core/Application.h"

Components::ComponentsManager::ComponentsManager()
{
#ifdef _EDITOR
    m_ComponentCreatorsName.insert(m_ComponentCreatorsName.begin(), "Add component ...");
    m_ComponentCreatorsHash.insert(m_ComponentCreatorsHash.begin(), 0u);
#endif // _EDITOR
}

bool Components::ComponentsManager::AddComponentCreator(const std::type_info& p_typeData, std::function<IComponent* (void)> p_creator)
{
    if (Core::Application::GetComponentsManager().m_ComponentCreators.find(p_typeData.hash_code()) != Core::Application::GetComponentsManager().m_ComponentCreators.end())
        return false;

    Core::Application::GetComponentsManager().m_ComponentCreators.emplace(p_typeData.hash_code(), p_creator);
#ifdef _EDITOR
    Core::Application::GetComponentsManager().m_ComponentCreatorsName.push_back(p_typeData.name());
    Core::Application::GetComponentsManager().m_ComponentCreatorsHash.push_back(p_typeData.hash_code());
#endif // _EDITOR

    return true;
}

Components::IComponent* Components::ComponentsManager::CreateComponent(size_t p_typeHash)
{
    if (m_ComponentCreators.find(p_typeHash) == m_ComponentCreators.end())
        return nullptr;

    return m_ComponentCreators[p_typeHash]();
}


#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"

void Components::ComponentsManager::ShowEditorControl()
{
    for (size_t indexComponentType = 1u; indexComponentType < m_ComponentCreatorsName.size(); indexComponentType++)
        ImGui::Text("%s (hash : %zu)", m_ComponentCreatorsName[indexComponentType], m_ComponentCreatorsHash[indexComponentType]);
}

bool Components::ComponentsManager::ShowComponentCombotBox(size_t* p_componentHash)
{
    static size_t indexCombo = 0u;
    ComponentsManager& cmpManager = Core::Application::GetComponentsManager();

    if (ImGui::Combo("##comboBoxComponent", (int*)&indexCombo, cmpManager.m_ComponentCreatorsName.data(), (int)cmpManager.m_ComponentCreatorsName.size()))
    {
        if (indexCombo != 0 && p_componentHash != nullptr)
        {
            (*p_componentHash) = cmpManager.m_ComponentCreatorsHash[indexCombo];
            indexCombo = 0u;
            return true;
        }
    }

    return false;
}
#endif // _EDITOR