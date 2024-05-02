#pragma once

#include <memory>

#include "Define.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Camera.h"

#include "Components/IComponent.h"

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

	private:
		Camera3D m_cameraData;
	};
}