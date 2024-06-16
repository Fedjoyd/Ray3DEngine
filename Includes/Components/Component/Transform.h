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
#include "raymath.h"

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

		Vector3&	Position() { m_update = true; return m_position; }
		Quaternion&	Rotation() { m_update = true; return m_rotation; }
		Vector3&	Scale() { m_update = true; return m_scale; }

		const Vector3&		GetPosition() const { return m_position; }
		const Quaternion&	GetRotation() const { return m_rotation; }
		const Vector3&		GetScale() const { return m_scale; }

		//void	SetPosition(Vector3 p_newPosition) { m_updatePhysicsOnUpdate = false; m_update = true; m_position = p_newPosition; }
		//void	SetRotation(Quaternion p_newRotation) { m_updatePhysicsOnUpdate = false; m_update = true; m_rotation = p_newRotation; }
		//void	SetScale(Vector3 p_newScale) { m_updatePhysicsOnUpdate = false; m_update = true; m_scale = p_newScale; }

		Vector3	GetGlobalRight() const { return { m_globalMatrix.m0, m_globalMatrix.m1, m_globalMatrix.m2 }; }
		Vector3	GetGlobalUp() const { return { m_globalMatrix.m4, m_globalMatrix.m5, m_globalMatrix.m6 }; }
		Vector3	GetGlobalFoward() const { return { m_globalMatrix.m8, m_globalMatrix.m9, m_globalMatrix.m10 }; }
		Vector3	GetGlobalPosition() const { return { m_globalMatrix.m12, m_globalMatrix.m13, m_globalMatrix.m14 }; }

		Quaternion		GetGlobalRotation() const;
		Vector3			GetGlobalScale() const;
		const Matrix&	GetGlobalMatrix() const { return m_globalMatrix; }

		// ---- Load/Save - Deserialize/Serialize ----

		virtual void DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent); // load Component from XML
		virtual void SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const; // save Component to XML

		// ---- Runtime/Editor methode ----

		virtual void Start(Core::GameObject* p_gameObject) override;
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

		Matrix	m_localMatrix;
		Matrix	m_globalMatrix;
		bool	m_update;

		bool IsAlreadyParent(int64_t m_transID, int64_t p_parentIdToTest);

#ifdef _EDITOR
		bool toTest = true;
		Vector3 m_editorRotation = Vector3{ 0 };
		std::string m_transformName = "";

		float m_currentGuizmoMatrix[16] = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
		Matrix m_ProcessGuizmoMatrix = MatrixIdentity();

		void UpdateTransform(bool updateEditorRotation = true);
#else
		void UpdateTransform();
#endif // _EDITOR

		// ---- static global ----

		static Snowflake<8456134653461L> transformUUIDgenerator;
		static std::unordered_map<int64_t, Transform*> listGlobalTransform;
	};
}