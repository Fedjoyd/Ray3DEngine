#include "Ressources/Scene.h"

bool Ressources::DefaultScene::LoadXmlDocument(tinyxml2::XMLDocument& dst)
{
	tinyxml2::XMLError result = dst.LoadFile((m_path + m_name + ".xml").c_str());
	return (result == tinyxml2::XML_SUCCESS);
}

void Ressources::DefaultScene::Save(tinyxml2::XMLDocument& dst)
{
	dst.SaveFile((m_path + m_name + ".xml").c_str());

	// TODO aslo make .h for intern load
}
