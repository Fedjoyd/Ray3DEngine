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
		ComponentsManager() {}
		~ComponentsManager() {}

		bool AddComponentCreator(size_t p_typeHash, std::function<IComponent*(void)> p_creator);

		IComponent* CreateComponent(size_t p_typeHash);

	private:
		std::unordered_map<size_t, std::function<IComponent*(void)>> m_ComponentCreators;
	};
}