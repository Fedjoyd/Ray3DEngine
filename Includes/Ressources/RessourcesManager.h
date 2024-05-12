#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "Define.h"

#include "Snowflake.hpp"

#include "Ressources/Scene.h"
#include "Ressources/IRessource.h"

namespace Ressources
{
	class RessourcesManager
	{
	public:
		RessourcesManager();
		~RessourcesManager() {}

		void LoadProjectRessources(tinyxml2::XMLElement* p_XMLRessourceManager, bool p_dontLoadSceneList = false);
#ifdef _EDITOR
		bool SaveProjectRessources(tinyxml2::XMLElement* p_XMLRessourceManager, bool p_dontSaveSceneList = false);
#endif // _EDITOR

		static bool AddRessourceCreator(const std::type_info& p_typeData, std::function<IRessource* (const int64_t)> p_creator);

		template <class T>
		std::shared_ptr<T> GetAndLoadRessource(const int64_t p_resourceUUID);
		template <class T>
		bool               TryGetAndLoadRessource(const int64_t p_resourceUUID, std::shared_ptr<T>* outResource);
		RessourcePtr       RegisterRessource(size_t p_TypeHash, int64_t p_resourceUUID = 0L);
		void               UnloadUnusedRessource();

		ScenePtr GetScene(const std::string& m_sceneName);
		bool	 RegisterScene(ScenePtr p_newScene);

#ifdef _EDITOR
		void ShowEditorControl(Core::ItemSelectionData& p_selectedItem);
		void ShowSceneControl();
		void ShowRessourceInspector(int64_t p_selectedRessource);

		static bool RessourceDnDTarget(int64_t* p_ptrResID);
#endif // _EDITOR

	private:
		std::unordered_map<int64_t, RessourcePtr> m_ressources;
		std::unordered_map<std::string, ScenePtr> m_scenes;

		Snowflake<1534832906275L> m_UUIDGenerator;

		std::unordered_map<size_t, std::function<IRessource* (const int64_t)>> m_RessourceCreators;

#ifdef _EDITOR
		std::vector<const char*> m_RessourceCreatorsName;
		std::vector<size_t> m_RessourceCreatorsHash;
		std::unordered_map<size_t, bool> m_resourcesFilter;
#endif // _EDITOR
	};

	template<class T>
	inline std::shared_ptr<T> RessourcesManager::GetAndLoadRessource(const int64_t p_resourceUUID)
	{
		if (p_resourceUUID == 0L || m_ressources.find(p_resourceUUID) == m_ressources.end())
			return nullptr;
		if (typeid(*m_ressources[p_resourceUUID]) != typeid(T))
			return nullptr;

		m_ressources[p_resourceUUID]->LoadResource();
		return std::dynamic_pointer_cast<T, IRessource>(m_ressources[p_resourceUUID]);
	}
	template<class T>
	inline bool RessourcesManager::TryGetAndLoadRessource(const int64_t p_resourceUUID, std::shared_ptr<T>* outResource)
	{
		std::shared_ptr<T> toTestReturn = GetAndLoadRessource<T>(p_resourceUUID);

		if (toTestReturn == nullptr)
			return false;

		if (outResource != nullptr)
			*outResource = toTestReturn;

		return true;
	}
}

#define REGISTER_RESSOURCE_CREATOR(CurComponent) Ressources::RessourcesManager::AddRessourceCreator(typeid(CurComponent), CurComponent::Create)