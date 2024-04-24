#include "Core/GameObject.h"

void Core::GameObject::DeserializeGameObject(tinyxml2::XMLElement* p_XMLGameObject)
{
	// component type : Attribute("Type", this->GetComponentType().hash_code()); est le hash code du type a aller chercher dans le ComponentsManager
	// TODO : il faut faire application avant
}

void Core::GameObject::SerializeGameObject(tinyxml2::XMLElement* p_XMLGameObject) const
{
	p_XMLGameObject->SetAttribute("Name", m_name.c_str());

	tinyxml2::XMLElement* CurrentUsedElement = p_XMLGameObject->InsertNewChildElement("Components");
	tinyxml2::XMLElement* ComponentElement = nullptr;

	if (CurrentUsedElement != nullptr)
		for (const ComponentPtr& curentComp : m_components)
		{
			ComponentElement = CurrentUsedElement->InsertNewChildElement("Component");

			if (ComponentElement != nullptr)
				curentComp->SerializeComponent(ComponentElement);
		}
}

void Core::GameObject::Start()
{
	for (ComponentPtr& curentComp : m_components)
		curentComp->Start(this);
}

void Core::GameObject::Update()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->Enabled())
			m_components[i]->Update(this);

		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}

void Core::GameObject::FixedUpdate()
{
	for (ComponentPtr& curentComp : m_components)
		if (curentComp->Enabled())
			curentComp->FixedUpdate(this);
}

#ifdef _EDITOR
void Core::GameObject::EditorUpdate()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->Enabled())
			m_components[i]->EditorUpdate(this);

		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}

void Core::GameObject::ShowEditorControl()
{
	// TODO : faire les controles pour le GameObject !!
}
#endif

void Core::GameObject::Draw() const
{
	for (const ComponentPtr& curentComp : m_components)
		if (curentComp->Enabled())
			curentComp->Draw(this);
}

void Core::GameObject::DrawUI() const
{
	for (const ComponentPtr& curentComp : m_components)
		if (curentComp->Enabled())
			curentComp->DrawUI(this);
}
