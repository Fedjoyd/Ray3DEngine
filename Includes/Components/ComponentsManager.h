#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "Define.h"

#include "Components/IComponent.h"

namespace Components
{
	class ComponentsManager
	{
	public:
		ComponentsManager();
		~ComponentsManager() {}

		static bool AddComponentCreator(const std::type_info& p_typeData, std::function<IComponent* (void)> p_creator);

		IComponent* CreateComponent(size_t p_typeHash);

#ifdef _EDITOR
		void ShowEditorControl();

		static bool ShowComponentCombotBox(size_t* p_componentHash);
#endif // _EDITOR

	private:
		std::unordered_map<size_t, std::function<IComponent*(void)>> m_ComponentCreators;

#ifdef _EDITOR
		std::vector<const char*> m_ComponentCreatorsName;
		std::vector<size_t> m_ComponentCreatorsHash;
#endif // _EDITOR
	};
}

#define REGISTER_COMPONENT_CREATOR(CurComponent) Components::ComponentsManager::AddComponentCreator(typeid(CurComponent), CurComponent::Create)