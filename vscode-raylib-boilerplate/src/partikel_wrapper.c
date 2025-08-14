#define LIBPARTIKEL_IMPLEMENTATION
#include "../vendor/partikel.h"

// Simple C wrapper functions to avoid C++ issues
Emitter* create_emitter(void) {
    // Create a simple white texture for particles
    Image whitePixel = GenImageColor(4, 4, WHITE);
    Texture2D particleTexture = LoadTextureFromImage(whitePixel);
    UnloadImage(whitePixel);
    
    EmitterConfig config = {
        .direction = {1.0f, 0.0f},
        .velocity = {50.0f, 150.0f},
        .directionAngle = {0.0f, 360.0f},
        .velocityAngle = {-10.0f, 10.0f},
        .offset = {0.0f, 10.0f},
        .originAcceleration = {0.0f, 0.0f},
        .burst = {10, 20},
        .capacity = 100,
        .emissionRate = 0,
        .origin = {0.0f, 0.0f},
        .externalAcceleration = {0.0f, 50.0f},
        .startColor = WHITE,
        .endColor = {255, 255, 255, 0},
        .age = {0.5f, 1.5f},
        .blendMode = BLEND_ADDITIVE,
        .texture = particleTexture,
        .particle_Deactivator = Particle_DeactivatorAge
    };
    
    return Emitter_New(config);
}

void set_emitter_origin(Emitter* emitter, Vector2 origin) {
    if (emitter) {
        emitter->config.origin = origin;
    }
}

void burst_particles(Emitter* emitter) {
    if (emitter) {
        Emitter_Burst(emitter);
    }
}

void update_particles(Emitter* emitter, float deltaTime) {
    if (emitter) {
        Emitter_Update(emitter, deltaTime);
    }
}

void draw_particles(Emitter* emitter) {
    if (emitter) {
        Emitter_Draw(emitter);
    }
}

void free_emitter(Emitter* emitter) {
    if (emitter) {
        // Unload the texture first
        UnloadTexture(emitter->config.texture);
        Emitter_Free(emitter);
    }
}