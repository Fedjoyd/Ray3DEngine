#include "Core/CamerasManager.h"

#include "Core/Application.h"

#define R3DE_CURRENT_FILE "CamerasManager.cpp"

const Camera3D Core::CamerasManager::Default3DCam = Camera3D{ Vector3{ 0.0f, 10.0f, 10.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };

Core::CamerasManager::CamerasManager() :
	m_mainCamera(nullptr),
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

void Core::CamerasManager::UseCamera(Components::Camera* p_cameraToUse)
{
	if (p_cameraToUse != nullptr)
		m_currentCamera = p_cameraToUse;
	else
		m_currentCamera = m_mainCamera;

	if (m_currentCamera != nullptr)
	{
		const Camera3D& CurentCameDat = m_currentCamera->GetCameraData();
		const CameraSize& camSizeRef = m_currentCamera->CameraTextureSize();

		//m_CurrentViewMatrix = MatrixLookAt(CurentCameDat.position, CurentCameDat.target, CurentCameDat.up);
		m_CurrentViewMatrix = GetCameraMatrix(CurentCameDat);

		if (CurentCameDat.projection == CAMERA_ORTHOGRAPHIC)
			m_CurrentProjectionMatrix = MatrixOrtho(-(camSizeRef.Width / 2.f), (camSizeRef.Width / 2.f), -(camSizeRef.Height / 2.f), (camSizeRef.Height / 2.f), DEFAULT_NEAR, DEFAULT_FAR);
		else
			m_CurrentProjectionMatrix = MatrixPerspective(CurentCameDat.fovy, ((float)camSizeRef.Width / (float)camSizeRef.Height), DEFAULT_NEAR, DEFAULT_FAR);
	}
	else
	{
		CameraSize camSizeRef = { GetRenderWidth(), GetRenderHeight() };

		//m_CurrentViewMatrix = MatrixLookAt(Default3DCam.position, Default3DCam.target, Default3DCam.up);
		m_CurrentViewMatrix = GetCameraMatrix(Default3DCam);

		if (Default3DCam.projection == CAMERA_ORTHOGRAPHIC)
			m_CurrentProjectionMatrix = MatrixOrtho(-(camSizeRef.Width / 2.f), (camSizeRef.Width / 2.f), -(camSizeRef.Height / 2.f), (camSizeRef.Height / 2.f), DEFAULT_NEAR, DEFAULT_FAR);
		else
			m_CurrentProjectionMatrix = MatrixPerspective(Default3DCam.fovy, ((float)camSizeRef.Width / (float)camSizeRef.Height), DEFAULT_NEAR, DEFAULT_FAR);
	}

#ifdef _EDITOR
	m_ImGuiViewMatrix[0] = m_CurrentViewMatrix.m0; m_ImGuiViewMatrix[1] = m_CurrentViewMatrix.m1; m_ImGuiViewMatrix[2] = m_CurrentViewMatrix.m2; m_ImGuiViewMatrix[3] = m_CurrentViewMatrix.m3;
	m_ImGuiViewMatrix[4] = m_CurrentViewMatrix.m4; m_ImGuiViewMatrix[5] = m_CurrentViewMatrix.m5; m_ImGuiViewMatrix[6] = m_CurrentViewMatrix.m6; m_ImGuiViewMatrix[7] = m_CurrentViewMatrix.m7;
	m_ImGuiViewMatrix[8] = m_CurrentViewMatrix.m8; m_ImGuiViewMatrix[9] = m_CurrentViewMatrix.m9; m_ImGuiViewMatrix[10] = m_CurrentViewMatrix.m10; m_ImGuiViewMatrix[11] = m_CurrentViewMatrix.m11;
	m_ImGuiViewMatrix[12] = m_CurrentViewMatrix.m12; m_ImGuiViewMatrix[13] = m_CurrentViewMatrix.m13; m_ImGuiViewMatrix[14] = m_CurrentViewMatrix.m14; m_ImGuiViewMatrix[15] = m_CurrentViewMatrix.m15;

	m_ImGuiProjectionMatrix[0] = m_CurrentProjectionMatrix.m0; m_ImGuiProjectionMatrix[1] = m_CurrentProjectionMatrix.m1; m_ImGuiProjectionMatrix[2] = m_CurrentProjectionMatrix.m2; m_ImGuiProjectionMatrix[3] = m_CurrentProjectionMatrix.m3;
	m_ImGuiProjectionMatrix[4] = m_CurrentProjectionMatrix.m4; m_ImGuiProjectionMatrix[5] = m_CurrentProjectionMatrix.m5; m_ImGuiProjectionMatrix[6] = m_CurrentProjectionMatrix.m6; m_ImGuiProjectionMatrix[7] = m_CurrentProjectionMatrix.m7;
	m_ImGuiProjectionMatrix[8] = m_CurrentProjectionMatrix.m8; m_ImGuiProjectionMatrix[9] = m_CurrentProjectionMatrix.m9; m_ImGuiProjectionMatrix[10] = m_CurrentProjectionMatrix.m10; m_ImGuiProjectionMatrix[11] = m_CurrentProjectionMatrix.m11;
	m_ImGuiProjectionMatrix[12] = m_CurrentProjectionMatrix.m12; m_ImGuiProjectionMatrix[13] = m_CurrentProjectionMatrix.m13; m_ImGuiProjectionMatrix[14] = m_CurrentProjectionMatrix.m14; m_ImGuiProjectionMatrix[15] = m_CurrentProjectionMatrix.m15;
#endif
}

Components::Camera* Core::CamerasManager::GetRenderedCamera()
{
	return Core::Application::GetCamerasManager().m_currentCamera;
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
	if (!TestMainCamera(nullptr))
		if (ImGui::Button("Deselect camera"))
			SetMainCamera(nullptr);

	static size_t IndexCamera = 0u;
	IndexCamera = 0u;

	for (std::vector<Components::Camera*>::iterator CameraIte = m_listCamera.begin(); CameraIte != m_listCamera.end(); CameraIte++)
	{
		if (TestMainCamera(*CameraIte))
			ImGui::Text("X Camera %zu", IndexCamera);
		else
			if (ImGui::Selectable(("- Camera " + std::to_string(IndexCamera)).c_str()))
				SetMainCamera(*CameraIte);

		IndexCamera++;
	}
}
#endif // _EDITOR