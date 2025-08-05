#include "raylib.h"
#include <print>

int main() {
  std::println("Starting raylib with C++23!");

  // Initialize window
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib + C++23 - Bouncing Ket");

  SetTargetFPS(60);

  // Load the ket image
  Texture2D ketTexture = LoadTexture("assets/ket.png");
  
  // Start position (center)
  float imageX = (screenWidth - ketTexture.width) / 2.0f;
  float imageY = (screenHeight - ketTexture.height) / 2.0f;
  
  // Movement velocity
  float dx = 3.0f;
  float dy = 2.0f;

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    imageX += dx;
    imageY += dy;
    
    // Bounce off edges
    if (imageX <= 0 || imageX >= screenWidth - ketTexture.width) {
      dx = -dx;
    }
    if (imageY <= 0 || imageY >= screenHeight - ketTexture.height) {
      dy = -dy;
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);
    
    // Draw the bouncing ket image
    DrawTexture(ketTexture, (int)imageX, (int)imageY, WHITE);
    
    // Draw text at top
    DrawText("Bouncing Ket!", 
             screenWidth/2 - MeasureText("Bouncing Ket!", 20)/2, 
             20, 
             20, WHITE);
    
    EndDrawing();
  }

  // Cleanup
  UnloadTexture(ketTexture);
  CloseWindow();

  std::println("raylib window closed!");
  return 0;
}