#include "Components/Component/Transform.h"

#include "Debug/Assert.h"
#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Transform.cpp"

Snowflake<8456134653461L> Components::Transform::transformUUIDgenerator;
std::unordered_map<int64_t, Components::Transform*> Components::Transform::listGlobalTransform = std::unordered_map<int64_t, Components::Transform*>();

Components::Transform::Transform() :
	m_id(transformUUIDgenerator.nextid()),
	m_parentId(0),
	m_childsId(),
	m_position({ 0 }),
	m_rotation({ 0 }),
	m_scale({ 1.f, 1.f, 1.f }),
	m_update(true)
{
	while (listGlobalTransform.find(m_id) != listGlobalTransform.end())
		m_id = transformUUIDgenerator.nextid();

	listGlobalTransform.insert({ m_id, this });
}

Components::Transform::~Transform()
{
	if (m_parentId != 0)
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->RemoveChild(m_id);

	for (int64_t childId : m_childsId)
		if (listGlobalTransform.find(childId) != listGlobalTransform.end())
			listGlobalTransform[childId]->SetParent(0, false);

	listGlobalTransform.erase(m_id);
}

void Components::Transform::SetParent(int64_t p_parentId, bool unsetParent)
{
	if (p_parentId == m_id)
	{
		R3DE_ERROR("impossible to make transform parent to himself !!");
		return;
	}

	if (m_parentId != 0 && unsetParent)
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->RemoveChild(m_id);

	m_parentId = p_parentId;

	if (m_parentId != 0)
	{
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->AddChild(m_id);
		else
		{
			R3DE_WARNING("transform %lld doesn't exist", m_parentId);
			m_parentId = 0;
		}
	}
}

void Components::Transform::AddChild(int64_t p_childId)
{
	m_childsId.push_back(p_childId);
}

void Components::Transform::RemoveChild(int64_t p_childId)
{
	for (size_t i = 0; i < m_childsId.size(); i++)
		if (m_childsId[i] == p_childId)
		{
			m_childsId.erase(m_childsId.begin() + i);
			i--;
		}
}

void Components::Transform::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	int64_t oldId = m_id;
	m_id = p_XMLComponent->Int64Attribute("Id", m_id);

	if (m_id != oldId)
	{
		listGlobalTransform.erase(oldId);

		if (listGlobalTransform.find(m_id) != listGlobalTransform.end())
		{
			R3DE_FATAL("Deserialize Trying to register Transform with already register ID");
			R3DE_ASSERT(false);
		}

		listGlobalTransform.insert({ m_id, this });
	}

	tinyxml2::XMLElement* ParentXML = p_XMLComponent->FirstChildElement("parent");
	if (ParentXML != nullptr)
		m_parentId = ParentXML->Int64Attribute("Id");

	tinyxml2::XMLElement* childXML = p_XMLComponent->FirstChildElement("childs");
	if (childXML != nullptr)
	{
		childXML = childXML->FirstChildElement("child");

		while (childXML != nullptr)
		{
			m_childsId.push_back(childXML->Int64Attribute("Id"));

			childXML = childXML->NextSiblingElement("child");
		}
	}

	tinyxml2::XMLElement* positionXML = p_XMLComponent->FirstChildElement("position");
	if (positionXML != nullptr)
	{
		m_position.x = positionXML->FloatAttribute("X");
		m_position.y = positionXML->FloatAttribute("Y");
		m_position.z = positionXML->FloatAttribute("Z");
	}

	tinyxml2::XMLElement* rotationXML = p_XMLComponent->FirstChildElement("rotation");
	if (rotationXML != nullptr)
	{
		m_rotation.x = rotationXML->FloatAttribute("I");
		m_rotation.y = rotationXML->FloatAttribute("J");
		m_rotation.z = rotationXML->FloatAttribute("K");
		m_rotation.w = rotationXML->FloatAttribute("R");
	}

	tinyxml2::XMLElement* scaleXML = p_XMLComponent->FirstChildElement("scale");
	if (scaleXML != nullptr)
	{
		m_scale.x = scaleXML->FloatAttribute("X", 1.f);
		m_scale.y = scaleXML->FloatAttribute("Y", 1.f);
		m_scale.z = scaleXML->FloatAttribute("Z", 1.f);
	}

	m_update = true;
}

void Components::Transform::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	p_XMLComponent->SetAttribute("Id", m_id);

	if (m_parentId != 0)
	{
		tinyxml2::XMLElement* parentXML = p_XMLComponent->InsertNewChildElement("parent");
		if (parentXML != nullptr)
			parentXML->SetAttribute("Id", m_parentId);
	}

	if (m_childsId.size() > 0u)
	{
		tinyxml2::XMLElement* childsXML = p_XMLComponent->InsertNewChildElement("childs");
		tinyxml2::XMLElement* childXML = nullptr;

		if (childsXML != nullptr)
			for (std::vector<int64_t>::const_iterator ChildIte = m_childsId.begin(); ChildIte != m_childsId.end(); ChildIte++)
			{
				childXML = p_XMLComponent->InsertNewChildElement("child");
				if (childXML != nullptr)
					childXML->SetAttribute("Id", *ChildIte);
			}
	}

	tinyxml2::XMLElement* positionXML = p_XMLComponent->InsertNewChildElement("position");
	if (positionXML != nullptr)
	{
		positionXML->SetAttribute("X", m_position.x);
		positionXML->SetAttribute("Y", m_position.y);
		positionXML->SetAttribute("Z", m_position.z);
	}

	tinyxml2::XMLElement* rotationXML = p_XMLComponent->InsertNewChildElement("rotation");
	if (rotationXML != nullptr)
	{
		rotationXML->SetAttribute("I", m_rotation.x);
		rotationXML->SetAttribute("J", m_rotation.y);
		rotationXML->SetAttribute("K", m_rotation.z);
		rotationXML->SetAttribute("R", m_rotation.w);
	}

	tinyxml2::XMLElement* scaleXML = p_XMLComponent->InsertNewChildElement("scale");
	if (scaleXML != nullptr)
	{
		scaleXML->SetAttribute("X", m_scale.x);
		scaleXML->SetAttribute("Y", m_scale.y);
		scaleXML->SetAttribute("Z", m_scale.z);
	}
}

void Components::Transform::FixedUpdate(Core::GameObject* p_gameObject)
{
	// TODO : Transform update
}

#ifdef _EDITOR
void Components::Transform::EditorFixedUpdate(Core::GameObject* p_gameObject)
{
}
void Components::Transform::ShowEditorControl(const unsigned int p_indexComponent)
{
	// TODO : Transform editor control
}
#endif // _EDITOR