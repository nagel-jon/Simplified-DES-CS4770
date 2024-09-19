#include "decrypt.h"
#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    // Global Variables
    bool debug = true;
    vector<unsigned char> encrypted_vector;
    bitset<10> key = 0b0111101010;

    if (debug) {
        cout << "Starting: " << argv[0] << " in DEBUG mode\n" << endl;
    }

    // Check for correct number, format of args
    check_args(argc, argv, debug);
    // Open input file
    encrypted_vector = get_decrypt_string("decrypt_in 1.txt", debug);

    // Print encrypted string and key if Debugging
    if (debug) {
        // cout << "Encrypted String: " << endl;
        // for (unsigned char byte : encrypted_vector) {
        //     cout << static_cast<int>(byte) << " ";
        // }
        // cout << endl;
    }

    // Decrypt the string
    DES_decrypt(encrypted_vector, key, debug);

    return 0;
}
