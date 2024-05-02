#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

#include "Ressources/Scene.h"

namespace Ressources
{
	class RessourcesManager
	{
	public:
		RessourcesManager() {}
		~RessourcesManager() {}

		ScenePtr GetScene(const std::string& m_sceneName);
		bool RegisterScene(ScenePtr p_newScene);

		void UnloadUnusedRessource();

	private:
		std::vector<ScenePtr> m_Scenes;
	};
}