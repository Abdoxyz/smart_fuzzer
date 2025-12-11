#ifndef SUPER_STATE_H
#define SUPER_STATE_H

#include <vector>
#include <random>

// --- CONFIGURATION DU MODELE UNIVERSEL ---
static const int NUM_MUTATORS = 7; // On augmente la taille du menu !
static const double ALPHA = 10.0;  // Récompense
static const double GAMMA = 0.999;  // Oubli
static const double MIN_WEIGHT = 0.1;

struct GlobalState {
    double mutator_weights[NUM_MUTATORS];
    long mutator_usage_count[NUM_MUTATORS];
    long mutator_success_count[NUM_MUTATORS];
    int last_mutator_used_id;

    GlobalState() {
        for(int i=0; i<NUM_MUTATORS; i++) {
            mutator_weights[i] = 1.0; 
            mutator_usage_count[i] = 0;
            mutator_success_count[i] = 0;
        }
        last_mutator_used_id = -1;
    }
};

extern GlobalState g_State;
extern std::mt19937 g_Rng;

#endif