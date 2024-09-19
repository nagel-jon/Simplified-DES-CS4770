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
    if (key.size() != 4) {
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

    keygen(key, k1, k2, debug);

    // Do work here
}

void keygen(const bitset<10>& key, bitset<8>& key1, bitset<8>& key2, bool debug) {
    if (debug) {
        cout << "Keygen" << endl;
        cout << "Initial Key: " << key << endl;
        cout << "Performing P10 Permutation" << endl;
    }
    // Do P10 permutation
    bitset<10> key_permuted = 0b0000000000;
    key_permuted[0] = key[2];
    key_permuted[1] = key[4];
    key_permuted[2] = key[1];
    key_permuted[3] = key[6];
    key_permuted[4] = key[3];
    key_permuted[5] = key[9];
    key_permuted[6] = key[0];
    key_permuted[7] = key[8];
    key_permuted[8] = key[7];
    key_permuted[9] = key[5];

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
    key1[0] = keyhalf_1[2];
    key1[1] = keyhalf_1[4];
    key1[2] = keyhalf_1[1];
    key1[3] = keyhalf_1[3];
    key1[4] = keyhalf_2[2];
    key1[5] = keyhalf_2[4];
    key1[6] = keyhalf_2[1];
    key1[7] = keyhalf_2[3];

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
    key2[0] = keyhalf_1[2];
    key2[1] = keyhalf_1[4];
    key2[2] = keyhalf_1[1];
    key2[3] = keyhalf_1[3];
    key2[4] = keyhalf_2[2];
    key2[5] = keyhalf_2[4];
    key2[6] = keyhalf_2[1];
    key2[7] = keyhalf_2[3];

    if (debug) {
        cout << "LS (2 of 2) Performed" << endl;
        cout << "Key Half 1: " << keyhalf_1 << endl;
        cout << "Key Half 2: " << keyhalf_2 << endl;
        cout << "Key 2: " << key2 << endl;
    }
}
