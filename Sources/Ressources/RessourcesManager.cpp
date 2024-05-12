#include "Ressources/RessourcesManager.h"

#include "raylib.h"

#include "Core/Application.h"

Ressources::RessourcesManager::RessourcesManager()
{
#ifdef _EDITOR
	m_RessourceCreatorsName.insert(m_RessourceCreatorsName.begin(), "Add ressource ...");
	m_RessourceCreatorsHash.insert(m_RessourceCreatorsHash.begin(), 0u);
#endif // _EDITOR
}

bool Ressources::RessourcesManager::AddRessourceCreator(const std::type_info& p_typeData, std::function<IRessource* (const int64_t)> p_creator)
{
	if (Core::Application::GetInstance().GetRessourcesManager().m_RessourceCreators.find(p_typeData.hash_code()) != Core::Application::GetInstance().GetRessourcesManager().m_RessourceCreators.end())
		return false;

	Core::Application::GetInstance().GetRessourcesManager().m_RessourceCreators.emplace(p_typeData.hash_code(), p_creator);
#ifdef _EDITOR
	Core::Application::GetInstance().GetRessourcesManager().m_RessourceCreatorsName.push_back(p_typeData.name());
	Core::Application::GetInstance().GetRessourcesManager().m_RessourceCreatorsHash.push_back(p_typeData.hash_code());
	Core::Application::GetInstance().GetRessourcesManager().m_resourcesFilter.insert({ p_typeData.hash_code(), true });
#endif // _EDITOR

	return true;
}

RessourcePtr Ressources::RessourcesManager::RegisterResource(size_t p_TypeHash, int64_t p_resourceUUID)
{
	if (m_RessourceCreators.find(p_TypeHash) == m_RessourceCreators.end())
		return nullptr;
	while (p_resourceUUID == 0L || m_ressources.find(p_resourceUUID) != m_ressources.end())
		p_resourceUUID = m_UUIDGenerator.nextid();

	RessourcePtr NewRessource = RessourcePtr(m_RessourceCreators[p_TypeHash](p_resourceUUID));
	m_ressources.insert({ NewRessource->GetUUID(), NewRessource });

	return NewRessource;
}

void Ressources::RessourcesManager::UnloadUnusedResource()
{
	for (const std::pair<size_t, RessourcePtr>& current : m_ressources)
	{
		if (current.second.use_count() == 2l)
			current.second->PreUnloadResource();
	}

	for (const std::pair<size_t, RessourcePtr>& current : m_ressources)
    {
        if (current.second.use_count() == 2l)
            current.second->UnloadResource();
    }
}

ScenePtr Ressources::RessourcesManager::GetScene(const std::string& m_sceneName)
{
    if (m_scenes.find(m_sceneName) == m_scenes.end())
		return nullptr;

	return m_scenes[m_sceneName];
}

bool Ressources::RessourcesManager::RegisterScene(ScenePtr p_newScene)
{
    if (p_newScene == nullptr)
        return false;

    if (m_scenes.find(p_newScene->GetName()) != m_scenes.end())
		return false;

	m_scenes.insert({ p_newScene->GetName(), p_newScene });
    return true;
}

#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"

void Ressources::RessourcesManager::ShowEditorControl(Core::ItemSelectionData& p_selectedItem)
{
	if (ImGui::Button("Unload unused ressource"))
		UnloadUnusedResource();

	ImGui::SameLine();
	ImGui::Text("|");

	for (size_t indexTypeName = 1u; indexTypeName < m_RessourceCreatorsName.size(); indexTypeName++)
	{
		ImGui::SameLine();
		ImGui::Checkbox(m_RessourceCreatorsName[indexTypeName], &(m_resourcesFilter[m_RessourceCreatorsHash[indexTypeName]]));
	}

	ImGui::Separator();

	ImGui::BeginChild("ListRessources");

	for (const std::pair<int64_t, RessourcePtr>& currentResource : m_ressources)
	{
		if (!m_resourcesFilter[currentResource.second->GetType().hash_code()])
			continue;

		if (currentResource.second->IsLoaded())
			ImGui::Text(" - [ LOADED ]");
		else
			ImGui::TextColored({ 0.6f, 0.3f, 0.3f, 1.f }, " - [UNLOADED]");

		ImGui::SameLine();

		if (ImGui::Selectable(("[ " + std::string(currentResource.second->GetType().name()) + " ] " + currentResource.second->GetName()).c_str()))
		{
			p_selectedItem.type = Core::TYPE_ITEM_SELECTED::RESSOURCE;
			p_selectedItem.Data.RessourceUUID = currentResource.first;
		}

		// drag sources here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			static int64_t currentUUID = 0u;
			currentUUID = currentResource.first;

			ImGui::SetDragDropPayload("DND_RESOURCE", &currentUUID, sizeof(int64_t));

			ImGui::Text("%s (%lld)", currentResource.second->GetName().c_str(), currentUUID);
			ImGui::EndDragDropSource();
		}
	}

	static size_t indexComboBox = 0u;
	if (ImGui::Combo("##comboBoxComponent", (int*)&indexComboBox, m_RessourceCreatorsName.data(), (int)m_RessourceCreatorsName.size()))
	{
		if (indexComboBox != 0)
		{
			RessourcePtr NewRess = RegisterResource(m_RessourceCreatorsHash[indexComboBox]);

			if (NewRess != nullptr)
			{
				p_selectedItem.type = Core::TYPE_ITEM_SELECTED::RESSOURCE;
				p_selectedItem.Data.RessourceUUID = NewRess->GetUUID();
			}

			indexComboBox = 0u;
		}
	}

	ImGui::EndChild();
}

