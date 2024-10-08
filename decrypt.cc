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
        cerr << "Key must be 5 characters long" << endl;
        exit(1);
    }
}

// vector<unsigned char> get_decrypt_string(const string& filename, bool debug) {
//     ifstream file(filename, ios::binary);
//     if (!file) {
//         cerr << "Error: unable to open input file: " << filename << endl;
//         exit(1);
//     }
//     vector<unsigned char> fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
//     file.close();
//     return fileContent;
// }

vector<unsigned char> get_decrypt_string(bool debug) {
    // Read from stdin
    vector<unsigned char> inputBuffer;
    char byte;

    while (cin.get(byte)) {
        inputBuffer.push_back(static_cast<unsigned char>(byte));
    }

    if (debug) {
        cout << "Read " << inputBuffer.size() << " bytes from stdin." << endl;
    }

    return inputBuffer;
}


std::bitset<8> expand(std::bitset<4> half) {
    std::bitset<8> expanded = 0b00000000;
    expanded[7] = half[0];
    expanded[6] = half[3];
    expanded[5] = half[2];
    expanded[4] = half[1];
    expanded[3] = half[2];
    expanded[2] = half[1];
    expanded[1] = half[0];
    expanded[0] = half[3];
    return expanded;
}

std::bitset<8> key_mix(const std::bitset<8>& byte, const std::bitset<8>& key) {
    bitset<8> result = 0b00000000;

    result[7] = byte[7] ^ key[7];
    result[6] = byte[6] ^ key[6];
    result[5] = byte[5] ^ key[5];
    result[4] = byte[4] ^ key[4];
    // P[0,0-3] first row - goes into S0

    result[3] = byte[3] ^ key[3];
    result[2] = byte[2] ^ key[2];
    result[1] = byte[1] ^ key[1];
    result[0] = byte[0] ^ key[0];
    //P[1,0-3] second row - goes into S1

        // Print the results for each bit position
    // std::cout << "Key Mixing" << "\n";
    // std::cout << "Byte: " << byte << "\n";
    // std::cout << "Key:  " << key << "\n";
    // std::cout << "Result: " << result << "\n";

    return result;
}

std::bitset<4> sbox_sub(std::bitset<8> byte, bool debug) {
    bitset<2> s0_result;
    bitset<2> s1_result;
    bitset<4> sbox_result;

    bitset<2> s0_col;
    bitset<2> s0_row;
    bitset<2> s1_col;
    bitset<2> s1_row;

    s0_row[1] = byte[7];
    s0_row[0] = byte[4];
    s0_col[1] = byte[6];
    s0_col[0] = byte[5];

    s1_row[1] = byte[3];
    s1_row[0] = byte[0];
    s1_col[1] = byte[2];
    s1_col[0] = byte[1];

    if (debug) {
        cout << "Performing S-Box Substitution" << endl;
        cout << "S0 Col: " << s0_col << endl;
        cout << "S0 Row: " << s0_row << endl;
        cout << "S1 Col: " << s1_col << endl;
        cout << "S1 Row: " << s1_row << endl;
    }

    //S0
    if (s0_col == 0) {
        if (s0_row == 0) { s0_result = 0b01; }
        else if (s0_row == 1) { s0_result = 0b11; }
        else if (s0_row == 2) { s0_result = 0b00; }
        else if (s0_row == 3) { s0_result = 0b11; }
    }
    else if (s0_col == 1) {
        if (s0_row == 0) { s0_result = 0b00; }
        else if (s0_row == 1) { s0_result = 0b10; }
        else if (s0_row == 2) { s0_result = 0b10; }
        else if (s0_row == 3) { s0_result = 0b01; }
    }
    else if (s0_col == 2) {
        if (s0_row == 0) { s0_result = 0b11; }
        else if (s0_row == 1) { s0_result = 0b01; }
        else if (s0_row == 2) { s0_result = 0b01; }
        else if (s0_row == 3) { s0_result = 0b11; }
    }
    else if (s0_col == 3) {
        if (s0_row == 0) { s0_result = 0b10; }
        else if (s0_row == 1) { s0_result = 0b00; }
        else if (s0_row == 2) { s0_result = 0b11; }
        else if (s0_row == 3) { s0_result = 0b10; }
    }
    else {
        cerr << "Error: S-Box Substitution (S0)" << endl;
        exit(1);
    }

    //S1

    if (s1_col == 0) {
        if (s1_row == 0) { s1_result = 0b00; }
        else if (s1_row == 1) { s1_result = 0b10; }
        else if (s1_row == 2) { s1_result = 0b11; }
        else if (s1_row == 3) { s1_result = 0b10; }
    }
    else if (s1_col == 1) {
        if (s1_row == 0) { s1_result = 0b01; }
        else if (s1_row == 1) { s1_result = 0b00; }
        else if (s1_row == 2) { s1_result = 0b00; }
        else if (s1_row == 3) { s1_result = 0b01; }
    }
    else if (s1_col == 2) {
        if (s1_row == 0) { s1_result = 0b10; }
        else if (s1_row == 1) { s1_result = 0b01; }
        else if (s1_row == 2) { s1_result = 0b01; }
        else if (s1_row == 3) { s1_result = 0b00; }
    }
    else if (s1_col == 3) {
        if (s1_row == 0) { s1_result = 0b11; }
        else if (s1_row == 1) { s1_result = 0b11; }
        else if (s1_row == 2) { s1_result = 0b00; }
        else if (s1_row == 3) { s1_result = 0b11; }
    }
    else {
        cerr << "Error: S-Box Substitution (S1)" << endl;
        exit(1);
    }

    if (debug) {
        cout << "S-Box Substitution Complete" << endl;
        cout << "S0 Result: " << s0_result << endl;
        cout << "S1 Result: " << s1_result << endl;
    }

    sbox_result[3] = s0_result[1];
    sbox_result[2] = s0_result[0];
    sbox_result[1] = s1_result[1];
    sbox_result[0] = s1_result[0];

    return sbox_result;
}




