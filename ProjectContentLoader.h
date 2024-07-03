#pragma once

#include "Components/ComponentsManager.h"
#include "Ressources/RessourcesManager.h"

//#include "YourHeaderFileWithPathHere.h"
#include "Ressources/Code/TestComponent.h"
#include "Ressources/Code/TestRessource.h"

//#include "YourSceneHeaderFileWithPathHere.h"
#include "Ressources/Scenes/TestScene.h"

void LoadComponentsType()
{
    //REGISTER_COMPONENT_CREATOR(ToRegisterComponent);

    REGISTER_COMPONENT_CREATOR(TestComponent);
}

void LoadRessourcesType()
{
    //REGISTER_RESSOURCE_CREATOR(ToRegisterRessource);

    REGISTER_RESSOURCE_CREATOR(TestRessource);
    REGISTER_RESSOURCE_CREATOR(TestRessource2);
}

void RegisterIntegratedScene(Ressources::RessourcesManager& m_RessourceManager)
{
    //m_RessourceManager.RegisterScene(std::make_shared<ToRegisterScene>());

    m_RessourceManager.RegisterScene(std::make_shared<Scene_TestScene_Integrated>());
}