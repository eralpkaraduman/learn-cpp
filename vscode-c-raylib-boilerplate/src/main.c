#include "raylib.h"
#define LIBPARTIKEL_IMPLEMENTATION
#include "../vendor/partikel.h"
#include "../vendor/reasings.h"

// Constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TARGET_FPS 60
#define KET_SPEED_X 3.0f
#define KET_SPEED_Y 2.0f
#define BOUNCE_DECAY_RATE 1.2f
#define ROTATION_MULTIPLIER 15.0f

Texture2D createParticleTexture() {
    Image whitePixel = GenImageColor(4, 4, WHITE);
    Texture2D particleTexture = LoadTextureFromImage(whitePixel);
    UnloadImage(whitePixel);
    return particleTexture;
}

Emitter* createParticleEmitter(Texture2D particleTexture) {
    EmitterConfig cfg = {0};
    cfg.direction = (Vector2){1.0f, 0.0f};
    cfg.velocity = (FloatRange){50.0f, 150.0f};
    cfg.directionAngle = (FloatRange){0.0f, 360.0f};
    cfg.velocityAngle = (FloatRange){-10.0f, 10.0f};
    cfg.offset = (FloatRange){0.0f, 10.0f};
    cfg.originAcceleration = (FloatRange){0.0f, 0.0f};
    cfg.burst = (IntRange){10, 20};
    cfg.capacity = 100;
    cfg.emissionRate = 0;
    cfg.origin = (Vector2){0.0f, 0.0f};
    cfg.externalAcceleration = (Vector2){0.0f, 50.0f};
    cfg.startColor = WHITE;
    cfg.endColor = (Color){255, 255, 255, 0};
    cfg.age = (FloatRange){0.5f, 1.5f};
    cfg.blendMode = BLEND_ADDITIVE;
    cfg.texture = particleTexture;
    cfg.particle_Deactivator = Particle_DeactivatorAge;
    
    return Emitter_New(cfg);
}

void handleBounce(Emitter *emitter, Vector2 burstPos, bool soundLoaded, Sound bounceSound, float *bounce, const char *direction) {
    if (soundLoaded) {
        TraceLog(LOG_DEBUG, "Playing bounce sound (%s)", direction);
        PlaySound(bounceSound);
    }
    emitter->config.origin = burstPos;
    Emitter_Burst(emitter);
    *bounce = 1.0f;
}

int main() {
  TraceLog(LOG_INFO, "Starting raylib with C!");

  // Let raylib handle resource paths automatically
  const char *appDir = GetApplicationDirectory();
  TraceLog(LOG_INFO, "Application directory: %s", appDir);
  TraceLog(LOG_INFO, "Current working directory: %s", GetWorkingDirectory());

  // Initialize window and audio
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib + C - Bouncing Ket");
  InitAudioDevice();

  SetTargetFPS(TARGET_FPS);

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
  float dx = KET_SPEED_X;
  float dy = KET_SPEED_Y;

  // Bounce value (1.0f → 0.0f)
  float bounce = 0.0f;

  Rectangle ketRect = {250.0f, 50.0f, rectWidth, rectHeight};
  Vector2 ketCenterOffset = {(float)ketTexture.width / 2.0f,
                             (float)ketTexture.height / 2.0f};
  Rectangle ketSourceRect = {0, 0, (float)ketTexture.width,
                             (float)ketTexture.height};

  // Create particle system
  Texture2D particleTexture = createParticleTexture();
  Emitter *emitter = createParticleEmitter(particleTexture);
  if (!emitter) {
    TraceLog(LOG_ERROR, "Failed to create particle emitter");
    CloseWindow();
    return -1;
  }

  // Main game loop
  while (!WindowShouldClose()) {
    // Update collision rectangle position
    ketRect.x += dx;
    ketRect.y += dy;

    // Bounce off edges with sound and particles
    if (ketRect.x <= 0 || ketRect.x >= SCREEN_WIDTH - ketRect.width) {
      dx = -dx;
      Vector2 burstPos = {ketRect.x <= 0 ? 0.0f : (float)SCREEN_WIDTH,
                          ketRect.y + ketRect.height / 2.0f};
      handleBounce(emitter, burstPos, soundLoaded, bounceSound, &bounce, "horizontal");
    }
    if (ketRect.y <= 0 || ketRect.y >= SCREEN_HEIGHT - ketRect.height) {
      dy = -dy;
      Vector2 burstPos = {ketRect.x + ketRect.width / 2.0f,
                          ketRect.y <= 0 ? 0.0f : (float)SCREEN_HEIGHT};
      handleBounce(emitter, burstPos, soundLoaded, bounceSound, &bounce, "vertical");
    }

    // Update bounce (decay from 1.0f → 0.0f)
    float deltaTime = GetFrameTime();
    if (bounce > 0.0f) {
      bounce -= deltaTime * BOUNCE_DECAY_RATE;
      if (bounce < 0.0f) {
        bounce = 0.0f;
      }
    }

    // Update particles
    Emitter_Update(emitter, deltaTime);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    float easedBounce = EaseElasticOut(1.0f - bounce, 0.0f, 1.0f, 1.0f);
    float rotation = (1 - easedBounce) * ROTATION_MULTIPLIER;

    Rectangle destRect = {.x = ketRect.x + ketCenterOffset.x,
                          .y = ketRect.y + ketCenterOffset.y,
                          .width = ketRect.width,
                          .height = ketRect.height};
    DrawTexturePro(ketTexture, ketSourceRect, destRect, ketCenterOffset,
                   rotation, WHITE);

    // Draw particles
    Emitter_Draw(emitter);

    EndDrawing();
  }

  // Cleanup
  UnloadTexture(particleTexture);
  Emitter_Free(emitter);
  if (soundLoaded) {
    UnloadSound(bounceSound);
  }
  UnloadTexture(ketTexture);
  CloseAudioDevice();
  CloseWindow();

  TraceLog(LOG_INFO, "raylib window closed!");
  return 0;
}