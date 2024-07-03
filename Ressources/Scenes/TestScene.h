#pragma once
#include <string>
#include "Define.h"
#include "tinyxml2.h"
#include "Ressources/Scene.h"

class Scene_TestScene_Integrated : public Ressources::Scene
{
public:
	Scene_TestScene_Integrated() {}
	~Scene_TestScene_Integrated() {}

	std::string GetName() const override { return "TestScene"; }
	bool LoadXmlDocument(tinyxml2::XMLDocument & dst) override { return (tinyxml2::XML_SUCCESS == dst.Parse("<?xml version=\"1.0\"?><?xml version=\"1.0\"?><GameObjects><GameObject Name=\"GameObject\"><Components><Component Enabled=\"true\" Type=\"2606396356\"/><Component Enabled=\"true\" Type=\"626503649\" Id=\"8639959387504648192\"><position X=\"1.3\" Y=\"1.2\" Z=\"1.2\"/><rotation I=\"-0.1309109\" J=\"0.13218537\" K=\"-0.08086431\" R=\"0.97920901\"/><scale X=\"0.80000001\" Y=\"0.80000001\" Z=\"0.80000001\"/></Component></Components></GameObject></GameObjects>")); }
};