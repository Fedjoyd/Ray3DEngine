#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

namespace Ressources
{
	class Scene
	{
	public:
		Scene() : m_name("") {}
		~Scene() {}

		std::string& GetName() { return m_name; }
		const std::string& GetName() const { return m_name; }

	private:
		std::string m_name;
	};
}

typedef std::shared_ptr<Ressources::Scene> ScenePtr;