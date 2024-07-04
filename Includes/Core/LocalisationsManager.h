#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "Define.h"

#include "tinyxml2.h"

namespace Core
{
	class LocalisationsManager
	{
	public:
		LocalisationsManager() : m_currentLanguage(0u), m_path(DEFAULT_LOCATION_PATH) { m_languages.push_back({"english", "English"}); }
		~LocalisationsManager() {}

		void SetLocalisationFilePath(const std::string& p_newPath) { m_path = p_newPath; }
		void LoadLanguagesList(tinyxml2::XMLElement* p_LanguageList);
		const std::vector<std::pair<std::string, std::string>>& GetLanguages() { return m_languages; }
		
		bool SetLanguage(size_t p_nextLanguage, bool p_forceLoad = false);
		size_t GetLanguage() { return m_currentLanguage; }

		static const std::string& GetLocalFormat(const std::string& p_Identifier);

#ifdef _EDITOR
		void SaveLanguageList(tinyxml2::XMLElement* p_LanguageList);
		void SaveCurrentLanguage();

		void ShowEditorControl();
#endif // _EDITOR

	private:
		size_t m_currentLanguage;
		std::vector<std::pair<std::string, std::string>> m_languages;
		std::unordered_map<std::string, std::string> m_data;

		std::string m_path;

		const std::string c_ERROR_MESSAGE = "????";
	};
}

#define GET_LOCAL(Identifier) Core::LocalisationsManager::GetLocalFormat(Identifier)