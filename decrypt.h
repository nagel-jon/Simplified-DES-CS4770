#ifndef DECRYPT_H
#define DECRYPT_H

#include <bitset>
#include <vector>
#include <string>

// Function prototypes
void check_args(int argc, char* argv[], bool debug);
std::vector<unsigned char> get_decrypt_string(const std::string& filename, bool debug);
void DES_decrypt(const std::vector<unsigned char>& encrypted_vector, const std::bitset<10>& key, bool debug);
void keygen(const std::bitset<10>& key, std::bitset<8>& key1, std::bitset<8>& key2, bool debug);

#endif // DECRYPT_H
