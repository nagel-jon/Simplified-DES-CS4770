#include "decrypt.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>

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


    //Cast Key to bitset
    unsigned int hexkey = static_cast<unsigned int>(stoi(argv[1], nullptr, 16));
    bitset<10> key(hexkey & 0x3FF);



    // Decrypt the string
    vector<unsigned char> decrypted_vector;
    vector<int> nums;
    decrypted_vector = DES_decrypt(encrypted_vector, key, debug, nums);

    //Output decrypted vector
    for (unsigned char byte : decrypted_vector) {
        //cout << byte;
        cout << byte;
    }
    cout << endl;
    ofstream outfile("nums.txt");
    if (!outfile) {
        cerr << "Error opening file nums.txt for writing" << endl;
        return 1;
    }

    for (unsigned char byte : decrypted_vector) {
        //cout << byte;
        cout << byte << " ";
    }

    outfile.close();


    return 0;
}
