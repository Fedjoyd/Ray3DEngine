#include "Ressources/RessourcesManager.h"

ScenePtr Ressources::RessourcesManager::GetScene(const std::string& m_sceneName)
{
    for (ScenePtr& CurrentTestScene : m_Scenes)
        if (CurrentTestScene->GetName() == m_sceneName)
            return CurrentTestScene;

    return nullptr;
}

bool Ressources::RessourcesManager::RegisterScene(ScenePtr p_newScene)
{
    if (p_newScene == nullptr)
        return false;

    for (ScenePtr& CurrentTestScene : m_Scenes)
        if (CurrentTestScene->GetName() == p_newScene->GetName())
            return false;

    m_Scenes.push_back(p_newScene);
    return true;
}

void Ressources::RessourcesManager::UnloadUnusedRessource()
{
}
