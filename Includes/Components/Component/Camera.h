#pragma once

#include <memory>

#include "Define.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Camera.h"

#include "Components/IComponent.h"

struct CameraSize {
	int Width;
	int Height;
};

namespace Components
{
	class Camera : public IComponent, public IGameplayComponent
	{
	public:
		Camera();
		~Camera();

		REGISTER_COMPONENT(Camera, COMPONENT_TYPE_GAMEPLAY)

		// ---- Load/Save - Deserialize/Serialize ----

		void DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent) override; // load Component from XML
		void SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const override; // save Component to XML

		// ---- Runtime/Editor methode ----

		void SetCurrentCamera();

		void Start(Core::GameObject* p_gameObject) override;
		void Update(Core::GameObject* p_gameObject) override;
#ifdef _EDITOR
		void EditorUpdate(Core::GameObject* p_gameObject) override;
		void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // _EDITOR

		const Camera3D& GetCameraData() const { return m_cameraData; }

		/*Vector3& GetPosition() { return m_cameraData.position; }
		const Vector3& GetPosition() const { return m_cameraData.position; }
		Vector3& GetTarget() { return m_cameraData.target; }
		const Vector3& GetTarget() const { return m_cameraData.target; }
		Vector3& GetUp() { return m_cameraData.up; }
		const Vector3& GetUp() const { return m_cameraData.up; }/**/
		float& GetFovY() { return m_cameraData.fovy; }
		const float& GetFovY() const { return m_cameraData.fovy; }
		int& GetProjection() { return m_cameraData.projection; }
		const int& GetProjection() const { return m_cameraData.projection; }

		bool& IsCursorLock() { return m_cursorLock; }
		bool IsCursorLock() const { return m_cursorLock; }

		Color& BackgroundColor() { return m_ColorBackground; }
		const Color& BackgroundColor() const { return m_ColorBackground; }

		bool& RenderCameraTexture() { return m_RenderCameraTexture; }
		bool RenderCameraTexture() const { return m_RenderCameraTexture; }

		const RenderTexture2D& CameraTexture() const { return m_CameraTexture; }

		CameraSize& CameraTextureSize() { return m_size; }
		const CameraSize& CameraTextureSize() const  { return m_size; }

		uint64_t DrawLayer() const { return m_drawLayer; }

	private:
		Camera3D m_cameraData;
		bool m_cursorLock;

		unsigned int m_indexTransform;

		CameraSize m_size;
		Color m_ColorBackground;
		RenderTexture2D m_CameraTexture;
		bool m_RenderCameraTexture;

		uint64_t m_drawLayer;
	};
}