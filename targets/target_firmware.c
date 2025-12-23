#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

// The "Secret" the fuzzer must find 
// In a real scenario, this could be a calculated checksum or hash.

const uint8_t MAGIC_HEADER[] = { 0xDE, 0xAD, 0xBE, 0xEF};

int parse_packet(uint8_t *data, size_t len){
    if (len < 4) return 0; // Minimum length check

    // The "Cliff" : A standard fuzzer sees 0 or 1
    // Our Bandit will see "How close were we?"
    if (memcmp(data, MAGIC_HEADER, 4) == 0) {
        // CRASH to signal success
        volatile int *p = NULL; *p = 0;
        return 1; // Indicate success
    }
    return 0; // Indicate failure
}

int main() {
    uint8_t buffer[1024];
    size_t len = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (len > 0) {
        parse_packet(buffer, len);
    }
    return 0;

}