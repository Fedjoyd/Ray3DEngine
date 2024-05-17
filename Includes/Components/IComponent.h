#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "Define.h"

#include "tinyxml2.h"

#define REGISTER_COMPONENT(CurComponent, CompType) \
	virtual const std::type_info& GetType() const override { return typeid(*this); } \
	virtual Components::COMPONENT_TYPE GetComponentType() const override { return CompType; } \
	static Components::IComponent* Create(void) { return (Components::IComponent*)(new CurComponent()); }

namespace Core
{
	class GameObject;
}

namespace Components
{
	enum COMPONENT_TYPE_ : unsigned char
	{
		COMPONENT_TYPE_NOTHING = 0,
		COMPONENT_TYPE_GAMEPLAY = 1 << 0,
		COMPONENT_TYPE_PHYSIC = 1 << 1,
		COMPONENT_TYPE_RENDER = 1 << 2,
		COMPONENT_TYPE_UI_RENDER = 1 << 3
	};
	typedef unsigned char COMPONENT_TYPE;

	class IComponent
	{
	public:
		IComponent() : m_enabled(true), m_deleteFlag(false) {}
		virtual ~IComponent() {}

		// surcharge OBLIGATOIRE : const std::type_info& GetType() const override { return typeid(*this); }
		virtual const std::type_info& GetType() const = 0;
		virtual COMPONENT_TYPE GetComponentType() const = 0;

		bool Enabled() const { return m_enabled; }
		void SetEnabled(bool p_value = true) { m_enabled = p_value; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		// ---- Load/Save - Deserialize/Serialize ----

		virtual void DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent); // load Component from XML
		virtual void SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const; // save Component to XML

		// ---- Runtime/Editor methode ----

		virtual void Start(Core::GameObject* p_gameObject) {}
#ifdef _EDITOR
		// don't forget preprocessor : #ifdef _EDITOR and #endif
		virtual void ShowEditorControl(const unsigned int p_indexComponent) {}
#endif // _EDITOR

	private:
		bool m_enabled;
		bool m_deleteFlag;
	};

	class IGameplayComponent
	{
	public:
		IGameplayComponent() {}
		virtual ~IGameplayComponent() {}

		virtual void Update(Core::GameObject* p_gameObject) = 0;
#ifdef _EDITOR
		// don't forget preprocessor : #ifdef _EDITOR and #endif
		virtual void EditorUpdate(Core::GameObject* p_gameObject) {}
#endif // _EDITOR
	private:
	};

	class IPhysicComponent
	{
	public:
		IPhysicComponent() {}
		virtual ~IPhysicComponent() {}

		virtual void FixedUpdate(Core::GameObject* p_gameObject) = 0;
#ifdef _EDITOR
		// don't forget preprocessor : #ifdef _EDITOR and #endif
		virtual void EditorFixedUpdate(Core::GameObject* p_gameObject) {}
#endif // _EDITOR
	private:
	};

	class IRenderComponent
	{
	public:
		IRenderComponent() {}
		virtual ~IRenderComponent() {}

		virtual void Draw(const Core::GameObject* p_gameObject) = 0;
		virtual uint64_t GetDrawLayer() const = 0;
	private:
	};

	class IUIRenderComponent
	{
	public:
		IUIRenderComponent() {}
		virtual ~IUIRenderComponent() {}

		virtual void DrawUi(const Core::GameObject* p_gameObject) = 0;
		virtual float GetLayer() const { return 0.f; }
	private:
	};
}

typedef std::unique_ptr<Components::IComponent> ComponentPtr;

#define CAST_GAMEPLAY_COMPONENT(CastToComponent) dynamic_cast<Components::IGameplayComponent*>(CastToComponent.get())
#define CAST_PHYSIC_COMPONENT(CastToComponent) dynamic_cast<Components::IPhysicComponent*>(CastToComponent.get())
#define CAST_RENDER_COMPONENT(CastToComponent) dynamic_cast<Components::IRenderComponent*>(CastToComponent.get())
#define CAST_UI_RENDER_COMPONENT(CastToComponent) dynamic_cast<Components::IUIRenderComponent*>(CastToComponent.get())