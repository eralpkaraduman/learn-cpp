#include <print>
#include "raylib.h"

int main() {
    std::println("Starting raylib with C++23!");
    
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib + C++23 - Basic Window");
    
    SetTargetFPS(60);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello raylib with C++23!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    
    // Cleanup
    CloseWindow();
    
    std::println("raylib window closed!");
    return 0;
}