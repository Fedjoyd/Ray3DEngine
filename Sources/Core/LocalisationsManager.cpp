#include "Core/LocalisationsManager.h"

#include "Core/Application.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "LocalisationManager.cpp"

void Core::LocalisationsManager::LoadLanguageList(tinyxml2::XMLElement* p_LanguageList)
{
}

bool Core::LocalisationsManager::SetLanguage(size_t p_nextLanguage, bool p_forceLoad)
{
	if ((m_currentLanguage == p_nextLanguage && !p_forceLoad) || p_nextLanguage >= m_language.size())
		return false;

	m_currentLanguage = p_nextLanguage;

	tinyxml2::XMLDocument m_LangXMLData;
	if (m_LangXMLData.LoadFile((m_path + m_language[m_currentLanguage].first + ".xml").c_str()) == tinyxml2::XML_SUCCESS)
	{
		m_data.clear();

		tinyxml2::XMLElement* datasListXML = m_LangXMLData.FirstChildElement("Datas");
		if (datasListXML != nullptr)
		{
			tinyxml2::XMLElement* dataXML = datasListXML->FirstChildElement("Data");

			while (dataXML != nullptr)
			{
				const char* dataIdentifier = dataXML->Attribute("Id");
				const char* dataText = dataXML->GetText();

				if (dataIdentifier != nullptr && dataText != nullptr)
					m_data.insert({ std::string(dataIdentifier), std::string(dataText) });

				dataXML = dataXML->NextSiblingElement("Data");
			}
		}
	}

	R3DE_INFO("Game's language set to %s", m_language[m_currentLanguage].first.c_str());

	return true;
}

const std::string& Core::LocalisationsManager::GetLocalFormat(const std::string& p_Identifier)
{
	LocalisationsManager& LocalManRef = Core::Application::GetLocalisationsManager();

	if (LocalManRef.m_data.find(p_Identifier) == LocalManRef.m_data.end())
	{
		R3DE_WARNING("Can't find \"%s\" in localisation database !!", p_Identifier.c_str());
		return LocalManRef.c_ERROR_MESSAGE;
	}

	return LocalManRef.m_data[p_Identifier];
}

#ifdef _EDITOR
void Core::LocalisationsManager::SaveLanguageList(tinyxml2::XMLElement* p_LanguageList)
{
}

void Core::LocalisationsManager::SaveCurrentLanguage()
{
}

void Core::LocalisationsManager::ShowEditorControl()
{
}
#endif // _EDITOR