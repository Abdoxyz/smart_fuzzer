import frida 
import sys
import os

# 1. The JavaScript Payload (Runs inside the target process)
# It intercepts 'memcmp' and calculates the distance.
js_code = """
// Helper: Count set bits (Brian Kernighan's Algorithm)
function countSetBits(n) {
    var count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

// Hook memcmp
Interceptor.attach(Module.findExportByName(null, "memcmp"), {
    onEnter: function (args) {
        var len = args[2].toInt32();
        if (len > 0) {
            // Read the buffers
            var buf1 = Memory.readByteArray(args[0], len);
            var buf2 = Memory.readByteArray(args[1], len);
            
            var b1_view = new Uint8Array(buf1);
            var b2_view = new Uint8Array(buf2);
            
            var total_bits = len * 8;
            var differing_bits = 0;
            
            // Calculate Hamming distance
            for (var i = 0; i < len; i++) {
                var xor_value = b1_view[i] ^ b2_view[i]; // XOR finds differences
                differing_bits += countSetBits(xor_value);
            }
            
            // CONVERT TO REWARD (Similarity)
            // Bandit wants to MAXIMIZE reward, so we convert distance.
            // Reward = Total Bits - differing bits
            var similarity_score = total_bits - differing_bits;
            
            send({
                "type": "reward",
                "score": similarity_score,
                "max_score": total_bits
            });
        }
    }
});
"""   

def on_message(message, data):
    if message['type'] == 'send':
        payload = message['payload']
        if payload['type'] == 'reward':
            score = payload['score']
            max_score = payload['max_score']
            print(f"[BANDIT SIGNAL] Similarity Score: {score}/{max_score}")
    else:
        print(message) 
        
# 2. Spawn the process 
device = frida.get_local_device()
pid = device.spawn(["./target_firmaware"], stdio='pipe')
session = device.attach(pid)

# 3. Inject the logic
script = session.create_script(js_code)
script.on('message', on_message)
script.load()

# 4. Feed it some data (Mocking the Fuzzer Input)
# Let's try sending "DE AD 00 00" (Partial match to "DE AD BE EF")
input_data = b'\xDE\xAD\x00\x00'

# Resume and write to stdin
device.resume(pid)
# Note : Writing to stdin with Frida spawn is tricky in Python alone.
# Usually, you'd write to a file and cat it ,or use a pipe.
# For this audit, assume input flows in.

