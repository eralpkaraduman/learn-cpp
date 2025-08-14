#include "raylib.h"
#include <print>
#include <random>
#include <vector>

// Particle structure
struct Particle {
  Vector2 position;
  Vector2 velocity;
  float life;
  float maxLife;
  Color color;
};

int main() {
  std::println("Starting raylib with C++23!");

  // Let raylib handle resource paths automatically
  const char *appDir = GetApplicationDirectory();
  std::println("Application directory: {}", appDir);
  std::println("Current working directory: {}", GetWorkingDirectory());

  // Initialize window and audio
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib + C++23 - Bouncing Ket");
  InitAudioDevice();

  SetTargetFPS(60);

  // Load assets relative to the binary location
  const char *texturePath = TextFormat("%s/assets/ket.png", GetApplicationDirectory());
  const char *soundPath = TextFormat("%s/assets/bounce.wav", GetApplicationDirectory());
  
  std::println("Attempting to load texture from: {}", texturePath);
  Texture2D ketTexture = LoadTexture(texturePath);
  std::println("Texture loaded - ID: {}, Width: {}, Height: {}", ketTexture.id,
               ketTexture.width, ketTexture.height);

  std::println("Attempting to load sound from: {}", soundPath);
  Sound bounceSound = LoadSound(soundPath);
  bool soundLoaded = (bounceSound.frameCount > 0);

  std::println("Sound frameCount: {}", bounceSound.frameCount);
  std::println("Sound loaded: {}", soundLoaded);

  if (!soundLoaded) {
    std::println("No bounce.wav found, sound effects disabled");
  } else {
    std::println("Bounce sound loaded successfully!");
  }

  float imageX = 250;
  float imageY = 50;

  // Movement velocity
  float dx = 3.0f;
  float dy = 2.0f;

  // Wobble effect variables
  float wobbleRotation = 0.0f;
  float wobbleScale = 1.0f;
  float wobbleTime = 0.0f;
  bool isWobbling = false;

  // Particle system
  std::vector<Particle> particles;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
  std::uniform_real_distribution<float> speedDist(50.0f, 150.0f);
  std::uniform_real_distribution<float> lifeDist(0.5f, 1.5f);

  // Function to create particle burst
  auto createParticleBurst = [&](Vector2 position, int count) {
    for (int i = 0; i < count; i++) {
      Particle particle;
      particle.position = position;

      float angle = angleDist(gen) * DEG2RAD;
      float speed = speedDist(gen);
      particle.velocity = {cos(angle) * speed, sin(angle) * speed};

      particle.maxLife = lifeDist(gen);
      particle.life = particle.maxLife;
      particle.color = {255, 255, 255, 255}; // White particles

      particles.push_back(particle);
    }
  };

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    imageX += dx;
    imageY += dy;

    // Bounce off edges with sound and particles
    if (imageX <= 0 || imageX >= screenWidth - ketTexture.width) {
      dx = -dx;
      if (soundLoaded) {
        std::println("Playing bounce sound (horizontal)");
        PlaySound(bounceSound);
      }
      // Create particle burst at collision point
      Vector2 burstPos = {imageX <= 0 ? 0.0f : (float)screenWidth,
                          imageY + ketTexture.height / 2.0f};
      createParticleBurst(burstPos, 15);

      // Start wobble effect
      isWobbling = true;
      wobbleTime = 0.0f;
    }
    if (imageY <= 0 || imageY >= screenHeight - ketTexture.height) {
      dy = -dy;
      if (soundLoaded) {
        std::println("Playing bounce sound (vertical)");
        PlaySound(bounceSound);
      }
      // Create particle burst at collision point
      Vector2 burstPos = {imageX + ketTexture.width / 2.0f,
                          imageY <= 0 ? 0.0f : (float)screenHeight};
      createParticleBurst(burstPos, 15);

      // Start wobble effect
      isWobbling = true;
      wobbleTime = 0.0f;
    }

    // Update wobble effect
    float deltaTime = GetFrameTime();
    if (isWobbling) {
      wobbleTime += deltaTime;
      const float wobbleDuration = 0.3f; // 300ms wobble

      if (wobbleTime < wobbleDuration) {
        // Create wobble using sine waves
        float progress = wobbleTime / wobbleDuration;
        float intensity = (1.0f - progress); // Fade out over time

        wobbleRotation =
            sin(wobbleTime * 30.0f) * 15.0f * intensity; // Rotation wobble
        wobbleScale =
            1.0f + sin(wobbleTime * 25.0f) * 0.15f * intensity; // Scale wobble
      } else {
        // End wobble
        isWobbling = false;
        wobbleRotation = 0.0f;
        wobbleScale = 1.0f;
      }
    }

    // Update particles
    for (auto it = particles.begin(); it != particles.end();) {
      it->life -= deltaTime;
      if (it->life <= 0) {
        it = particles.erase(it);
      } else {
        // Update position
        it->position.x += it->velocity.x * deltaTime;
        it->position.y += it->velocity.y * deltaTime;

        // Fade out over time
        float alpha = (it->life / it->maxLife) * 255.0f;
        it->color.a = (unsigned char)alpha;

        // Slow down particles
        it->velocity.x *= 0.98f;
        it->velocity.y *= 0.98f;

        ++it;
      }
    }

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw the bouncing ket image with wobble effect
    Vector2 ketCenter = {imageX + ketTexture.width / 2.0f,
                         imageY + ketTexture.height / 2.0f};
    Rectangle sourceRec = {0, 0, (float)ketTexture.width,
                           (float)ketTexture.height};
    Rectangle destRec = {ketCenter.x, ketCenter.y,
                         ketTexture.width * wobbleScale,
                         ketTexture.height * wobbleScale};

    DrawTexturePro(ketTexture, sourceRec, destRec,
                   {destRec.width / 2.0f, destRec.height / 2.0f},
                   wobbleRotation, WHITE);

    // Draw particles
    for (const auto &particle : particles) {
      DrawCircleV(particle.position, 2.0f, particle.color);
    }

    // Draw text at top
    DrawText("Bouncing Ket with Particles!",
             screenWidth / 2 -
                 MeasureText("Bouncing Ket with Particles!", 20) / 2,
             20, 20, WHITE);

    // Draw particle count for debugging
    DrawText(TextFormat("Particles: %d", (int)particles.size()), 10, 50, 16,
             WHITE);

    EndDrawing();
  }

  // Cleanup
  if (soundLoaded) {
    UnloadSound(bounceSound);
  }
  UnloadTexture(ketTexture);
  CloseAudioDevice();
  CloseWindow();

  std::println("raylib window closed!");
  return 0;
}