#include "Components/IComponent.h"

void Components::IComponent::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	m_enabled = p_XMLComponent->BoolAttribute("Enabled");
}

void Components::IComponent::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	p_XMLComponent->SetAttribute("Enabled", m_enabled);
	p_XMLComponent->SetAttribute("Type", this->GetType().hash_code());
}

#ifdef _EDITOR
void Components::IComponent::ShowEditorControl(const unsigned int p_indexComponent)
{
	// TODO : component editor
}
#endif // _EDITOR
