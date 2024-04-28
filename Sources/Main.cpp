// Ray3DEngine.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <string>

#ifdef _CONSOLE
#include <iostream>
#endif // CONSOLE

#include "Define.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Core/Application.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Main.cpp"

#ifdef _CONSOLE

class CmdLogger : public Debug::ILogger
{
public:
    CmdLogger() {}
    ~CmdLogger() {}

    void Print(const std::string& p_str) override { std::cout << p_str; }
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
    Core::Application::Initialize();
#ifdef _CONSOLE
    Core::Application::SetLogger(&CurrentLogger);
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Ray3DEngine");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    std::string CameraTargetStr = "";

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    R3DE_DEBUG("Test : %d", 6621565);
    R3DE_INFO("Test");
    R3DE_WARNING("Test");
    R3DE_ERROR("Test");
    R3DE_FATAL("Test");

    DisableCursor();

    // Main game loop
    while (!Core::Application::ShouldExit())    // Detect window close button
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        Core::Application::Update();

        if (IsKeyPressed(KEY_ESCAPE))
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
        CameraTargetStr += std::to_string(Vector3Length(Vector3Subtract(camera.target, camera.position)));

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawSphere(camera.target, 1.f, RED);

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText(CameraTargetStr.c_str(), 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------

        //Core::Application::Draw();

        //----------------------------------------------------------------------------------
    }

    EnableCursor();

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
