#include "decrypt.h"
#include <iostream>
#include <fstream>

using namespace std;

void check_args(int argc, char* argv[], bool debug) {
    if (debug) {
        cout << "Checking Args" << endl;
        for (int i = 0; i < argc; i++) {
            cout << "Arg " << i << ": " << argv[i] << endl;
        }
    }
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <key>" << endl;
        exit(1);
    }

    string key = argv[1];
    if (key.size() != 5) {
        cerr << "Key must be 4 characters long" << endl;
        exit(1);
    }
}

vector<unsigned char> get_decrypt_string(const string& filename, bool debug) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: unable to open input file: " << filename << endl;
        exit(1);
    }
    vector<unsigned char> fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return fileContent;
}

void DES_decrypt(const vector<unsigned char>& encrypted_vector, const bitset<10>& key, bool debug) {
    bitset<8> k1 = 0b00000000;
    bitset<8> k2 = 0b00000000;
    if (debug) {
        cout << "Key: " << key << endl;
    }

    //Generate keys
    keygen(key, k1, k2, debug);
    if (debug) {
        cout << "Keygen Complete" << endl;
        cout << "Key 1: " << k1 << endl;
        cout << "Key 1 (int): " << k1.to_ulong() << endl;
        cout << "Key 2: " << k2 << endl;
        cout << "Key 2 (int): " << k2.to_ulong() << endl;
    }

    //Do Initial Permutation (IP)
    if (debug) {
        cout << "Starting Loop Over Encrypted bytes" << endl;
    }


    int i = 0;  //print which byte    
    for (unsigned char byte : encrypted_vector) {
        bitset<8> encrypted_byte = byte;
        if (debug) {
            // cout << "Byte #" << i << " ";
            // cout << static_cast<int>(encrypted_byte.to_ulong()) << endl;
            //cout << "Encrypted Byte: " << encrypted_byte << endl;
            i++;

        }

        // Perform Initial Permutation
        bitset<8> ip_byte = 0b00000000;
        ip_byte[7] = encrypted_byte[6];
        ip_byte[6] = encrypted_byte[2];
        ip_byte[5] = encrypted_byte[5];
        ip_byte[4] = encrypted_byte[7];
        ip_byte[3] = encrypted_byte[4];
        ip_byte[2] = encrypted_byte[0];
        ip_byte[1] = encrypted_byte[3];
        ip_byte[0] = encrypted_byte[1];

        if (debug) {
            //cout << "Initial Permutation: " << ip_byte << endl;
        }

        // Perform Fiestal Function (round 1)
        bitset<8> fiestal_byte = fiestal(ip_byte, k2, debug);

    }

    //Loop through each byte in the encrypted vector
    //During loop need to perform the following operations:
    //1. Perform Initial Permutation
    //2. Perform Fiestal Function (round 1)
        //2a.  Perform Expansion Permutation
        //3b.  Perform Key Mixing
        //3c.  Perform S-Box Substitution
        //3d.  Perform Permutation
    //3. Perform Nibble Swap
    //4. Perform Fiestal Function (round 2)
    //5. Perform Inverse Initial Permutation
    //6. Output decrypted byte


}

