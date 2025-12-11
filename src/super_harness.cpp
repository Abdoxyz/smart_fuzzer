#include "super_state.h" // Attention au nom
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <algorithm>

bool ParseFirmwarePacket(const uint8_t *Data, size_t Size);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    
    bool success = ParseFirmwarePacket(Data, Size);
    int m_id = g_State.last_mutator_used_id;
    
    if (m_id >= 0 && m_id < NUM_MUTATORS) {
        double reward = 0.0;
        if (success) {
            reward = ALPHA;
            g_State.mutator_success_count[m_id]++;
        }

        double current_weight = g_State.mutator_weights[m_id];
        double new_weight = (current_weight * GAMMA) + reward;
        if (new_weight < MIN_WEIGHT) new_weight = MIN_WEIGHT;

        g_State.mutator_weights[m_id] = new_weight;
    }
    return 0;
}