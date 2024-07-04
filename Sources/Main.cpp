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

#include "Components/Component/Transform.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Main.cpp"

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

    //--------------------------------------------------------------------------------------

    R3DE_DEBUG("Test -> %d", 6621565);
    R3DE_INFO("Test");
    R3DE_WARNING("Test");
    R3DE_ERROR("Test");
    R3DE_FATAL("Test");

    R3DE_INFO(GET_LOCAL("TEST").c_str());
    R3DE_INFO(GET_LOCAL("TEST_RUN").c_str());
    R3DE_INFO(GET_LOCAL("ULM").c_str());

    // Main game loop
    while (!Core::Application::ShouldExit())    // Detect window close button
    {
        // Update
        //----------------------------------------------------------------------------------

        Core::Application::Update();

        // Draw
        //----------------------------------------------------------------------------------

        Core::Application::RenderCamera();

#ifdef _EDITOR
        if (Core::Application::FullscreenGame())
        {
            BeginDrawing();

            Core::Application::Draw();

            Core::Application::EditorWindows();
        }
        else
        {
            BeginTextureMode(Core::Application::GameRenderTexture());

            Core::Application::Draw();

            EndTextureMode();

            // ---- Start editor ----

            BeginDrawing();
            ClearBackground(BLACK);

            Core::Application::EditorWindows();
        }
#else
        BeginDrawing();

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