void keygen(const bitset<10>& key, bitset<8>& key1, bitset<8>& key2, bool debug) {
    if (debug) {
        cout << "Keygen" << endl;
        cout << "Initial Key: " << key << endl;
        cout << "Initial Key (int): " << key.to_ulong() << endl;
        cout << "Performing P10 Permutation" << endl;
    }
    // Do P10 permutation
    bitset<10> key_permuted = 0b0000000000;
    key_permuted[9] = key[7];
    key_permuted[8] = key[5];
    key_permuted[7] = key[8];
    key_permuted[6] = key[3];
    key_permuted[5] = key[6];
    key_permuted[4] = key[0];
    key_permuted[3] = key[9];
    key_permuted[2] = key[1];
    key_permuted[1] = key[2];
    key_permuted[0] = key[4];


    if (debug) {
        cout << "Permuted Key: " << key_permuted << endl;
        cout << "Splitting into two 5 bit halves" << endl;
    }

    // Split key into two 5-bit halves
    bitset<5> keyhalf_1 = 0b00000;
    bitset<5> keyhalf_2 = 0b00000;
    keyhalf_2[0] = key_permuted[0];
    keyhalf_2[1] = key_permuted[1];
    keyhalf_2[2] = key_permuted[2];
    keyhalf_2[3] = key_permuted[3];
    keyhalf_2[4] = key_permuted[4];
    keyhalf_1[0] = key_permuted[5];
    keyhalf_1[1] = key_permuted[6];
    keyhalf_1[2] = key_permuted[7];
    keyhalf_1[3] = key_permuted[8];
    keyhalf_1[4] = key_permuted[9];

    if (debug) {
        cout << "Key Half 1: " << keyhalf_1 << endl;
        cout << "Key Half 2: " << keyhalf_2 << endl;
        cout << "Performing Left Shifts" << endl;
    }

    bitset<1> temp = 0b0;
    // Perform left shifts
    temp = bitset<1>(keyhalf_1[4]);
    keyhalf_1 <<= 1;
    keyhalf_1[0] = temp[0];

    temp = bitset<1>(keyhalf_2[4]);
    keyhalf_2 <<= 1;
    keyhalf_2[0] = temp[0];

    if (debug) {
        cout << "Left Shift (1) Performed" << endl;
        cout << "Key Half 1: " << keyhalf_1 << endl;
        cout << "Key Half 2: " << keyhalf_2 << endl;
        cout << "Performing P8 Permutation" << endl;
    }

    // Do P8 Permutation to get K1
    key1[7] = keyhalf_2[4];
    key1[6] = keyhalf_1[2];
    key1[5] = keyhalf_2[3];
    key1[4] = keyhalf_1[3];
    key1[3] = keyhalf_2[2];
    key1[2] = keyhalf_1[0];
    key1[1] = keyhalf_2[0];
    key1[0] = keyhalf_2[1];

    if (debug) {
        cout << "Key 1: " << key1 << endl;
    }

    // Perform second left shift
    if (debug) {
        cout << "Performing Double Left Shift" << endl;
    }

    temp = bitset<1>(keyhalf_1[4]);
    keyhalf_1 <<= 1;
    keyhalf_1[0] = temp[0];

    temp = bitset<1>(keyhalf_2[4]);
    keyhalf_2 <<= 1;
    keyhalf_2[0] = temp[0];

    if (debug) {
        cout << "Left Shift (1 of 2) Performed" << endl;
        cout << "Key Half 1: " << keyhalf_1 << endl;
        cout << "Key Half 2: " << keyhalf_2 << endl;
    }

    // Do another left shift
    temp = bitset<1>(keyhalf_1[4]);
    keyhalf_1 <<= 1;
    keyhalf_1[0] = temp[0];

    temp = bitset<1>(keyhalf_2[4]);
    keyhalf_2 <<= 1;
    keyhalf_2[0] = temp[0];

    // Do P8 Permutation to get K2
    key2[7] = keyhalf_2[4];
    key2[6] = keyhalf_1[2];
    key2[5] = keyhalf_2[3];
    key2[4] = keyhalf_1[3];
    key2[3] = keyhalf_2[2];
    key2[2] = keyhalf_1[0];
    key2[1] = keyhalf_2[0];
    key2[0] = keyhalf_2[1];


    if (debug) {
        cout << "LS (2 of 2) Performed" << endl;
        cout << "Key Half 1: " << keyhalf_1 << endl;
        cout << "Key Half 2: " << keyhalf_2 << endl;
        cout << "Key 2: " << key2 << endl;
    }
}


bitset<8> fiestal(const std::bitset<8>& byte, const std::bitset<8>& key, bool debug) {
    if (debug) {
        cout << "Fiestal Function" << endl;
        cout << "Byte: " << byte << endl;
        cout << "Key: " << key << endl;
    }

    // Perform Expansion Permutation
    bitset<8> expanded_byte = 0b00000000;
    expanded_byte[7] = byte[4];
    expanded_byte[6] = byte[7];
    expanded_byte[5] = byte[6];
    expanded_byte[4] = byte[5];
    expanded_byte[3] = byte[6];
    expanded_byte[2] = byte[5];
    expanded_byte[1] = byte[4];
    expanded_byte[0] = byte[7];

    if (debug)
    {
        cout << "Expanded Byte: " << expanded_byte << endl;
    }



    // Perform Key Mixing
    bitset<8> mixed_byte = expanded_byte ^ key;

    if (debug) {
        cout << "Mixed Byte: " << mixed_byte << endl;
    }

    // Perform S-Box Substitution
    //idk how this works yet

    // Perform Permutation



}
