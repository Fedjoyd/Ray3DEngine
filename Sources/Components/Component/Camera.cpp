#include "Components/Component/Camera.h"

#include "Core/Application.h"
#include "Components/Component/Transform.h"

Components::Camera::Camera() : m_cameraData({ 0 }), m_forwardVector({ 0 }), m_rightVector({0}), m_cursorLock(false), m_indexTransform(0u), m_colorBackground(RAYWHITE), m_cameraTextureSize({10, 10}), m_renderCameraTexture(false), m_cameraTexture({0}), m_drawLayer(UINT64_MAX)
{
	Core::Application::GetCamerasManager().AddCamera(this);

	m_cameraData.up.y = 1.f;
	m_cameraData.fovy = 45.f;

	m_cameraTexture = LoadRenderTexture(m_cameraTextureSize.Width, m_cameraTextureSize.Height);
}

Components::Camera::~Camera()
{
	Core::Application::GetCamerasManager().RemoveCamera(this);

	if (Core::Application::GetCamerasManager().TestMainCamera(this))
		Core::Application::GetCamerasManager().SetMainCamera();

	UnloadRenderTexture(m_cameraTexture);
}

void Components::Camera::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	IComponent::DeserializeComponent(p_XMLComponent);
}

void Components::Camera::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	IComponent::SerializeComponent(p_XMLComponent);
}

void Components::Camera::Start(Core::GameObject* p_gameObject)
{
}

void Components::Camera::Update(Core::GameObject* p_gameObject)
{
	static Components::Transform* CurrentPositionTransform = nullptr;
	if (p_gameObject->TryGetComponent(&CurrentPositionTransform, m_indexTransform))
	{
		m_cameraData.position = CurrentPositionTransform->GetGlobalPosition();

		m_rightVector = CurrentPositionTransform->GetGlobalRight();
		m_forwardVector = CurrentPositionTransform->GetGlobalFoward();
		m_cameraData.up = CurrentPositionTransform->GetGlobalUp();

		m_cameraData.target = Vector3Add(m_cameraData.position, m_forwardVector);
	}

	if (Core::Application::GetCamerasManager().TestMainCamera(this))
	{
		if (IsWindowResized())
			m_cameraTextureSize = { GetRenderWidth(), GetRenderHeight() };

		if (m_renderCameraTexture && (m_cameraTexture.texture.width != m_cameraTextureSize.Width || m_cameraTexture.texture.height != m_cameraTextureSize.Height))
		{
			UnloadRenderTexture(m_cameraTexture);
			m_cameraTexture = LoadRenderTexture(m_cameraTextureSize.Width, m_cameraTextureSize.Height);
		}
	}
	else
		if (m_cameraTexture.texture.width != m_cameraTextureSize.Width || m_cameraTexture.texture.height != m_cameraTextureSize.Height)
		{
			UnloadRenderTexture(m_cameraTexture);
			m_cameraTexture = LoadRenderTexture(m_cameraTextureSize.Width, m_cameraTextureSize.Height);
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

	if (Core::Application::GetCamerasManager().TestMainCamera(this))
	{
		if (ImGui::Button(("Deselect##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetCamerasManager().SetMainCamera();
	}
	else
	{
		if (ImGui::Button(("Select##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::Application::GetCamerasManager().SetMainCamera(this);
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

	ImGui::InputInt2(("Size##camera" + std::to_string(p_indexComponent)).c_str(), (int*)&m_cameraTextureSize);
	ImGui::Checkbox(("Allways render texture##camera" + std::to_string(p_indexComponent)).c_str(), &m_renderCameraTexture);

	rlImGuiImageRenderTexture(&m_cameraTexture);
}
#endif // _EDITOR