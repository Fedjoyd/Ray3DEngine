#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Application.h"

#include "Ressources/Scene.h"

#include "Core/GameObjectManager.h"
#include "Core/CamerasManager.h"
#include "Core/InputsManager.h"
#include "Core/LocalisationsManager.h"
#include "Core/SettingsManager.h"
#include "Core/Time.h"

#include "Components/ComponentsManager.h"
#include "Ressources/RessourcesManager.h"

namespace Core
{
	class Application
	{
	public:
		~Application() {}

		static Application& GetInstance() { return m_singleton; }

		static const std::string& GetName() { return m_singleton.m_name; }
		static void Initialize();
		static bool ShouldExit() { return ((WindowShouldClose() && !IsKeyDown(KEY_ESCAPE)) || m_singleton.m_shouldExit); }
		static void Kill() { m_singleton.m_shouldExit = true; }

		static void Update();
		static void Draw();

		static void SetLogger(Debug::ILogger* p_logger = nullptr) { m_singleton.m_log.SetLogger(p_logger); }

		// ---- Application module getter ----

		GameObjectManager& GetGameObjectManager() { return m_gameObjectManager; }
		const GameObjectManager& GetGameObjectManager() const { return m_gameObjectManager; }
		CamerasManager& GetCamerasManager() { return m_camerasManager; }
		const CamerasManager& GetCamerasManager() const { return m_camerasManager; }
		InputsManager& GetInputsManager() { return m_inputsManager; }
		const InputsManager& GetInputsManager() const { return m_inputsManager; }
		LocalisationsManager& GetLocalisationsManager() { return m_localisationsManager; }
		const LocalisationsManager& GetLocalisationsManager() const { return m_localisationsManager; }
		SettingsManager& GetSettingsManager() { return m_settingsManager; }
		const SettingsManager& GetSettingsManager() const { return m_settingsManager; }

		Components::ComponentsManager& GetComponentsManager() { return m_componentsManager; }
		const Components::ComponentsManager& GetComponentsManager() const { return m_componentsManager; }
		Ressources::RessourcesManager& GetRessourcesManager() { return m_ressourcesManager; }
		const Ressources::RessourcesManager& GetRessourcesManager() const { return m_ressourcesManager; }

		Debug::Log& GetLog() { return m_log; }
		const Debug::Log& GetLog() const { return m_log; }

		Time& GetTime() { return m_time; }
		const Time& GetTime() const { return m_time; }

	private:
		static Application m_singleton;

		Application() : m_shouldExit(false), m_name("none"), m_queryLoadScene(false), m_nextSceneName("") {}

		std::string m_name;
		bool        m_shouldExit;

		// change scene
		bool        m_queryLoadScene;
		std::string m_nextSceneName;

		// ---- Data Storage ----

		GameObjectManager m_gameObjectManager;
		CamerasManager m_camerasManager;
		InputsManager m_inputsManager;
		LocalisationsManager m_localisationsManager;
		SettingsManager m_settingsManager;

		Components::ComponentsManager m_componentsManager;
		Ressources::RessourcesManager m_ressourcesManager;

		Debug::Log m_log;

		Time m_time;

		// ---- Private Methode ----
	};
}