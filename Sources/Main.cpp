// Ray3DEngine.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <string>

#ifdef _CONSOLE
#include <iostream>
#endif // CONSOLE

#include "Define.h"

#include "raylib.h"
#include "raymath.h"

#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"
#include "rlImGui.h"
#endif // _EDITOR

#include "Core/Application.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Main.cpp"

class TestRessource : public Ressources::IRessource
{
public:
    TestRessource(const int64_t p_UUID) : IRessource(p_UUID) {}
    ~TestRessource() {}

    REGISTER_RESSOURCE(TestRessource)

private:
};
class TestRessource2 : public Ressources::IRessource
{
public:
    TestRessource2(const int64_t p_UUID) : IRessource(p_UUID) {}
    ~TestRessource2() {}

    REGISTER_RESSOURCE(TestRessource2)

private:
};

class TestComponent : public Components::IComponent
{
public:
    TestComponent() {}
    ~TestComponent() {}

    REGISTER_COMPONENT(TestComponent, Components::COMPONENT_TYPE_NOTHING)

#ifdef _EDITOR
        void ShowEditorControl(const unsigned int p_indexComponent) override
    {
        IComponent::ShowEditorControl(p_indexComponent);

        static int64_t TestComponentDnDID = 0L;

        ImGui::Text("TestRessource :");
        ImGui::SameLine();
        ImGui::InputText(("##TestRessourceOfTestComponent_" + std::to_string(p_indexComponent)).c_str(), &m_TestRessourceName, ImGuiInputTextFlags_ReadOnly);
        if (Ressources::RessourcesManager::RessourceDnDTarget(&TestComponentDnDID))
            if (Core::Application::GetRessourcesManager().TryGetAndLoadRessource(TestComponentDnDID, &m_TestRessource))
            {
                m_TestRessourceName = m_TestRessource->GetName() + " (" + std::to_string(m_TestRessource->GetUUID()) + ")";
                TestComponentDnDID = 0L;
            }
        ImGui::SameLine();
        if (ImGui::Button(("X##TestRessourceOfTestComponent_" + std::to_string(p_indexComponent)).c_str()))
        {
            m_TestRessource = nullptr;
            m_TestRessourceName = "Empty";
        }
    }
#endif // _EDITOR

private:
    std::shared_ptr<TestRessource> m_TestRessource = nullptr;

#ifdef _EDITOR
    std::string m_TestRessourceName = "Empty";
#endif // _EDITOR
};

#ifdef _CONSOLE

class CmdLogger : public Debug::ILogger
{
public:
    CmdLogger() {}
    ~CmdLogger() {}

    void Print(const std::string& p_str) override { std::cout << p_str << "\n"; }
    void Clear() override {}
};

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

    CmdLogger CurrentLogger = CmdLogger();

    //std::cout << "old : 897437200, new : " << typeid(Components::Component).hash_code() << std::endl;
#else
int WinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
#endif
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Ray3DEngine");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
#ifdef _EDITOR
    rlImGuiSetup(true);
#endif // _EDITOR

    Core::Application::Initialize();
#ifdef _CONSOLE
    Core::Application::SetLogger(&CurrentLogger);
#endif

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    std::string CameraTargetStr = "";

    //--------------------------------------------------------------------------------------

    R3DE_DEBUG("Test -> %d", 6621565);
    R3DE_INFO("Test");
    R3DE_WARNING("Test");
    R3DE_ERROR("Test");
    R3DE_FATAL("Test");

    REGISTER_COMPONENT_CREATOR(TestComponent);
    REGISTER_COMPONENT_CREATOR(Components::Camera);

    REGISTER_RESSOURCE_CREATOR(TestRessource);
    REGISTER_RESSOURCE_CREATOR(TestRessource2);

    // Main game loop
    while (!Core::Application::ShouldExit())    // Detect window close button
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        /*if (IsKeyPressed(KEY_ESCAPE))
            EnableCursor();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            DisableCursor();

        if (IsKeyDown(KEY_UP))
            cubePosition.z += GetFrameTime();
        if (IsKeyDown(KEY_DOWN))
            cubePosition.z -= GetFrameTime();
        if (IsKeyDown(KEY_RIGHT))
            cubePosition.x += GetFrameTime();
        if (IsKeyDown(KEY_LEFT))
            cubePosition.x -= GetFrameTime();
        if (IsKeyDown(KEY_KP_ADD))
            cubePosition.y += GetFrameTime();
        if (IsKeyDown(KEY_KP_SUBTRACT))
            cubePosition.y -= GetFrameTime();

        UpdateCamera(&camera, CAMERA_FREE);

        CameraTargetStr = "X = ";
        CameraTargetStr += std::to_string(camera.target.x - camera.position.x);
        CameraTargetStr += "(";
        CameraTargetStr += std::to_string(camera.target.x);
        CameraTargetStr += "), Y = ";
        CameraTargetStr += std::to_string(camera.target.y - camera.position.y);
        CameraTargetStr += "(";
        CameraTargetStr += std::to_string(camera.target.y);
        CameraTargetStr += "), Z = ";
        CameraTargetStr += std::to_string(camera.target.z - camera.position.z);
        CameraTargetStr += "(";
        CameraTargetStr += std::to_string(camera.target.z);
        CameraTargetStr += "), DistHeadTarget = ";
        CameraTargetStr += std::to_string(Vector3Length(Vector3Subtract(camera.target, camera.position)));/**/

        Core::Application::Update();

        // Draw
        //----------------------------------------------------------------------------------
#ifdef _EDITOR
        if (Core::Application::FullscreenGame())
        {
            BeginDrawing();
            ClearBackground(Core::Application::BackgroundColor());

            Core::Application::Draw();

            Core::Application::EditorWindows();
        }
        else
        {
            BeginTextureMode(Core::Application::GameRenderTexture());
            ClearBackground(Core::Application::BackgroundColor());

            Core::Application::Draw();

            EndTextureMode();

            // ---- Start editor ----

            BeginDrawing();
            ClearBackground(BLACK);

            Core::Application::EditorWindows();
        }
#else
        BeginDrawing();
        ClearBackground(Core::Application::BackgroundColor());

        Core::Application::Draw();
#endif // _EDITOR

        EndDrawing();
        //----------------------------------------------------------------------------------
    }


#ifdef _EDITOR
    rlImGuiShutdown();
#endif // _EDITOR
    CloseWindow();

    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
