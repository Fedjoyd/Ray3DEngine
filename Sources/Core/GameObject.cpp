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
#include "imgui.h"
#include "imgui_stdlib.h"

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
	ImGui::InputText("##GameObjectName", &m_name);
	ImGui::SameLine();
	if (ImGui::Button("Delete##GameObject"))
		ImGui::OpenPopup("DeleteGameObject?");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("DeleteGameObject?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("%s will be deleted.\nThis operation cannot be undone !\n\n", m_name.c_str());
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			SetDeleteFlag(true);
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::Separator();

	static bool IsChecked = false;
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		IsChecked = m_components[i]->Enabled();
		if (ImGui::Checkbox(("##EnabledComponent_" + std::to_string(i)).c_str(), &IsChecked))
			m_components[i]->SetEnabled(IsChecked);
		ImGui::SameLine();
		ImGui::Text("Component : %s", m_components[i]->GetType().name());

		if (ImGui::Button(("Delete##Component_" + std::to_string(i)).c_str()))
			m_components[i]->SetDeleteFlag(true);

		m_components[i]->ShowEditorControl(i);
		ImGui::Separator();
	}

	size_t ComponentHash = 0;
	if (Components::ComponentsManager::ShowComponentCombotBox(&ComponentHash))
		m_components.emplace_back(Core::Application::GetInstance().GetComponentsManager().CreateComponent(ComponentHash));
}

void Core::GameObject::SetupGameObjectID(std::vector<GameObjectPtr>& m_gameObjectsList)
{
	unsigned int newID = 1u;

	for (GameObjectPtr& currentGo : m_gameObjectsList)
	{
		currentGo->m_ID = newID;
		newID = newID + 1u;
	}
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
