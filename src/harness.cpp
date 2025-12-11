#include "state.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <algorithm> // pour std::max

// Cible
bool ParseFirmwarePacket(const uint8_t *Data, size_t Size);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    
    // 1. Exécution
    bool success = ParseFirmwarePacket(Data, Size);

    // 2. Feedback Loop (Bandit Manchot Non-Stationnaire)
    int m_id = g_State.last_mutator_used_id;
    
    if (m_id >= 0 && m_id < NUM_MUTATORS) {
        // A. Calcul de la Récompense (Rt)
        double reward = 0.0;
        if (success) {
            reward = ALPHA;
            g_State.mutator_success_count[m_id]++;
            
            // Log si c'est une percée (optionnel, pour voir le succès)
            // On peut le commenter si ça spamme trop
            // printf("\n[SMART] Mutator %d passed the check! (+%.1f)\n", m_id, reward);
        }

        // B. Mise à jour du Poids (Wt+1 = Gamma * Wt + Rt)
        double current_weight = g_State.mutator_weights[m_id];
        double new_weight = (current_weight * GAMMA) + reward;

        // C. Borne inférieure (pour éviter que le poids ne tombe à 0 et tue l'exploration)
        if (new_weight < MIN_WEIGHT) new_weight = MIN_WEIGHT;

        g_State.mutator_weights[m_id] = new_weight;
    }

    return 0;
}