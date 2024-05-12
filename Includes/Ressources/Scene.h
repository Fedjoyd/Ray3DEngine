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
#ifdef _EDITOR
		virtual void Save(tinyxml2::XMLDocument& dst) {}
#endif // _EDITOR
	};

	class DefaultScene : public Scene
	{
	public:
		DefaultScene(const std::string& p_name) : m_name(p_name), m_path(DEFAULT_SCENE_PATH) {}
		DefaultScene(const std::string& p_name, const std::string& p_path) : m_name(p_name), m_path(p_path) {}
		~DefaultScene() {}

		const std::string& GetName() const override { return m_name; }
		std::string& GetPath() { return m_path; }

		// ---- Load/Save - Deserialize/Serialize ----

		bool LoadXmlDocument(tinyxml2::XMLDocument& dst) override;
#ifdef _EDITOR
		void Save(tinyxml2::XMLDocument& dst) override;
#endif // _EDITOR

	private:
		std::string m_name;
		std::string m_path;
	};
}

typedef std::shared_ptr<Ressources::Scene> ScenePtr;
#define CAST_SCENE(CastToScene) std::static_pointer_cast<Ressources::Scene>(CastToScene)