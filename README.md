# Smart Firmware Fuzzer : Ordonnancement Adaptatif de Mutations

![Language](https://img.shields.io/badge/Language-C++-blue.svg)
![Engine](https://img.shields.io/badge/Engine-LLVM_LibFuzzer-red.svg)
![Type](https://img.shields.io/badge/Type-Research_PoC-green.svg)

> **Projet de Recherche :** Optimisation du Fuzzing de Firmware par apprentissage par renforcement  pour franchir les barrières structurelles (CRC/Checksums).


## Prérequis

```bash
# Installation des dépendances (Ubuntu/Debian)
sudo apt update
sudo apt install clang llvm python3 python3-pip
pip3 install pandas matplotlib
```

## Compilation

```bash
mkdir -p build corpus

clang++ -g -fsanitize=fuzzer,address \
    src/mutator.cpp \
    src/harness.cpp \
    targets/vuln_firmware.cpp \
    -o build/fuzzer

# Nettoyage des anciens fichiers
rm -rf corpus/* 

# Création d'un paquet Modbus minimal
python3 -c "import sys; sys.stdout.buffer.write(b'\x01\x03\x00\x00\x00\x00')" > corpus

./build/fuzzer corpus/
```
