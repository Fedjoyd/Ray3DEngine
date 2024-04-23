// Ray3DEngine.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <string>

#ifdef _DEBUG
#include <iostream>
#endif // DEBUG

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "Ray3DEngine");

    std::cout << "Hello World!\n";

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

    bool ShouldNotQuit = true;

    DisableCursor();

    // Main game loop
    while (ShouldNotQuit)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        if (WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))
            ShouldNotQuit = false;

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

        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

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
        CameraTargetStr += ")";

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText(CameraTargetStr.c_str(), 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
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
