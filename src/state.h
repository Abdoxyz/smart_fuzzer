#ifndef STATE_H
#define STATE_H

#include <vector>
#include <random>

// --- CONFIGURATION DU MODELE ---
static const int NUM_MUTATORS = 3;
static const double ALPHA = 10.0;   // Récompense (Gain d'Information)
static const double GAMMA = 0.95;   // Facteur d'Oubli (0 < gamma < 1)
static const double MIN_WEIGHT = 0.1; // Poids minimal pour garder une chance d'exploration

struct GlobalState {
    // Poids des mutateurs
    double mutator_weights[NUM_MUTATORS];
    
    // Stats pour le debug
    long mutator_usage_count[NUM_MUTATORS];
    long mutator_success_count[NUM_MUTATORS];

    // Communication Mutator -> Harness
    int last_mutator_used_id;

    GlobalState() {
        for(int i=0; i<NUM_MUTATORS; i++) {
            mutator_weights[i] = 1.0; // Poids initial
            mutator_usage_count[i] = 0;
            mutator_success_count[i] = 0;
        }
        last_mutator_used_id = -1;
    }
};

// Singleton
extern GlobalState g_State;
extern std::mt19937 g_Rng;

#endif