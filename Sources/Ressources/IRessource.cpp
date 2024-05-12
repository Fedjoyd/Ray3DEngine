#include "Ressources/IRessource.h"

void Ressources::IRessource::DeserializeRessource(tinyxml2::XMLElement* p_XMLRessource)
{
	m_name = p_XMLRessource->Attribute("Name");
}

void Ressources::IRessource::SerializeRessource(tinyxml2::XMLElement* p_XMLRessource) const
{
	p_XMLRessource->SetAttribute("Type", this->GetType().hash_code());
	p_XMLRessource->SetAttribute("UUID", m_UUID);
	p_XMLRessource->SetAttribute("Name", m_name.c_str());
}
