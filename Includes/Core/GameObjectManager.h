#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Define.h"

#include "tinyxml2.h"

#include "Ressources/Scene.h"
#include "Core/GameObject.h"
#include "Components/IComponent.h"

namespace Core
{
	class GameObjectManager
	{
	public:
		GameObjectManager() {}
		~GameObjectManager() {}

		void LoadScene(ScenePtr m_SceneToLoad);
		bool SaveScene(ScenePtr& m_SceneToLoad);

		std::vector<GameObjectPtr>& GetGameObjects() { return m_gameObjectsList; }
		const std::vector<GameObjectPtr>& GetGameObjects() const { return m_gameObjectsList; }

		// ---- Runtime/Editor methode ----

		void Start();
		void Update();
		void FixedUpdate();
#ifdef _EDITOR
		void EditorUpdate();
		void EditorFixedUpdate();

		void ShowManagerWindow(bool* p_opened = nullptr);
		void ShowEditorWindow(bool* p_opened = nullptr);
#endif // _EDITOR
		void Draw() const;
		void DrawUI() const;

	private:
		std::vector<GameObjectPtr> m_gameObjectsList;

		std::map<float, std::pair<const GameObject*, Components::IUIRenderComponent*>> m_uiRenderComponents;
	};
}