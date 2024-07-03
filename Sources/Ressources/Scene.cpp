#include "Ressources/Scene.h"

bool Ressources::DefaultScene::LoadXmlDocument(tinyxml2::XMLDocument& dst)
{
	tinyxml2::XMLError result = dst.LoadFile((m_path + m_name + ".xml").c_str());
	return (result == tinyxml2::XML_SUCCESS);
}

#ifdef _EDITOR
#include <fstream>

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Scene.cpp"

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void Ressources::DefaultScene::Save(tinyxml2::XMLDocument& dst)
{
	dst.SaveFile((m_path + m_name + ".xml").c_str());

	std::ofstream SceneHeaderFile((m_path + m_name + ".h").c_str());
	if (SceneHeaderFile)
	{
		tinyxml2::XMLPrinter XMLprinter(0, true);
		dst.Print(&XMLprinter);

		SceneHeaderFile << "#pragma once\n";
		SceneHeaderFile << "#include <string>\n";
		SceneHeaderFile << "#include \"Define.h\"\n";
		SceneHeaderFile << "#include \"tinyxml2.h\"\n";
		SceneHeaderFile << "#include \"Ressources/Scene.h\"\n\n";

		SceneHeaderFile << "class Scene_" << ReplaceAll(m_name, " ", "_") << "_Integrated : public Ressources::Scene\n{\n";
		SceneHeaderFile << "public:\n";
		SceneHeaderFile << "\tScene_" << ReplaceAll(m_name, " ", "_") << "_Integrated() {}\n";
		SceneHeaderFile << "\t~Scene_" << ReplaceAll(m_name, " ", "_") << "_Integrated() {}\n\n";
		SceneHeaderFile << "\tstd::string GetName() const override { return \"" << m_name << "\"; }\n";
		SceneHeaderFile << "\tbool LoadXmlDocument(tinyxml2::XMLDocument & dst) override { return (tinyxml2::XML_SUCCESS == dst.Parse(\"<?xml version=\\\"1.0\\\"?>";
		SceneHeaderFile << ReplaceAll(XMLprinter.CStr(), "\"", "\\\"");
		SceneHeaderFile << "\")); }\n};";
	}
	else
		R3DE_ERROR("Impossible d'ouvrire le fichier \"%s\"", (m_name + ".h").c_str());
}
#endif // _EDITOR
