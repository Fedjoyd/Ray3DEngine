#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "Define.h"

#include "tinyxml2.h"

namespace Core
{
	class GameObject;
}

namespace Components
{
	class Component
	{
	public:
		Component() : m_enabled(true), m_deleteFlag(false) {}
		virtual ~Component() {}

		// surcharge OBLIGATOIRE : const std::type_info& GetComponentType() const override { return typeid(*this); }
		virtual const std::type_info& GetComponentType() const = 0;

		bool Enabled() const { return m_enabled; }
		void SetEnabled(bool p_value = true) { m_enabled = p_value; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		// ---- Load/Save - Deserialize/Serialize ----

		virtual void DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent); // load Component from XML
		virtual void SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const; // save Component to XML

		// ---- Runtime/Editor methode ----

		virtual void Start(Core::GameObject* p_gameObject) {}
		virtual void Update(Core::GameObject* p_gameObject) {}
		virtual void FixedUpdate(Core::GameObject* p_gameObject) {}
#ifdef _EDITOR
		// don't forget preprocessor : #ifdef MODE_EDITOR and #endif
		virtual void EditorUpdate(Core::GameObject* p_gameObject) {}
		// don't forget preprocessor : #ifdef MODE_EDITOR and #endif
		virtual void ShowEditorControl(const unsigned int p_indexComponent) {}
#endif // _EDITOR
		virtual void Draw(const Core::GameObject* p_gameObject) const {}
		virtual void DrawUI(const Core::GameObject* p_gameObject) const {}

	private:
		bool m_enabled;
		bool m_deleteFlag;
	};
}

typedef std::unique_ptr<Components::Component> ComponentPtr;
#define CAST_COMPONENT(CastToComponent) std::static_pointer_cast<Components::Component>(CastToComponent)