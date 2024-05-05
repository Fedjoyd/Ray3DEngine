#include "Core/Application.h"

#include "Debug/Assert.h"

#define R3DE_CURRENT_FILE "Application.cpp"

Core::Application Core::Application::m_singleton;

void Core::Application::Initialize()
{
	Debug::Log::SetInstance(&(m_singleton.m_log));

    m_singleton.m_GameTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

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

    if (IsWindowResized())
    {
        UnloadRenderTexture(m_singleton.m_GameTexture);
        m_singleton.m_GameTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    if (IsKeyPressed(KEY_H))
        m_singleton.m_ShowEditorControl = !(m_singleton.m_ShowEditorControl);

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
    BeginMode3D(m_singleton.m_camerasManager.GetCameraData());

    //DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
    //DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
    //DrawSphere(camera.target, 1.f, RED);

    m_singleton.m_gameObjectManager.Draw();

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
            ImGui::MenuItem("Style", NULL, &(m_singleton.m_OpenStyle));
            ImGui::MenuItem("Exit", NULL, &(m_singleton.m_shouldExit));
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * (m_singleton.m_fullscreenGame ? 0.5f : 0.225f), ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Avocado"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Broccoli"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Cucumber"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    if (!(m_singleton.m_fullscreenGame))
    {
        ImGui::BeginChild("ChildM", ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
        rlImGuiImageRenderTextureFit(&(m_singleton.m_GameTexture), true);
        ImGui::EndChild();

        ImGui::SameLine();
    }

    ImGui::BeginChild("ChildR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("GameObject Inspector"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ressources Inspector"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::End();

    ImGui::ShowDemoWindow();

    if (m_singleton.m_OpenStyle)
    {
        ImGui::Begin("Dear ImGui Style Editor", &(m_singleton.m_OpenStyle));
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    rlImGuiEnd(); // end ImGui Content
}
#endif // _EDITOR