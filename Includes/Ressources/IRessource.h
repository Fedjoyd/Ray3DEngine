#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <typeinfo>

#include "tinyxml2.h"

#include "Define.h"

#define REGISTER_RESSOURCE(CurRessource) \
	virtual const std::type_info& GetType() const override { return typeid(*this); } \
	static Ressources::IRessource* Create(const int64_t TheUUID) { return (Ressources::IRessource*)(new CurRessource(TheUUID)); }

namespace Ressources
{
	class IRessource
	{
	public:
		IRessource(const int64_t p_UUID) : m_name(""), m_UUID(p_UUID), m_loaded(false) {}
		virtual ~IRessource() { UnloadResource(); }

		// surcharge OBLIGATOIRE : const std::type_info& GetType() const override { return typeid(*this); }
		virtual const std::type_info& GetType() const = 0;

		std::string& GetName() { return m_name; }
		const std::string& GetName() const { return m_name; }
		int64_t GetUUID() const { return m_UUID; }
		bool IsLoaded() const { return m_loaded; }

		// ---- Load/Save - Deserialize/Serialize ----

		virtual void DeserializeRessource(tinyxml2::XMLElement* p_XMLRessource); // load Ressource from XML
		virtual void SerializeRessource(tinyxml2::XMLElement* p_XMLRessource) const; // save Ressource to XML

		// ---- Runtime/Editor methode ----

		virtual void LoadResource() { m_loaded = true; }
		virtual void PreUnloadResource() {} // used for bundled ressource to detach under ressource (can also be call in recursive mode, if needed))
		virtual void UnloadResource() { m_loaded = false; } // used for single ressource (Don't call recursivelly)

#ifdef _EDITOR
		virtual void ShowEditorControl() {}
#endif // _EDITOR

	private:
		std::string m_name;
		int64_t m_UUID;
		bool m_loaded;
	};
}

typedef std::shared_ptr<Ressources::IRessource> RessourcePtr;