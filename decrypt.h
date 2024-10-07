#ifndef DECRYPT_H
#define DECRYPT_H

#include <bitset>
#include <vector>
#include <string>
#include <bitset>


// Function prototypes
void check_args(int argc, char* argv[], bool debug);
std::bitset<8> key_mix(const std::bitset<8>& byte, const std::bitset<8>& key);
std::vector<unsigned char> get_decrypt_string(bool debug);
std::vector<unsigned char> DES_decrypt(const std::vector<unsigned char>& encrypted_vector, const std::bitset<10>& key, bool debug, std::vector<int>& nums);
void keygen(const std::bitset<10>& key, std::bitset<8>& key1, std::bitset<8>& key2, bool debug);
std::bitset<8> fiestal(const std::bitset<8>& byte, const std::bitset<8>& key, bool debug);
std::bitset<8> expand(std::bitset<4> half);
std::bitset<4> sbox_sub(std::bitset<8> byte, bool debug);

#endif // DECRYPT_H
