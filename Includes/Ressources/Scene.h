#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

#include "tinyxml2.h"

namespace Ressources
{
	class Scene
	{
	public:
		Scene() {}
		~Scene() {}

		virtual const std::string& GetName() const = 0;

		// ---- Load/Save - Deserialize/Serialize ----

		virtual bool LoadXmlDocument(tinyxml2::XMLDocument& dst) = 0;
		virtual void Save(tinyxml2::XMLDocument& dst) {}
	};

	class DefaultScene : public Scene
	{
	public:
		DefaultScene() : m_name("default"), m_path(DEFAULT_SCENE_PATH) {}
		DefaultScene(const std::string& p_name) : m_name(p_name), m_path(DEFAULT_SCENE_PATH) {}
		DefaultScene(const std::string& p_name, const std::string& p_path) : m_name(p_name), m_path(p_path) {}
		~DefaultScene() {}

		std::string& GetName() { return m_name; }
		const std::string& GetName() const override { return m_name; }

		// ---- Load/Save - Deserialize/Serialize ----

		bool LoadXmlDocument(tinyxml2::XMLDocument& dst) override;
		void Save(tinyxml2::XMLDocument& dst) override;

	private:
		std::string m_name;
		std::string m_path;
	};
}

typedef std::shared_ptr<Ressources::Scene> ScenePtr;
#define CAST_SCENE(CastToScene) std::static_pointer_cast<Ressources::Scene>(CastToScene)