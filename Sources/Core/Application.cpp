#include "Core/Application.h"

#include "Debug/Assert.h"

#define R3DE_CURRENT_FILE "Application.cpp"

Core::Application Core::Application::m_singleton;

void Core::Application::Initialize()
{
	Debug::Log::SetInstance(&(m_singleton.m_log));

#ifdef _EDITOR
    m_singleton.m_GameTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
#endif // _EDITOR

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

            m_singleton.m_CurrentSceneName = nextScene->GetName();

#ifdef _EDITOR
            if (m_singleton.m_run)
                m_singleton.m_gameObjectManager.Start();
#else
            m_singleton.m_gameObjectManager.Start();
#endif // _EDITOR
        }
        else
            R3DE_ERROR("Scene %s does not exist !", m_singleton.m_nextSceneName.c_str());

        m_singleton.m_nextSceneName = "";
        m_singleton.m_queryLoadScene = false;
    }

#ifdef _EDITOR
    if (IsWindowResized())
    {
        UnloadRenderTexture(m_singleton.m_GameTexture);
        m_singleton.m_GameTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    if (IsKeyPressed(KEY_H))
        m_singleton.m_ShowEditorControl = !(m_singleton.m_ShowEditorControl);
#endif // _EDITOR

    m_singleton.m_time.Update();
    m_singleton.GetCamerasManager().Update();

#ifdef _EDITOR
    if (m_singleton.m_run)
        m_singleton.m_gameObjectManager.Update();
    else
        m_singleton.m_gameObjectManager.EditorUpdate();

    if (Time::Fixing() > 0)
    {
        if (m_singleton.m_run)
            m_singleton.m_gameObjectManager.FixedUpdate();
        else
            m_singleton.m_gameObjectManager.EditorFixedUpdate();
    }
#else
    m_singleton.m_gameObjectManager.Update();

    if (Time::Fixing() > 0)
        m_singleton.m_gameObjectManager.FixedUpdate();
#endif // _EDITOR
}

