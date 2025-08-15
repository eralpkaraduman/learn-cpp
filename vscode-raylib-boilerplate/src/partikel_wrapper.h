#pragma once
#include "raylib.h"

// Forward declaration for the C struct
struct Emitter;

// C wrapper functions
extern "C" {
    struct Emitter* create_emitter(void);
    void set_emitter_origin(struct Emitter* emitter, Vector2 origin);
    void burst_particles(struct Emitter* emitter);
    void update_particles(struct Emitter* emitter, float deltaTime);
    void draw_particles(struct Emitter* emitter);
    void free_emitter(struct Emitter* emitter);
}

// Simple C++ wrapper class
class SimpleParticleSystem {
private:
    struct Emitter* emitter;

public:
    SimpleParticleSystem() {
        emitter = create_emitter();
    }
    
    ~SimpleParticleSystem() {
        free_emitter(emitter);
    }
    
    void burst(Vector2 position) {
        set_emitter_origin(emitter, position);
        burst_particles(emitter);
    }
    
    void update(float deltaTime) {
        update_particles(emitter, deltaTime);
    }
    
    void draw() {
        draw_particles(emitter);
    }
};