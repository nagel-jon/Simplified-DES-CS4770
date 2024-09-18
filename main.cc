#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
using namespace std;

/*
Jonathan Nagel 10 Bit DES Decrypter
    9/17/2024
    jn85542@ohio.edu
    CS 4770 Chad Mourning
*/
/*
    Description:
    Take an encrypted input string from standard input.
    Output the decrypted string to standard output.
  in  Input may contain extended ASCII bytes.

*/
/*
       Usage: /.decrypter encrypt_in.txt

       The program expects a secret key to be passed as the first and only command-line argument.
       The key is provided in hexadecimal format

*/

/*
How to Proceed
Start making functions for steps and test them
wrap functions together in DES_decrypt
Loop through all possible keys
Find way to tell which file is decrypted and print*/


/*
NEXT:
write left shift function
only needs to be done on 5 bits

do rest keygen implementation

implement permutations
*/

//Function Prototypes
void check_args(int argc, char* argv[], bool debug);

vector<unsigned char> get_decrypt_string(string filename, bool debug);

void DES_decrypt(vector<unsigned char> encrypted_vector, unsigned int key, bool debug);

void keygen(bitset<10> key, bitset<8>& key1, bitset<8>& key2, bool debug);




int main(int argc, char* argv[]) {
    //Global Variables
    bool debug = true;
    vector<unsigned char> encrypted_vector;
    bitset<10> key = 0b0000000000;

    if (debug) {
        cout << "Starting: " << argv[0] << " " << "in DEBUG mode\n" << endl;
    }

    //Check for correct number, format of args
    check_args(argc, argv, debug);
    //Open input file
    encrypted_vector = get_decrypt_string("decrypt_in 1.txt", debug);

    //Print encrypted string and key if Debugging
    if (debug) {
        cout << "Encrypted String: " << endl;
        for (unsigned char byte : encrypted_vector) {
            cout << static_cast<int>(byte) << " ";
        }
        cout << endl;
    }

    //Decrypt the string
    DES_decrypt(encrypted_vector, key, debug);



    exit(0);
}



//Function Definitions
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

vector<unsigned char> get_decrypt_string(string filename, bool debug) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: unable to open input file: " << filename << endl;
        exit(1);
    }
    vector<unsigned char> fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return fileContent;
}

void DES_decrypt(vector<unsigned char> encrypted_vector, unsigned int key, bool debug) {
    //Convert key to binary
    if (debug) {
        cout << "Key: " << key << endl;
    }

    //Do work here

}

void keygen(bitset<10> key, bitset<8>& key1, bitset<8>& key2, bool debug) {
    //Convert 10 bit key to 8 bit keys

}