void Core::Application::RenderCamera()
{
    std::vector<Components::Camera*>& lstCamera = m_singleton.m_camerasManager.GetListCamera();

    for (std::vector<Components::Camera*>::iterator CameraIte = lstCamera.begin(); CameraIte != lstCamera.end(); CameraIte++)
    {
        if ((m_singleton.m_camerasManager.TestCurrentCamera(*CameraIte) && !((*CameraIte)->RenderCameraTexture())) || !((*CameraIte)->Enabled()))
            continue;

        BeginTextureMode((*CameraIte)->CameraTexture());
        ClearBackground((*CameraIte)->BackgroundColor());
        BeginMode3D((*CameraIte)->GetCameraData());

        DrawCube(Vector3{ -2.5f, -2.5f, -2.5f }, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(Vector3{ 2.5f, 2.5f, 2.5f }, 2.0f, 2.0f, 2.0f, MAROON);
        DrawSphere(Vector3{ 0.f, 0.f, 0.f }, 1.f, RED);

        m_singleton.m_gameObjectManager.Draw((*CameraIte)->DrawLayer());

        EndMode3D();
        EndTextureMode();
    }
}

void Core::Application::Draw()
{
    ClearBackground(m_singleton.m_camerasManager.GetBackgroundColor());

    BeginMode3D(m_singleton.m_camerasManager.GetCameraData());

    DrawCube(Vector3{ -2.5f, -2.5f, -2.5f }, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires(Vector3{ 2.5f, 2.5f, 2.5f }, 2.0f, 2.0f, 2.0f, MAROON);
    DrawSphere(Vector3{0.f, 0.f, 0.f}, 1.f, RED);

    m_singleton.m_gameObjectManager.Draw(m_singleton.m_camerasManager.GetDrawLayer());

    DrawGrid(10, 1.0f);

    EndMode3D();

    //DrawText(CameraTargetStr.c_str(), 10, 40, 20, DARKGRAY);
    m_singleton.m_gameObjectManager.DrawUI();

    DrawFPS(10, 10);
}

#ifdef _EDITOR
void Core::Application::EditorWindows()
{
    if (!(m_singleton.m_ShowEditorControl))
        return;

    rlImGuiBegin(); // start ImGui Conent

    if (m_singleton.m_fullscreenGame)
        ImGui::Begin("Editor Application", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
    else
    {
        ImGui::Begin("Editor Application", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetWindowPos(viewport->Pos);
        ImGui::SetWindowSize(viewport->Size);
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::MenuItem("Game Fullscreen", NULL, &(m_singleton.m_fullscreenGame));
            if (ImGui::MenuItem("Run", NULL, &(m_singleton.m_run)))
                m_singleton.m_gameObjectManager.Start();
            ImGui::Separator();
            ImGui::MenuItem("Log", NULL, &(m_singleton.m_OpenLog));
            ImGui::MenuItem("Localisations Manager", NULL, &(m_singleton.m_OpenLocalisationsManager));
            ImGui::MenuItem("Style", NULL, &(m_singleton.m_OpenStyle));
            ImGui::Separator();
            ImGui::MenuItem("Exit", NULL, &(m_singleton.m_shouldExit));
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * (m_singleton.m_fullscreenGame ? 0.5f : 0.225f), ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTabBar("LeftTabBar", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Settings"))
        {
            if (ImGui::CollapsingHeader("Cameras Manager"))
                m_singleton.m_camerasManager.ShowEditorControl();
            if (ImGui::CollapsingHeader("Inputs Manager"))
                m_singleton.m_inputsManager.ShowEditorControl();
            if (ImGui::CollapsingHeader("Settings Manager"))
                m_singleton.m_settingsManager.ShowEditorControl();
            if (ImGui::CollapsingHeader("Time"))
                m_singleton.m_time.ShowEditorControl();
            if (ImGui::CollapsingHeader("Components Manager Type"))
                m_singleton.m_componentsManager.ShowEditorControl();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("GameObject List"))
        {
            m_singleton.m_gameObjectManager.ShowEditorControl(m_singleton.m_SelectedItem);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ressources List"))
        {
            m_singleton.m_ressourcesManager.ShowEditorControl(m_singleton.m_SelectedItem);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Scenes"))
        {
            m_singleton.m_ressourcesManager.ShowSceneControl();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    if (!(m_singleton.m_fullscreenGame))
    {
        ImGui::BeginChild("ChildM", ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        rlImGuiImageRenderTextureFit(&(m_singleton.m_GameTexture), true);
        ImGui::EndChild();

        ImGui::SameLine();
    }

    ImGui::BeginChild("ChildR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
    switch (m_singleton.m_SelectedItem.type)
    {
    case Core::TYPE_ITEM_SELECTED::GAMEOBJECT:
        m_singleton.m_gameObjectManager.ShowGameObjectInspector(m_singleton.m_SelectedItem.Data.GameObjectIndex);
        break;
    case Core::TYPE_ITEM_SELECTED::RESSOURCE:
        m_singleton.m_ressourcesManager.ShowRessourceInspector(m_singleton.m_SelectedItem.Data.RessourceUUID);
        break;
    default:
        ImGui::Text("No GameObject or Ressources Selected");
        break;
    }
    ImGui::EndChild();

    ImGui::End();

    // ---- End Main Window ----

    ImGui::ShowDemoWindow();

    if (m_singleton.m_OpenStyle)
    {
        ImGui::Begin("Dear ImGui Style Editor", &(m_singleton.m_OpenStyle));
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    if (m_singleton.m_OpenLocalisationsManager)
    {
        ImGui::Begin("Localisations Window", &(m_singleton.m_OpenLocalisationsManager));
        m_singleton.m_localisationsManager.ShowEditorControl();
        ImGui::End();
    }

    if (m_singleton.m_OpenLog)
    {
        ImGui::Begin("Log Window", &(m_singleton.m_OpenLog));
        m_singleton.m_log.ShowEditorControl();
        ImGui::End();
    }

    rlImGuiEnd(); // end ImGui Content
}
#endif // _EDITOR