vector<unsigned char> DES_decrypt(const vector<unsigned char>& encrypted_vector, const bitset<10>& key, bool debug, vector<int>& nums) {
    vector<unsigned char> decrypted_vector;
    bitset<8> k1 = 0b00000000;
    bitset<8> k2 = 0b00000000;
    if (debug) {
        cout << "Starting DES Decryption" << endl;
        //cout << "Key: " << key << endl << endl;
        cout << "Key (int): " << key.to_ulong() << endl;
    }

    cerr << "p10: " << key.to_ullong() << endl;

    //Generate keys
    keygen(key, k1, k2, debug);
    if (debug) {
        //cout << "Keygen Complete" << endl;
        //cout << "Key 1: " << k1 << endl;
        cout << "Key 1 (int): " << k1.to_ulong() << endl;
        //cout << "Key 2: " << k2 << endl;
        cout << "Key 2 (int): " << k2.to_ulong() << endl << endl;
    }
    cerr << "k1: " << k1.to_ullong() << endl;
    cerr << "k2: " << k2.to_ullong() << endl;

    //Do Initial Permutation (IP)
    if (debug) {
        cout << "Starting Loop Over Encrypted bytes" << endl << endl;
    }


    int i = 0;  //print which byte    
    for (unsigned char byte : encrypted_vector) {
        bitset<8> encrypted_byte = byte;
        if (debug) {
            cout << "Decrypting byte #" << i << " " << "with value: " << static_cast<int>(encrypted_byte.to_ulong()) << endl;
            // cout << static_cast<int>(encrypted_byte.to_ulong()) << endl;
            // cout << "Encrypted Byte: " << encrypted_byte << endl;
            i++;

        }
        cerr << "decrypting byte #" << i << " with value: " << static_cast<int>(encrypted_byte.to_ulong()) << endl;

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
            cout << "IP : " << static_cast<int>(ip_byte.to_ulong()) << endl;
        }
        cerr << "ip: " << static_cast<int>(ip_byte.to_ulong()) << endl;
        // Perform Fiestal Function (round 1)
        bitset<8> fiestal_byte = fiestal(ip_byte, k2, debug);

        if (debug) {
            // cout << "Fiestal Round 1 Done: " << fiestal_byte << endl;
            // cout << "fk1: " << static_cast<int>(fiestal_byte.to_ulong()) << endl;
        }

        // Perform Nibble Swap

        //NEEDS CHANGED AFTER FIESTAL
        bitset<8> nibble_swap_byte = 0b00000000;
        nibble_swap_byte[7] = fiestal_byte[3];
        nibble_swap_byte[6] = fiestal_byte[2];
        nibble_swap_byte[5] = fiestal_byte[1];
        nibble_swap_byte[4] = fiestal_byte[0];
        nibble_swap_byte[3] = fiestal_byte[7];
        nibble_swap_byte[2] = fiestal_byte[6];
        nibble_swap_byte[1] = fiestal_byte[5];
        nibble_swap_byte[0] = fiestal_byte[4];


        cerr << "fk1: " << static_cast<int>(nibble_swap_byte.to_ulong()) << endl;
        if (debug) {
            // cout << "Nibble Swap Performed" << endl;
            // cout << "Before: " << fiestal_byte << endl;
            // cout << "After: " << nibble_swap_byte << endl;
            // cout << "Nibble Swap: " << static_cast<int>(nibble_swap_byte.to_ulong()) << endl;
            cout << "fk1: " << static_cast<int>(nibble_swap_byte.to_ulong()) << endl;
            // cout << "---------- [F1 COMPLETE] ----------" << endl << endl;
        }



        // Perform Fiestal Function (round 2)
        bitset<8> fiestal_byte2 = fiestal(nibble_swap_byte, k1, debug);



        if (debug) {
            // cout << "Fiestal Round 2 Done: " << fiestal_byte2 << endl;
            cout << "fk2: " << static_cast<int>(fiestal_byte2.to_ulong()) << endl;
        }
        cerr << "fk2: " << static_cast<int>(fiestal_byte2.to_ulong()) << endl;

        // Perform Inverse Initial Permutation
        bitset<8> inverse_ip_byte = 0b00000000;

        inverse_ip_byte[7] = fiestal_byte2[4];
        inverse_ip_byte[6] = fiestal_byte2[7];
        inverse_ip_byte[5] = fiestal_byte2[5];
        inverse_ip_byte[4] = fiestal_byte2[3];
        inverse_ip_byte[3] = fiestal_byte2[1];
        inverse_ip_byte[2] = fiestal_byte2[6];
        inverse_ip_byte[1] = fiestal_byte2[0];
        inverse_ip_byte[0] = fiestal_byte2[2];


        if (debug) {
            // cout << "Inverse Initial Permutation: " << inverse_ip_byte << endl;
            // cout << "Inverse IP: " << static_cast<int>(inverse_ip_byte.to_ulong()) << endl;
        }

        decrypted_vector.push_back(static_cast<unsigned char>(inverse_ip_byte.to_ullong()));
        nums.push_back(static_cast<int>(inverse_ip_byte.to_ulong()));
        //cerr << inverse_ip_byte.to_ulong() << " ";

        if (debug) {
            // cout << "Decrypted Byte: " << static_cast<int>(inverse_ip_byte.to_ulong()) << endl;
            // cout << "Decrypted Byte: " << inverse_ip_byte << endl;
            // cout << endl;
            // cout << "ASCII: " << static_cast<unsigned char>(inverse_ip_byte.to_ullong()) << endl;
        }

        //cout << endl;

    }
    return decrypted_vector;

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
        // cout << "Starting Keygen ---------------------" << endl << endl;
        // // cout << "Initial Key: " << key << endl;
        // // cout << "Initial Key (int): " << key.to_ulong() << endl;
        // cout << "Performing P10 Permutation" << endl;
        // cout << "3 5 2 7 4 10 1 9 8 6" << endl;
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
        // cout << "Before Permutation: " << endl << key << endl;
        // cout << "After Permutation: " << endl << key_permuted << endl << endl;
        // cout << "Splitting into two 5 bit halves" << endl;
        // cout << "P10: " << static_cast<int>(key_permuted.to_ulong()) << endl;
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
        // cout << "Key Half 1: " << keyhalf_1 << endl;
        // cout << "Key Half 2: " << keyhalf_2 << endl << endl;

        // cout << "Performing Left Shifts" << endl;
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
        // cout << "Left Shift (1) Performed" << endl;
        // cout << "Key Half 1: " << keyhalf_1 << endl;
        // cout << "Key Half 2: " << keyhalf_2 << endl << endl;
        // cout << "Performing P8 Permutation" << endl;
        // cout << "6 3 7 4 8 5 10 9" << endl;
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
        // cout << "Before P8:" << endl;
        // cout << keyhalf_1 << keyhalf_2 << endl;
        // cout << "After P8:" << endl;
        // cout << key1 << endl << endl;
    }
    // Perform second left shift
    if (debug) {
        // cout << "Performing Double Left Shift" << endl;
        // cout << "Before Shift 1: " << keyhalf_1 << " " << keyhalf_2 << endl;
    }
    temp = bitset<1>(keyhalf_1[4]);
    keyhalf_1 <<= 1;
    keyhalf_1[0] = temp[0];

    temp = bitset<1>(keyhalf_2[4]);
    keyhalf_2 <<= 1;
    keyhalf_2[0] = temp[0];

    if (debug) {
        // cout << "Left Shift (1 of 2) Performed" << endl;
        // cout << "Key Half 1: " << keyhalf_1 << endl;
        // cout << "Key Half 2: " << keyhalf_2 << endl << endl;
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
        // cout << "LS (2 of 2) Performed" << endl;
        // cout << "Key Half 1: " << keyhalf_1 << endl;
        // cout << "Key Half 2: " << keyhalf_2 << endl << endl;
        // cout << "Key 1: " << key1 << endl;
        // cout << "Key 2: " << key2 << endl;
        // cout << "Keygen Complete ---------------------" << endl << endl;
    }
}


