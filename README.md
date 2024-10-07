# Assignment 2 - Simplified DES

## Overview

This project is part of a school assignment focused on implementing a **Simplified DES (Data Encryption Standard)** algorithm. Depending on your process ID (PID), you will either create an encrypter or a decrypter.

### Decrypter
- Write a Simplified DES decrypter.
- The program should:
  - Take an encrypted input string from standard input.
  - Output the decrypted string to standard output.
  - Input may contain extended ASCII bytes.

### Collaboration
- You are encouraged to test your implementation with others, but the actual development should be done individually.
- You will only be graded on your assigned functionality (encryption or decryption based on your PID).

## Usage

The program expects a **secret key** to be passed as the first and only command-line argument. The key is provided in hexadecimal format (e.g., `0xYYY`).

Example command:
```bash
./decrypter 0x36C < 'decrypt_in 1.txt'
