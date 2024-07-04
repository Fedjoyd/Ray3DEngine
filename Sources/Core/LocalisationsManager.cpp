#include "Core/LocalisationsManager.h"

#include "Core/Application.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "LocalisationManager.cpp"

void Core::LocalisationsManager::LoadLanguagesList(tinyxml2::XMLElement* p_LanguageList)
{
	if (p_LanguageList == nullptr)
		return;

	m_languages.clear();
	tinyxml2::XMLElement* languageXML = p_LanguageList->FirstChildElement("Language");

	while (languageXML != nullptr)
	{
		const char* languageIdentifier = languageXML->Attribute("Id");
		const char* languageName = languageXML->GetText();

		if (languageIdentifier != nullptr && languageName != nullptr)
			m_languages.push_back({ std::string(languageIdentifier), std::string(languageName) });

		languageXML = languageXML->NextSiblingElement("Language");
	}
}

bool Core::LocalisationsManager::SetLanguage(size_t p_nextLanguage, bool p_forceLoad)
{
	if ((m_currentLanguage == p_nextLanguage && !p_forceLoad) || p_nextLanguage >= m_languages.size())
		return false;

	m_currentLanguage = p_nextLanguage;

	tinyxml2::XMLDocument m_LangXMLData;
	if (m_LangXMLData.LoadFile((m_path + m_languages[m_currentLanguage].first + ".xml").c_str()) == tinyxml2::XML_SUCCESS)
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

	R3DE_INFO("Game's language set to %s", m_languages[m_currentLanguage].first.c_str());

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
#include "imgui.h"
#include "imgui_stdlib.h"

void Core::LocalisationsManager::SaveLanguageList(tinyxml2::XMLElement* p_LanguageList)
{
	if (p_LanguageList == nullptr)
		return;

	tinyxml2::XMLElement* languageXML = nullptr;

	for (size_t currentIndex = 0u; currentIndex < m_languages.size(); currentIndex++)
	{
		languageXML = p_LanguageList->InsertNewChildElement("Language");

		if (languageXML == nullptr)
			continue;

		languageXML->SetAttribute("Id", m_languages[currentIndex].first.c_str());
		languageXML->SetText(m_languages[currentIndex].second.c_str());
	}
}

void Core::LocalisationsManager::SaveCurrentLanguage()
{
	if (m_currentLanguage >= m_languages.size())
		return;

	tinyxml2::XMLDocument currentLanguageXMLDocument;
	currentLanguageXMLDocument.Parse("<?xml version=\"1.0\"?><Datas></Datas>");

	tinyxml2::XMLElement* listXMLDatas = currentLanguageXMLDocument.FirstChildElement("Datas");
	tinyxml2::XMLElement* currentXMLData = nullptr;

	for (std::unordered_map<std::string, std::string>::const_iterator ConstLangIterator = m_data.begin(); ConstLangIterator != m_data.end(); ConstLangIterator++)
	{
		currentXMLData = listXMLDatas->InsertNewChildElement("Data");

		if (currentXMLData == nullptr)
			continue;

		currentXMLData->SetAttribute("Id", ConstLangIterator->first.c_str());
		currentXMLData->SetText(ConstLangIterator->second.c_str());
	}

	currentLanguageXMLDocument.SaveFile((m_path + m_languages[m_currentLanguage].first + ".xml").c_str());
}

void Core::LocalisationsManager::ShowEditorControl()
{
	if (ImGui::BeginCombo("##comboLanguageSelection", (m_languages[m_currentLanguage].first + " - " + m_languages[m_currentLanguage].second).c_str()))
	{
		for (size_t n = 0; n < m_languages.size(); n++)
		{
			const bool is_selected = (m_currentLanguage == n);
			if (ImGui::Selectable((m_languages[n].first + " - " + m_languages[n].second).c_str(), is_selected))
				SetLanguage(n);

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();

	static std::string neLanguageId = "";
	static std::string neLanguageName = "";

	if (ImGui::Button("Add language"))
	{
		neLanguageId = "";
		neLanguageName = "";
		ImGui::OpenPopup("New Language");
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("New Language", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("New language id", &neLanguageId);
		ImGui::InputText("New language name", &neLanguageName);

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_languages.push_back({ neLanguageId, neLanguageName });
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button("Save Language Data"))
		SaveCurrentLanguage();

	ImGui::Separator();

	ImGui::BeginChild("scrollInData");

	if (ImGui::BeginTable("LanguageDataTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Key");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		std::vector<std::string> ToDelete;

		for (std::unordered_map<std::string, std::string>::iterator row = m_data.begin(); row != m_data.end(); row++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(row->first.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
			ImGui::InputText(("##ValueOf" + row->first).c_str(), &(row->second));

			ImGui::TableSetColumnIndex(2);
			if (ImGui::Button(("Delete##DeleteOf" + row->first).c_str()))
				ToDelete.push_back(row->first);
		}

		for (std::vector<std::string>::const_iterator iterToDel = ToDelete.begin(); iterToDel != ToDelete.end(); iterToDel++)
			m_data.erase(*iterToDel);

		static std::string LanguageKeyToAdd = "";

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::SetNextItemWidth(ImGui::GetColumnWidth(0));
		ImGui::InputText("##KeyDataToAdd", &LanguageKeyToAdd);

		ImGui::TableSetColumnIndex(1);
		if (ImGui::Button("Add"))
		{
			if (LanguageKeyToAdd == "" || m_data.find(LanguageKeyToAdd) != m_data.end())
				R3DE_WARNING("Can't add empty or existing Key on data");
			else
				m_data.insert({ LanguageKeyToAdd, std::string("") });

			LanguageKeyToAdd = "";
		}

		ImGui::EndTable();
	}

	ImGui::EndChild();
}
#endif // _EDITOR