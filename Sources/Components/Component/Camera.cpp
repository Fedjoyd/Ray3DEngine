#include "Components/Component/Camera.h"

#include "Core/Application.h"

Components::Camera::Camera() : m_cameraData({ 0 }), m_cursorLock(false), m_indexTransform(0u), m_ColorBackground(RAYWHITE), m_size({10, 10}), m_RenderCameraTexture(false), m_CameraTexture({0}), m_drawLayer(UINT64_MAX)
{
	Core::Application::GetCamerasManager().AddCamera(this);
	
	m_cameraData.up.y = 1.f;
	m_cameraData.fovy = 45.f;

	m_CameraTexture = LoadRenderTexture(m_size.Width, m_size.Height);
}

Components::Camera::~Camera()
{
	Core::Application::GetCamerasManager().RemoveCamera(this);

	if (Core::Application::GetCamerasManager().TestCurrentCamera(this))
		Core::Application::GetCamerasManager().SetCurrentCamera();

	UnloadRenderTexture(m_CameraTexture);
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
	Core::Application::GetCamerasManager().SetCurrentCamera(this);
}

void Components::Camera::Start(Core::GameObject* p_gameObject)
{
}

void Components::Camera::Update(Core::GameObject* p_gameObject)
{


	if (m_CameraTexture.texture.width != m_size.Width || m_CameraTexture.texture.height != m_size.Height)
	{
		UnloadRenderTexture(m_CameraTexture);
		m_CameraTexture = LoadRenderTexture(m_size.Width, m_size.Height);
	}
}

#ifdef _EDITOR
void Components::Camera::EditorUpdate(Core::GameObject* p_gameObject)
{
	Update(p_gameObject);
}

void Components::Camera::ShowEditorControl(const unsigned int p_indexComponent)
{
	ImGui::SameLine();

	if (Core::Application::GetCamerasManager().TestCurrentCamera(this))
	{
		if (ImGui::Button(("Deselect##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetCamerasManager().SetCurrentCamera();
	}
	else
	{
		if (ImGui::Button(("Select##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetCamerasManager().SetCurrentCamera(this);
	}

	ImGui::Spacing();

	ImGui::InputInt(("Transform Index##camera" + std::to_string(p_indexComponent)).c_str(), (int*)&m_indexTransform);

	//ImGui::DragFloat3(("Position##camera" + std::to_string(p_indexComponent)).c_str(), (float*)&(m_cameraData.position));

	ImGui::DragFloat(("Fov Y##camera" + std::to_string(p_indexComponent)).c_str(), &(m_cameraData.fovy));

	static bool isOrthographic = false;
	isOrthographic = (m_cameraData.projection == CAMERA_ORTHOGRAPHIC);
	ImGui::Checkbox(("Is orthographic##camera" + std::to_string(p_indexComponent)).c_str(), &isOrthographic);
	m_cameraData.projection = (isOrthographic ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE);

	ImGui::Spacing();

	ImGui::InputInt2(("Size##camera" + std::to_string(p_indexComponent)).c_str(), (int*)&m_size);
	ImGui::Checkbox(("Allways render texture##camera" + std::to_string(p_indexComponent)).c_str(), &m_RenderCameraTexture);

	rlImGuiImageRenderTexture(&m_CameraTexture);
}
#endif // _EDITOR