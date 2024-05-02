#include "Components/Component/Camera.h"

#include "Core/Application.h"

Components::Camera::Camera()
{
}

Components::Camera::~Camera()
{
	if (Core::Application::GetInstance().GetCamerasManager().TestCurrentCamera(this))
		Core::Application::GetInstance().GetCamerasManager().SetCurrentCamera();
}

void Components::Camera::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	IComponent::DeserializeComponent(p_XMLComponent);
}

void Components::Camera::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	IComponent::SerializeComponent(p_XMLComponent);
}

void Components::Camera::SetCurrentCamera()
{
	Core::Application::GetInstance().GetCamerasManager().SetCurrentCamera(this);
}

void Components::Camera::Start(Core::GameObject* p_gameObject)
{
}

void Components::Camera::Update(Core::GameObject* p_gameObject)
{
}

#ifdef _EDITOR
void Components::Camera::EditorUpdate(Core::GameObject* p_gameObject)
{
}

void Components::Camera::ShowEditorControl(const unsigned int p_indexComponent)
{
	IComponent::ShowEditorControl(p_indexComponent);
}
#endif // _EDITOR