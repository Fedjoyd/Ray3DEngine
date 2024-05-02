#include "Core/GameObject.h"

#include "Core/Application.h"

#include "Debug/Assert.h"
#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "GameObject.cpp"

void Core::GameObject::DeserializeGameObject(tinyxml2::XMLElement* p_XMLGameObject)
{
	tinyxml2::XMLElement* ListComponents = p_XMLGameObject->FirstChildElement("Components");
	tinyxml2::XMLElement* ComponentElement = nullptr;

	if (ListComponents != nullptr)
		ComponentElement = ListComponents->FirstChildElement("Component");

	while (ComponentElement != nullptr)
	{
		m_components.emplace_back(Core::Application::GetInstance().GetComponentsManager().CreateComponent(ComponentElement->UnsignedAttribute("Type")));

		m_components.back()->DeserializeComponent(ComponentElement);

		ComponentElement = ComponentElement->NextSiblingElement("Component");
	}
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
		if (m_components[i]->Enabled() && (m_components[i]->GetComponentType() & Components::COMPONENT_TYPE_GAMEPLAY))
		{
			Components::IGameplayComponent* AsGameplayComp = CAST_GAMEPLAY_COMPONENT(m_components[i]);

			if (AsGameplayComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IGameplayComponent");
			R3DE_ASSERT(AsGameplayComp != nullptr);

			AsGameplayComp->Update(this);
		}

		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}

void Core::GameObject::FixedUpdate()
{
	for (ComponentPtr& currentComp : m_components)
		if (currentComp->Enabled() && (currentComp->GetComponentType() & Components::COMPONENT_TYPE_PHYSIC))
		{
			Components::IPhysicComponent* AsPhysicComp = CAST_PHYSIC_COMPONENT(currentComp);

			if (AsPhysicComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IPhysicComponent");
			R3DE_ASSERT(AsPhysicComp != nullptr);

			AsPhysicComp->FixedUpdate(this);
		}
}

#ifdef _EDITOR
void Core::GameObject::EditorUpdate()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->Enabled() && (m_components[i]->GetComponentType() & Components::COMPONENT_TYPE_GAMEPLAY))
		{
			Components::IGameplayComponent* AsGameplayComp = CAST_GAMEPLAY_COMPONENT(m_components[i]);

			if (AsGameplayComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IGameplayComponent");
			R3DE_ASSERT(AsGameplayComp != nullptr);

			AsGameplayComp->EditorUpdate(this);
		}

		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}
void Core::GameObject::EditorFixedUpdate()
{
	for (ComponentPtr& currentComp : m_components)
		if (currentComp->Enabled() && (currentComp->GetComponentType() & Components::COMPONENT_TYPE_PHYSIC))
		{
			Components::IPhysicComponent* AsPhysicComp = CAST_PHYSIC_COMPONENT(currentComp);

			if (AsPhysicComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IPhysicComponent");
			R3DE_ASSERT(AsPhysicComp != nullptr);

			AsPhysicComp->EditorFixedUpdate(this);
		}
}

void Core::GameObject::ShowEditorControl()
{
	// TODO : gameObject editor
}
#endif

void Core::GameObject::Draw() const
{
	for (const ComponentPtr& currentComp : m_components)
		if (currentComp->Enabled() && (currentComp->GetComponentType() & Components::COMPONENT_TYPE_RENDER))
		{
			Components::IRenderComponent* AsRenderComp = CAST_RENDER_COMPONENT(currentComp);

			if (AsRenderComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IRenderComponent");
			R3DE_ASSERT(AsRenderComp != nullptr);

			AsRenderComp->Draw(this);
		}
}

void Core::GameObject::RegisterUiComponent(std::map<float, std::pair<const GameObject*, Components::IUIRenderComponent*>>& p_uiComponent) const
{
	for (const ComponentPtr& currentComp : m_components)
		if (currentComp->Enabled() && (currentComp->GetComponentType() & Components::COMPONENT_TYPE_UI_RENDER))
		{
			Components::IUIRenderComponent* AsUirenderComp = CAST_UI_RENDER_COMPONENT(currentComp);

			if (AsUirenderComp == nullptr) R3DE_FATAL("Le composant n'herite pas de Components::IUIRenderComponent");
			R3DE_ASSERT(AsUirenderComp != nullptr);

			float layer = AsUirenderComp->GetLayer();
			while (p_uiComponent.find(layer) != p_uiComponent.end())
				layer = layer + 0.25f;

			p_uiComponent.insert({layer, {this, AsUirenderComp}});
		}
}
