#include "Components/Component/Camera.h"

#include "Core/Application.h"

Components::Camera::Camera() : m_cameraData({ 0 }), m_cursorLock(false)
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
	ImGui::SameLine();

	if (Core::Application::GetInstance().GetCamerasManager().TestCurrentCamera(this))
	{
		if (ImGui::Button(("Deselect##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetInstance().GetCamerasManager().SetCurrentCamera();
	}
	else
	{
		if (ImGui::Button(("Select##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetInstance().GetCamerasManager().SetCurrentCamera(this);
	}

	ImGui::Spacing();

	//ImGui::InputInt(("Transform##camera" + std::to_string(p_indexComponent)).c_str(), (int*)&IndexTransform);

	static bool isOrthographic = false;
	isOrthographic = (m_cameraData.projection == CAMERA_ORTHOGRAPHIC);
	ImGui::Checkbox(("Is orthographic##camera" + std::to_string(p_indexComponent)).c_str(), &isOrthographic);
	m_cameraData.projection = (isOrthographic ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE);
}
#endif // _EDITOR