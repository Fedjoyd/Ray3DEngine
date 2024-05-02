#include "Core/Application.h"

#include "Debug/Assert.h"

#define R3DE_CURRENT_FILE "Application.cpp"

Core::Application Core::Application::m_singleton;

void Core::Application::Initialize()
{
	Debug::Log::SetInstance(&(m_singleton.m_log));

	R3DE_INFO("Initialisation Finished !!");
}

void Core::Application::Update()
{
    if (m_singleton.m_queryLoadScene)
    {
        std::shared_ptr<Ressources::Scene> nextScene = m_singleton.m_ressourcesManager.GetScene(m_singleton.m_nextSceneName);

        if (nextScene != nullptr)
        {
            m_singleton.m_gameObjectManager.LoadScene(nextScene);
            m_singleton.m_ressourcesManager.UnloadUnusedRessource();

            R3DE_INFO("Scene %s loaded !", nextScene->GetName().c_str());

            m_singleton.m_gameObjectManager.Start();
        }
        else
            R3DE_ERROR("Scene %s does not exist !", m_singleton.m_nextSceneName.c_str());

        m_singleton.m_nextSceneName = "";
        m_singleton.m_queryLoadScene = false;
    }

    m_singleton.m_time.Update();
    m_singleton.GetCamerasManager().Update();

    m_singleton.m_gameObjectManager.Update();

    int FixedUpdateCount = Time::Fixing();
    while (FixedUpdateCount > 0)
    {
        m_singleton.m_gameObjectManager.FixedUpdate();
        FixedUpdateCount--;
    }
}

void Core::Application::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(m_singleton.GetCamerasManager().GetCameraData());

    //DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
    //DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
    //DrawSphere(camera.target, 1.f, RED);

    m_singleton.m_gameObjectManager.Draw();

    DrawGrid(10, 1.0f);

    EndMode3D();

    //DrawText(CameraTargetStr.c_str(), 10, 40, 20, DARKGRAY);
    m_singleton.m_gameObjectManager.DrawUI();

    DrawFPS(10, 10);

    EndDrawing();
}