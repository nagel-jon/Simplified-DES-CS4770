#include "decrypt.h"
#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    // Global Variables
    bool debug = true;
    vector<unsigned char> encrypted_vector;

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

    //Cast Key to bitset
    unsigned int hexkey = static_cast<unsigned int>(stoi(argv[1], nullptr, 16));
    bitset<10> key(hexkey & 0x3FF);

    // Decrypt the string
    vector<unsigned char> decrypted_vector;
    decrypted_vector = DES_decrypt(encrypted_vector, key, debug);

    //Output decrypted vector
    for (unsigned char byte : decrypted_vector) {
        //cout << byte;
        cout << byte;
    }
    cout << endl;



    return 0;
}
