#include "raylib.h"
#include "reasings.h"
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
  TraceLog(LOG_INFO, "Starting raylib with C++23!");

  // Let raylib handle resource paths automatically
  const char *appDir = GetApplicationDirectory();
  TraceLog(LOG_INFO, "Application directory: %s", appDir);
  TraceLog(LOG_INFO, "Current working directory: %s", GetWorkingDirectory());

  // Initialize window and audio
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib + C++23 - Bouncing Ket");
  InitAudioDevice();

  SetTargetFPS(60);

  // Load assets relative to the binary location
  const char *texturePath =
      TextFormat("%s/assets/ket.png", GetApplicationDirectory());
  const char *soundPath =
      TextFormat("%s/assets/bounce.wav", GetApplicationDirectory());

  TraceLog(LOG_INFO, "Attempting to load texture from: %s", texturePath);
  Texture2D ketTexture = LoadTexture(texturePath);
  TraceLog(LOG_INFO, "Texture loaded - ID: %u, Width: %d, Height: %d",
           ketTexture.id, ketTexture.width, ketTexture.height);

  // Define fixed rectangle dimensions based on texture
  float rectWidth = (float)ketTexture.width;
  float rectHeight = (float)ketTexture.height;

  TraceLog(LOG_INFO, "Attempting to load sound from: %s", soundPath);
  Sound bounceSound = LoadSound(soundPath);
  bool soundLoaded = (bounceSound.frameCount > 0);

  TraceLog(LOG_INFO, "Sound frameCount: %u", bounceSound.frameCount);
  TraceLog(LOG_INFO, "Sound loaded: %s", soundLoaded ? "true" : "false");

  if (!soundLoaded) {
    TraceLog(LOG_WARNING, "No bounce.wav found, sound effects disabled");
  } else {
    TraceLog(LOG_INFO, "Bounce sound loaded successfully!");
  }

  // Movement velocity
  float dx = 3.0f;
  float dy = 2.0f;

  // Bounce value (1.0f → 0.0f)
  float bounce = 0.0f;

  Rectangle imageRect = {250.0f, 50.0f, rectWidth, rectHeight};
  Vector2 imageCenterOffset = {(float)ketTexture.width / 2.0f,
                               (float)ketTexture.height / 2.0f};
  Rectangle imageSourceRect = {0, 0, (float)ketTexture.width,
                               (float)ketTexture.height};

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
    // Update collision rectangle position
    imageRect.x += dx;
    imageRect.y += dy;

    // Bounce off edges with sound and particles
    if (imageRect.x <= 0 || imageRect.x >= screenWidth - imageRect.width) {
      dx = -dx;
      if (soundLoaded) {
        TraceLog(LOG_DEBUG, "Playing bounce sound (horizontal)");
        PlaySound(bounceSound);
      }
      // Create particle burst at collision point
      Vector2 burstPos = {imageRect.x <= 0 ? 0.0f : (float)screenWidth,
                          imageRect.y + imageRect.height / 2.0f};
      createParticleBurst(burstPos, 15);

      // Trigger bounce
      bounce = 1.0f;
    }
    if (imageRect.y <= 0 || imageRect.y >= screenHeight - imageRect.height) {
      dy = -dy;
      if (soundLoaded) {
        TraceLog(LOG_DEBUG, "Playing bounce sound (vertical)");
        PlaySound(bounceSound);
      }
      // Create particle burst at collision point
      Vector2 burstPos = {imageRect.x + imageRect.width / 2.0f,
                          imageRect.y <= 0 ? 0.0f : (float)screenHeight};
      createParticleBurst(burstPos, 15);

      // Trigger bounce
      bounce = 1.0f;
    }

    // Update bounce (decay from 1.0f → 0.0f)
    float deltaTime = GetFrameTime();
    if (bounce > 0.0f) {
      bounce -= deltaTime * 1.2f; // Decay rate
      if (bounce < 0.0f) {
        bounce = 0.0f;
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

    float easedBounce = EaseElasticOut(1.0f - bounce, 0.0f, 1.0f, 1.0f);
    float rotation = (1 - easedBounce) * 15.0f; // Rotate based on bounce

    Rectangle destRect = {.x = imageRect.x + imageCenterOffset.x,
                          .y = imageRect.y + imageCenterOffset.y,
                          .width = imageRect.width,
                          .height = imageRect.height};
    DrawTexturePro(ketTexture, imageSourceRect, destRect, imageCenterOffset,
                   rotation, WHITE);
    EndDrawing();
  }

  // Cleanup
  if (soundLoaded) {
    UnloadSound(bounceSound);
  }
  UnloadTexture(ketTexture);
  CloseAudioDevice();
  CloseWindow();

  TraceLog(LOG_INFO, "raylib window closed!");
  return 0;
}