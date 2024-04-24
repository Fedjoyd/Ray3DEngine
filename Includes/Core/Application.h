#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Application.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Ressources/Scene.h"

#include "Core/GameObjectManager.h"
#include "Core/CamerasManager.h"
#include "Core/InputsManager.h"
#include "Core/LocalizationsManager.h"
#include "Core/SettingsManager.h"

#include "Components/ComponentsManager.h"
#include "Ressources/RessourcesManager.h"

namespace Core
{
	class Application
	{
	public:
		~Application() {}

		static Application& GetInstance() { return m_singleton;
		}

		static const std::string& GetName() { return m_singleton.m_name; }
		static void Initialize();
		static bool ShouldExit() { return ((WindowShouldClose() && !IsKeyDown(KEY_ESCAPE)) || m_singleton.m_shouldExit); }
		static void Kill() { m_singleton.m_shouldExit = true; }

		static void Update();
		static void Draw();

		static void SetLogger(Debug::ILogger* p_logger = nullptr) { m_singleton.m_log.SetLogger(p_logger); }

	private:
		static Application m_singleton;

		Application() : m_shouldExit(false), m_name("none"), m_queryLoadScene(false), m_nextSceneName("") {}

		std::string m_name;
		bool        m_shouldExit;

		// change scene
		bool        m_queryLoadScene;
		std::string m_nextSceneName;

		// ---- Data Storage ----

		Core::GameObjectManager m_gameObjectManager;
		Core::CamerasManager m_camerasManager;
		Core::InputsManager m_inputsManager;
		Core::LocalizationsManager m_localizationsManager;
		Core::SettingsManager m_settingsManager;

		Components::ComponentsManager m_componentsManager;
		Ressources::RessourcesManager m_ressourcesManager;

		Debug::Log m_log;

		// ---- Private Methode ----
	};
}