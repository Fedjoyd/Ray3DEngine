#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Define.h"

#include "tinyxml2.h"

#include "Components/IComponent.h"

namespace Core
{
	class GameObject
	{
	public:
		GameObject(const std::string& p_name) : m_name(p_name), m_deleteFlag(false), m_components(), m_DrawLayer(0u) {}
		~GameObject() {}

		std::string& GetName() { return m_name; }
		const std::string& GetName() const { return m_name; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		std::vector<ComponentPtr>& GetComponents() { return m_components; }
		const std::vector<ComponentPtr>& GetComponents() const { return m_components; }

		// ---- Component Getter ----
		template <class T>
		T* GetComponent(const unsigned int indexComponent = 0u);
		template <class T>
		bool TryGetComponent(T** out_component, const unsigned int indexComponent = 0u);
		template <class T>
		std::vector<T*> GetAllComponents();

		// ---- constant Component Getter ----
		template <class T>
		T* GetComponent(const unsigned int indexComponent = 0u) const;
		template <class T>
		bool TryGetComponent(T** out_component, const unsigned int indexComponent = 0u) const;
		template <class T>
		std::vector<const T*> GetAllComponents() const;

		// ---- Load/Save - Deserialize/Serialize ----

		void DeserializeGameObject(tinyxml2::XMLElement* p_XMLGameObject); // load GameObject from XML
		void SerializeGameObject(tinyxml2::XMLElement* p_XMLGameObject) const; // save GameObject to XML

		// ---- Runtime/Editor methode ----

		void Start();
		void Update();
		void FixedUpdate();
#ifdef _EDITOR
		void EditorUpdate();
		void EditorFixedUpdate();

		void ShowEditorControl();

		unsigned int GetId() const { return m_ID; }

		static void SetupGameObjectID(std::vector<std::unique_ptr<Core::GameObject>>& m_gameObjectsList);
#endif // _EDITOR
		void Draw(uint64_t p_layer = UINT64_MAX) const;
		uint64_t GetDrawLayer() const { return m_DrawLayer; }
		void ProcessDrawLayer();

		void RegisterUiComponent(std::map<float, std::pair<const GameObject*, Components::IUIRenderComponent*>>& p_uiComponent) const;

	private:
		std::string m_name;
		bool        m_deleteFlag;

		std::vector<ComponentPtr> m_components;

		uint64_t m_DrawLayer;

#ifdef _EDITOR
		unsigned int m_ID = 0u;
#endif // _EDITOR
	};

	// ---- Component Getter ----

	template<class T>
	inline T* GameObject::GetComponent(const unsigned int indexComponent)
	{
		T* toReturnPtr = nullptr;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetType() == typeid(T))
			{
				toReturnPtr = dynamic_cast<T*>(m_components[i].get());

				if (currentComponent < indexComponent)
					currentComponent++;
				else
					return toReturnPtr;
			}
		}

		return toReturnPtr;
	}

	template<class T>
	inline bool GameObject::TryGetComponent(T** out_component, const unsigned int indexComponent)
	{
		T* toTestReturn = GetComponent<T>(indexComponent);

		if (toTestReturn == nullptr)
			return false;

		if (out_component != nullptr)
			*out_component = toTestReturn;

		return true;
	}

	template<class T>
	inline std::vector<T*> GameObject::GetAllComponents()
	{
		std::vector<T*> toReturnlist;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetType() == typeid(T))
				toReturnlist.push_back(dynamic_cast<T*>(m_components[i].get()));
		}

		return toReturnlist;
	}

	// ---- constant Component Getter ----

	template<class T>
	inline T* GameObject::GetComponent(const unsigned int indexComponent) const
	{
		T* toReturnPtr = nullptr;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetType() == typeid(T))
			{
				toReturnPtr = dynamic_cast<T*>(m_components[i].get());

				if (currentComponent < indexComponent)
					currentComponent++;
				else
					return toReturnPtr;
			}
		}

		return toReturnPtr;
	}

	template<class T>
	inline bool GameObject::TryGetComponent(T** out_component, const unsigned int indexComponent) const
	{
		T* toTestReturn = GetComponent<T>(indexComponent);

		if (toTestReturn == nullptr)
			return false;

		if (out_component != nullptr)
			*out_component = toTestReturn;

		return true;
	}

	template<class T>
	inline std::vector<const T*> GameObject::GetAllComponents() const
	{
		std::vector<const T*> toReturnlist;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetType() == typeid(T))
				toReturnlist.push_back(dynamic_cast<const T*>(m_components[i].get()));
		}

		return toReturnlist;
	}
}

typedef std::unique_ptr<Core::GameObject> GameObjectPtr;
//#define newGameObject(GOName) std::make_unique<Core::GameObject>(Core::GameObject(GOName))