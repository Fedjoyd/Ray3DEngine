#include "Components/ComponentsManager.h"

bool Components::ComponentsManager::AddComponentCreator(size_t p_typeHash, std::function<IComponent* ()> p_creator)
{
    if (m_ComponentCreators.find(p_typeHash) == m_ComponentCreators.end())
        return false;

    m_ComponentCreators.emplace(p_typeHash, p_creator);

    return true;
}

Components::IComponent* Components::ComponentsManager::CreateComponent(size_t p_typeHash)
{
    if (m_ComponentCreators.find(p_typeHash) == m_ComponentCreators.end())
        return nullptr;

    return m_ComponentCreators[p_typeHash]();
}
