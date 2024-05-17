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

#ifdef _EDITOR
bool Core::GameObjectManager::SaveScene(ScenePtr& m_SceneToSave)
{
	if (m_SceneToSave == nullptr)
		return false;

	tinyxml2::XMLDocument CurrentSceneXMLDocument;
	CurrentSceneXMLDocument.Parse("<?xml version=\"1.0\"?><GameObjects></GameObjects>");

	tinyxml2::XMLElement* ListXMLGameObject = CurrentSceneXMLDocument.FirstChildElement("GameObjects");
	tinyxml2::XMLElement* CurrentXMLGameObject = nullptr;

	if (ListXMLGameObject != nullptr)
		for (GameObjectPtr& currentGameObject : m_gameObjectsList)
		{
			CurrentXMLGameObject = ListXMLGameObject->InsertNewChildElement("GameObject");

			if (CurrentXMLGameObject != nullptr)
				currentGameObject->SerializeGameObject(CurrentXMLGameObject);
		}
	else
		return false;

	m_SceneToSave->Save(CurrentSceneXMLDocument);
	return true;
}
#endif // _EDITOR

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
			continue;
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
#include "imgui.h"
#include "imgui_stdlib.h"

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

void Core::GameObjectManager::ShowEditorControl(ItemSelectionData& p_selectedItem)
{
	for (unsigned int i = 0; i < m_gameObjectsList.size(); i++)
	{
		if (m_gameObjectsList[i]->GetId() == 0u)
			GameObject::SetupGameObjectID(m_gameObjectsList);

		if (ImGui::Selectable((m_gameObjectsList[i]->GetName() + "##" + std::to_string(m_gameObjectsList[i]->GetId())).c_str()))
		{
			p_selectedItem.type = TYPE_ITEM_SELECTED::GAMEOBJECT;
			p_selectedItem.Data.RessourceUUID = 0L;
			p_selectedItem.Data.GameObjectIndex = i;
		}

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			size_t i_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (i_next >= 0 && i_next < m_gameObjectsList.size())
			{
				p_selectedItem.Data.GameObjectIndex = i_next;
				m_gameObjectsList[i_next].swap(m_gameObjectsList[i]);
				ImGui::ResetMouseDragDelta();
			}
		}
	}

	if (ImGui::Button("Add GameObject##GameObjectCreator"))
		m_gameObjectsList.emplace_back(std::make_unique<GameObject>("GameObject"));
}

void Core::GameObjectManager::ShowGameObjectInspector(size_t p_selectedGameObject)
{
	if (p_selectedGameObject < m_gameObjectsList.size())
		m_gameObjectsList[p_selectedGameObject]->ShowEditorControl();
	else
		ImGui::Text("No GameObject or Ressources Selected");
}
#endif // _EDITOR

void Core::GameObjectManager::Draw(uint64_t p_layer) const
{
	for (const GameObjectPtr& CurrentGameObject : m_gameObjectsList)
		if ((p_layer & CurrentGameObject->GetDrawLayer()) > 0)
			CurrentGameObject->Draw(p_layer);
}

void Core::GameObjectManager::DrawUI() const
{
	for (const std::pair<float, std::pair<const GameObject*, Components::IUIRenderComponent*>>& currentuiComponent : m_uiRenderComponents)
		currentuiComponent.second.second->DrawUi(currentuiComponent.second.first);
}