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

		void SetCurrentCamera(Components::Camera* p_CurrentCamera = nullptr) { m_CurrentCamera = p_CurrentCamera; }
		bool TestCurrentCamera(const Components::Camera* p_CurrentCamera) { return p_CurrentCamera == m_CurrentCamera; }

#ifdef _EDITOR
		void SetFreeFly(bool p_freeFly = true) { m_freeFly = p_freeFly; }
		bool IsFreeFly() const { return m_freeFly; }

		void ShowEditorControl();
#endif // _EDITOR

		void Update();

		const Camera3D& GetCameraData() const;
		bool IsCursorLock() const { return (m_CurrentCamera == nullptr ? false : m_CurrentCamera->IsCursorLock()); }

	private:
#ifdef _EDITOR
		bool m_freeFly;
		bool m_CursorLock;
		Camera3D m_freeFlyData;
#endif // _EDITOR

		Components::Camera* m_CurrentCamera;

		float m_defaultFovY;      // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
		int m_defaultProjection;  // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC

		static const Camera3D Default3DCam;
	};
}