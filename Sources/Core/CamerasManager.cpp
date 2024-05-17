#include "Core/CamerasManager.h"

#define R3DE_CURRENT_FILE "CamerasManager.cpp"

const Camera3D Core::CamerasManager::Default3DCam = Camera3D{ Vector3{ 0.0f, 10.0f, 10.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };

Core::CamerasManager::CamerasManager() :
#ifdef _EDITOR
	m_freeFly(false),
	m_CursorLock(false),
	m_freeFlyData({ 0 }),
#endif // _EDITOR
	m_currentCamera(nullptr),
	m_defaultFovY(45.0f),
	m_defaultProjection(CAMERA_PERSPECTIVE)
{
#ifdef _EDITOR
	m_freeFlyData.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
	m_freeFlyData.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	m_freeFlyData.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	m_freeFlyData.fovy = 45.0f;                                // Camera field-of-view Y
	m_freeFlyData.projection = CAMERA_PERSPECTIVE;             // Camera mode type
#endif // _EDITOR
}

bool Core::CamerasManager::AddCamera(Components::Camera* p_camera)
{
	if (p_camera == nullptr)
		return false;

	for (std::vector<Components::Camera*>::iterator CameraIte = m_listCamera.begin(); CameraIte != m_listCamera.end(); CameraIte++)
		if ((*CameraIte) == p_camera)
			return false;

	m_listCamera.push_back(p_camera);
	return true;
}

bool Core::CamerasManager::RemoveCamera(Components::Camera* p_camera)
{
	if (p_camera == nullptr)
		return false;

	for (std::vector<Components::Camera*>::iterator CameraIte = m_listCamera.begin(); CameraIte != m_listCamera.end(); CameraIte++)
	{
		if ((*CameraIte) != p_camera)
			continue;

		m_listCamera.erase(CameraIte);
		return true;
	}

	return false;
}

void Core::CamerasManager::Update()
{
#ifdef _EDITOR
/*	m_freeFlyData.fovy = m_defaultFovY;
	m_freeFlyData.projection = m_defaultProjection;

	ImGuiIO& currentIO = ImGui::GetIO();

	if ((m_freeFly || m_currentCamera == nullptr) && !m_CursorLock && IsKeyPressed(KEY_TAB) && !currentIO.WantCaptureMouse)
	{
		DisableCursor();
		m_CursorLock = true;
		m_freeFly = true;
		currentIO.ConfigFlags = ImGuiConfigFlags_NoMouse;
		currentIO.AddFocusEvent(false);
	}
	else if (m_freeFly && m_CursorLock && IsKeyPressed(KEY_TAB))
	{
		EnableCursor();
		m_CursorLock = false;
		currentIO.ConfigFlags = ImGuiConfigFlags_None;
	}

	if (m_freeFly && m_CursorLock)
		UpdateCamera(&m_freeFlyData, CAMERA_FREE);/**/
#endif // _EDITOR
}

const Camera3D& Core::CamerasManager::GetCameraData() const
{
#ifdef _EDITOR
	if (m_freeFly || m_currentCamera == nullptr)
		return m_freeFlyData;
#endif // _EDITOR

	if (m_currentCamera != nullptr)
		return m_currentCamera->GetCameraData();

	return Default3DCam;
}

#ifdef _EDITOR
void Core::CamerasManager::ShowEditorControl()
{
	if (!TestCurrentCamera(nullptr))
		if (ImGui::Button("Deselect camera"))
			SetCurrentCamera(nullptr);

	static size_t IndexCamera = 0u;
	IndexCamera = 0u;

	for (std::vector<Components::Camera*>::iterator CameraIte = m_listCamera.begin(); CameraIte != m_listCamera.end(); CameraIte++)
	{
		if (TestCurrentCamera(*CameraIte))
			ImGui::Text("X Camera %zu", IndexCamera);
		else
			if (ImGui::Selectable(("- Camera " + std::to_string(IndexCamera)).c_str()))
				SetCurrentCamera(*CameraIte);

		IndexCamera++;
	}
}
#endif // _EDITOR