bitset<8> fiestal(const std::bitset<8>& byte, const std::bitset<8>& key, bool debug) {
    if (debug) {
        // cout << "Fiestal Entered" << endl;
        // cout << "Byte: " << byte << endl;
        // cout << "Key: " << key << endl;
    }

    ///Split into halves
    bitset<4> left(byte.to_ulong() >> 4);
    bitset<4> right(byte.to_ulong() & 0xF);

    if (debug) {
        // cout << "Left: " << left << endl;
        // cout << "Right: " << right << endl;
    }
    //Starting Step F(R, SK)
    // Perform Expansion Permutation
    bitset<8> expanded_byte = expand(right);

    if (debug)
    {
        // cout << "Expanded Byte: " << expanded_byte << endl;
    }

    // Perform Key Mixing
    //Break out into named bits and do XOR
    bitset<8> mixed_byte = key_mix(expanded_byte, key);


    if (debug) {
        // cout << "Key Mixing done" << endl;
        // cout << "Expanded Byte: " << expanded_byte << endl;
        // cout << "Key: " << key << endl;
        // cout << "Mixed Byte: " << mixed_byte << endl;
    }
    //Do S-Box Substitution Next
    bitset<4> sbox_result = sbox_sub(mixed_byte, debug);

    if (debug) {
        // cout << "S-Box Substitution Complete" << endl;
        // cout << "S-Box Result: " << sbox_result << endl;
    }
    //Perform Permutation (P4)

    bitset<4> permuted_byte = 0b0000;
    permuted_byte[3] = sbox_result[2];
    permuted_byte[2] = sbox_result[0];
    permuted_byte[1] = sbox_result[1];
    permuted_byte[0] = sbox_result[3];

    if (debug) {
        cout << "P4 Permutation: " << permuted_byte << endl;
    }

    //XOR Left half with permuted byte
    bitset<4> new_left = left ^ permuted_byte;

    if (debug) {
        cout << "New Left (After Xor): " << new_left << endl;
    }

    //Combine left and right
    bitset<8> new_byte = 0b00000000;

    new_byte[7] = new_left[3];
    new_byte[6] = new_left[2];
    new_byte[5] = new_left[1];
    new_byte[4] = new_left[0];
    new_byte[3] = right[3];
    new_byte[2] = right[2];
    new_byte[1] = right[1];
    new_byte[0] = right[0];

    if (debug) {
        // cout << "New Byte: " << new_byte << endl;
        // cout << "New Left: " << new_left << endl;
        // cout << "New Byte: " << new_byte << endl;
        // cout << "Fiestal Function Complete" << endl;
    }

    return new_byte;

}

