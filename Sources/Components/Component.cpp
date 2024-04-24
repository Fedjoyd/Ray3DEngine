#include "Components/Component.h"

void Components::Component::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	m_enabled = p_XMLComponent->BoolAttribute("Enabled");
}

void Components::Component::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	p_XMLComponent->SetAttribute("Enabled", m_enabled);
	p_XMLComponent->SetAttribute("Type", this->GetComponentType().hash_code());
}