void Ressources::RessourcesManager::ShowSceneControl()
{
	static std::string tempSceneToDelete = "";

	for (std::unordered_map<std::string, ScenePtr>::iterator CurrentScn = m_scenes.begin(); CurrentScn != m_scenes.end(); CurrentScn++)
	{
		ScenePtr& CurrentScnRef = CurrentScn->second;

		if (Core::Application::IsCurrentScene(CurrentScnRef->GetName()))
		{
			ImGui::Text(" X %s", CurrentScnRef->GetName().c_str());

			ImGui::SameLine();
			if (ImGui::Button(("Reload##Scene" + CurrentScnRef->GetName()).c_str()))
				Core::Application::QueryLoadScene(CurrentScnRef->GetName());

			ImGui::SameLine();
			if (ImGui::Button(("Save##Scene" + CurrentScnRef->GetName()).c_str()))
				Core::Application::GetInstance().GetGameObjectManager().SaveScene(CurrentScnRef);
		}
		else
		{
			ImGui::Text(" - %s", CurrentScnRef->GetName().c_str());

			ImGui::SameLine();
			if (ImGui::Button(("Load##Scene" + CurrentScnRef->GetName()).c_str()))
				Core::Application::QueryLoadScene(CurrentScnRef->GetName());

			ImGui::SameLine();
			if (ImGui::Button(("Delete##Scene" + CurrentScnRef->GetName()).c_str()))
				ImGui::OpenPopup(("Delete Scene ?##Scene" + CurrentScnRef->GetName()).c_str());

			ImGui::SameLine();
			if (ImGui::Button(("Copy##Scene" + CurrentScnRef->GetName()).c_str()))
				ImGui::OpenPopup(("Copy Scene ?##Scene" + CurrentScnRef->GetName()).c_str());
		}

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(("Copy Scene ?##Scene" + CurrentScnRef->GetName()).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("this action will override \"%s\" content.\nThis operation cannot be undone !\n\n", CurrentScnRef->GetName().c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				Core::Application::GetInstance().GetGameObjectManager().SaveScene(CurrentScnRef);
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		// Always center this window when appearing
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(("Delete Scene ?##Scene" + CurrentScnRef->GetName()).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("this action will delete \"%s\".\nThis operation cannot be undone !\n\n", CurrentScnRef->GetName().c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				tempSceneToDelete = CurrentScnRef->GetName();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}

	if (tempSceneToDelete != "")
	{
		m_scenes.erase(tempSceneToDelete);
		tempSceneToDelete = "";
	}

	static std::string newSceneName = "";
	ImGui::InputText("##SceneName", &newSceneName);
	ImGui::SameLine();
	if (ImGui::Button("Add Scene##SceneName"))
		if (m_scenes.find(newSceneName) == m_scenes.end())
		{
			m_scenes.insert({ newSceneName, std::make_shared<DefaultScene>(newSceneName) });
			newSceneName = "";
		}
}

void Ressources::RessourcesManager::ShowRessourceInspector(int64_t p_selectedRessource)
{
	static bool deleteCurrent = false;
	static bool directDelete = false;

	if (m_ressources.find(p_selectedRessource) != m_ressources.end())
	{
		ImGui::InputText("##RessourceName", &(m_ressources[p_selectedRessource]->GetName()));
		ImGui::Text("UUID : %lld\ntype : %s\nuse count = %ld", m_ressources[p_selectedRessource]->GetUUID(), m_ressources[p_selectedRessource]->GetType().name(), m_ressources[p_selectedRessource].use_count());

		ImGui::Spacing();

		if (ImGui::Button("Delete##Resource"))
		{
			deleteCurrent = true;
			if (m_ressources[p_selectedRessource].use_count() > 1)
				ImGui::OpenPopup("DeleteResource?");
			else
				directDelete = true;
		}
		ImGui::SameLine();
		if (m_ressources[p_selectedRessource]->IsLoaded())
		{
			if (ImGui::Button("Unload##Resource"))
				m_ressources[p_selectedRessource]->UnloadResource();
			ImGui::SameLine();
			if (ImGui::Button("Reload##Resource"))
			{
				m_ressources[p_selectedRessource]->UnloadResource();
				m_ressources[p_selectedRessource]->LoadResource();
			}
		}
		else
			if (ImGui::Button("Load##Resource"))
				m_ressources[p_selectedRessource]->LoadResource();

		ImGui::Separator();
		m_ressources[p_selectedRessource]->ShowEditorControl();
	}

	if (deleteCurrent)
	{
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("DeleteResource?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("%s (%lld) will be deleted, but he still used by component.\nload and unload while not be process by resources manager anymore !\n\n", m_ressources[p_selectedRessource]->GetName().c_str(), m_ressources[p_selectedRessource]->GetUUID());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				directDelete = true;
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				deleteCurrent = false;
			}

			ImGui::EndPopup();
		}

		if (directDelete)
		{
			m_ressources.erase(p_selectedRessource);
			deleteCurrent = false;
			directDelete = false;
		}
	}
}
#endif // _EDITOR