#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>

#include "Define.h"

#include "tinyxml2.h"
#include "Snowflake.hpp"
#include "raylib.h"

#include "Components/IComponent.h"

namespace Components
{
	class Transform : public IComponent, public IPhysicComponent
	{
	public:
		Transform();
		~Transform();

		REGISTER_COMPONENT(Transform, COMPONENT_TYPE_PHYSIC)

		int64_t GetID() const { return m_id; }

		int64_t	GetParent() const { return m_parentId; }
		bool	HasParent() const { return (m_parentId != 0); }
		void	SetParent(int64_t p_parentId, bool unsetParent = true);
		void	AddChild(int64_t p_childId);
		void	RemoveChild(int64_t p_childId);

		// ---- Load/Save - Deserialize/Serialize ----

		virtual void DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent); // load Component from XML
		virtual void SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const; // save Component to XML

		// ---- Runtime/Editor methode ----

		virtual void FixedUpdate(Core::GameObject* p_gameObject) override;
#ifdef _EDITOR
		virtual void EditorFixedUpdate(Core::GameObject* p_gameObject) override;
		virtual void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // _EDITOR

		// ---- static global ----

		static void InitUUIDGenerator() { transformUUIDgenerator.init(2, 2); }

	private:
		int64_t m_id;

		int64_t m_parentId;
		std::vector<int64_t> m_childsId;
		
		Vector3		m_position;
		Quaternion	m_rotation;
		Vector3		m_scale;

		bool m_update;

#ifdef _EDITOR
#endif // _EDITOR

		// ---- static global ----

		static Snowflake<8456134653461L> transformUUIDgenerator;
		static std::unordered_map<int64_t, Transform*> listGlobalTransform;
	};
}