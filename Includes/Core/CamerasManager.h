#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

#include "raylib.h"
#include "raymath.h"

#ifdef _EDITOR
#include "imgui.h"
#include "rlImGui.h"
#endif // _EDITOR

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "CamerasManager.h"

#include "Components/Component/Camera.h"

namespace Core
{
	class CamerasManager
	{
	public:
		CamerasManager();
		~CamerasManager() {}

		void SetMainCamera(Components::Camera* p_currentCamera = nullptr) { m_mainCamera = p_currentCamera; if (m_mainCamera != nullptr) { m_mainCamera->UpdateToRenderSize(); } }
		bool TestMainCamera(const Components::Camera* p_currentCamera) { return p_currentCamera == m_mainCamera; }

		bool AddCamera(Components::Camera* p_camera);
		bool RemoveCamera(Components::Camera* p_camera);

		std::vector<Components::Camera*>& GetListCamera() { return m_listCamera; }
		const std::vector<Components::Camera*>& GetListCamera() const { return m_listCamera; }

		void UseCamera(Components::Camera* p_cameraToUse = nullptr);

		static Components::Camera* GetRenderedCamera();

#ifdef _EDITOR
		void SetFreeFly(bool p_freeFly = true) { m_freeFly = p_freeFly; }
		bool IsFreeFly() const { return m_freeFly; }

		void ShowEditorControl();

		const float* GetImGuiViewMatrix() { return m_ImGuiViewMatrix; }
		const float* GetImGuiProjectionMatrix() { return m_ImGuiProjectionMatrix; }
#endif // _EDITOR

		void Update();

		const Camera3D& GetCameraData() const;
		Color GetBackgroundColor() const { return (m_currentCamera == nullptr ? RAYWHITE : m_currentCamera->BackgroundColor()); }
		uint64_t GetDrawLayer() const {return (m_currentCamera == nullptr ? UINT64_MAX : m_currentCamera->DrawLayer()); }
		const Matrix& GetViewMatrix() const { return m_CurrentViewMatrix; }
		const Matrix& GetProjectionMatrix() const { return m_CurrentProjectionMatrix; }

		bool IsCursorLock() const { return (m_currentCamera == nullptr ? false : m_currentCamera->IsCursorLock()); }

	private:
#ifdef _EDITOR
		bool m_freeFly = false;
		bool m_CursorLock = false;
		Camera3D m_freeFlyData = { 0 };

		float m_ImGuiViewMatrix[16] = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
		float m_ImGuiProjectionMatrix[16] = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
#endif // _EDITOR

		Matrix m_CurrentViewMatrix;
		Matrix m_CurrentProjectionMatrix;

		Components::Camera* m_mainCamera;
		Components::Camera* m_currentCamera;
		std::vector<Components::Camera*> m_listCamera;

		float m_defaultFovY;      // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
		int m_defaultProjection;  // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC

		static const Camera3D Default3DCam;
	};
}