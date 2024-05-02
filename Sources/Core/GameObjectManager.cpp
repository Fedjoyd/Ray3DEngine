#include "Core/GameObjectManager.h"

void Core::GameObjectManager::LoadScene(ScenePtr m_SceneToLoad)
{
	m_gameObjectsList.clear();

	if (m_SceneToLoad == nullptr)
		return;

	tinyxml2::XMLDocument CurrentSceneXMLDocument;

	if (!m_SceneToLoad->LoadXmlDocument(CurrentSceneXMLDocument))
		return;

	tinyxml2::XMLElement* ListXMLGameObject = CurrentSceneXMLDocument.FirstChildElement("GameObjects");
	tinyxml2::XMLElement* CurrentXMLGameObject = nullptr;

	if (ListXMLGameObject != nullptr)
		CurrentXMLGameObject = ListXMLGameObject->FirstChildElement("GameObject");

	while (CurrentXMLGameObject != nullptr)
	{
		m_gameObjectsList.emplace_back(new Core::GameObject(CurrentXMLGameObject->Attribute("Name")));

		m_gameObjectsList.back()->DeserializeGameObject(CurrentXMLGameObject);

		CurrentXMLGameObject = CurrentXMLGameObject->NextSiblingElement("GameObject");
	}
}

bool Core::GameObjectManager::SaveScene(ScenePtr& m_SceneToSave)
{
	if (m_SceneToSave == nullptr)
		return false;

	tinyxml2::XMLDocument CurrentSceneXMLDocument;
	CurrentSceneXMLDocument.Parse("<?xml version=\"1.0\"?><GameObjects></GameObjects>");

	tinyxml2::XMLElement* ListXMLGameObject = CurrentSceneXMLDocument.FirstChildElement("GameObjects");
	tinyxml2::XMLElement* CurrentXMLGameObject = nullptr;

	if (ListXMLGameObject != nullptr)
		for (GameObjectPtr& currentGameObjet : m_gameObjectsList)
		{
			CurrentXMLGameObject = ListXMLGameObject->InsertNewChildElement("GameObject");

			if (CurrentXMLGameObject != nullptr)
				currentGameObjet->SerializeGameObject(CurrentXMLGameObject);
		}
	else
		return false;

	m_SceneToSave->Save(CurrentSceneXMLDocument);
	return true;
}

void Core::GameObjectManager::Start()
{
    for (GameObjectPtr& CurrentGameObject : m_gameObjectsList)
        CurrentGameObject->Start();
}

void Core::GameObjectManager::Update()
{
	m_uiRenderComponents.clear();

	for (unsigned int i = 0; i < m_gameObjectsList.size(); i++)
	{
		m_gameObjectsList[i]->Update();

		if (m_gameObjectsList[i]->HasDeleteFlag())
		{
			m_gameObjectsList.erase(m_gameObjectsList.begin() + i);
			i--;
		}

		m_gameObjectsList[i]->RegisterUiComponent(m_uiRenderComponents);
	}
}

void Core::GameObjectManager::FixedUpdate()
{
	for (GameObjectPtr& CurrentGameObject : m_gameObjectsList)
		CurrentGameObject->FixedUpdate();
}

#ifdef _EDITOR
void Core::GameObjectManager::EditorUpdate()
{
	for (unsigned int i = 0; i < m_gameObjectsList.size(); i++)
	{
		m_gameObjectsList[i]->EditorUpdate();

		if (m_gameObjectsList[i]->HasDeleteFlag())
		{
			m_gameObjectsList.erase(m_gameObjectsList.begin() + i);
			i--;
		}
	}
}
void Core::GameObjectManager::EditorFixedUpdate()
{
	for (GameObjectPtr& CurrentGameObject : m_gameObjectsList)
		CurrentGameObject->EditorFixedUpdate();
}

void Core::GameObjectManager::ShowManagerWindow(bool* p_opened)
{
	// TODO : Manager window
}

void Core::GameObjectManager::ShowEditorWindow(bool* p_opened)
{
	// TODO : Editor window
}
#endif // _EDITOR

void Core::GameObjectManager::Draw() const
{
	for (const GameObjectPtr& CurrentGameObject : m_gameObjectsList)
		CurrentGameObject->Draw();
}

void Core::GameObjectManager::DrawUI() const
{
	for (const std::pair<float, std::pair<const GameObject*, Components::IUIRenderComponent*>>& currentuiComponent : m_uiRenderComponents)
		currentuiComponent.second.second->DrawUi(currentuiComponent.second.first);
}