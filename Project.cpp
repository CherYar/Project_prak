#include <iostream>
#include <fstream>
#include <openssl/evp.h>

#define BUF_SIZE 1024

using namespace std;

int encrypt_file(const char* in_filename, const char* out_filename, const char* key, const char* iv) {
    EVP_CIPHER_CTX* ctx;
    const EVP_CIPHER* cipher;
    unsigned char in_buf[BUF_SIZE], out_buf[BUF_SIZE + EVP_MAX_BLOCK_LENGTH];
    int in_len, out_len, len;

    // Set up the encryption context and algorithm
    OpenSSL_add_all_algorithms();
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, cipher, NULL, (unsigned char*)key, (unsigned char*)iv);

    // Open the input and output files and encrypt the data
    ifstream fin(in_filename, ios::binary);
    if (!fin) {
        cerr << "Unable to open input file.\n";
        return 1;
    }
    ofstream fout(out_filename, ios::binary);
    if (!fout) {
        cerr << "Unable to open output file.\n";
        return 1;
    }
    while (fin) {
        in_len = fin.readsome((char*)in_buf, BUF_SIZE);
        if (in_len > 0) {
            if (!EVP_EncryptUpdate(ctx, out_buf, &out_len, in_buf, in_len)) {
                cerr << "Error encrypting data.\n";
                EVP_CIPHER_CTX_cleanup(ctx);
                return 1;
            }
            fout.write((char*)out_buf, out_len);
        }
    }
    if (!EVP_EncryptFinal_ex(ctx, out_buf, &out_len)) {
        cerr << "Error finalizing encryption.\n";
        EVP_CIPHER_CTX_cleanup(ctx);
        return 1;
    }
    fout.write((char*)out_buf, out_len);

    return 0;
}

int decrypt_file(const char* in_filename, const char* out_filename, const char* key, const char* iv) {
    EVP_CIPHER_CTX* ctx;
    const EVP_CIPHER* cipher;
    unsigned char in_buf[BUF_SIZE + EVP_MAX_BLOCK_LENGTH], out_buf[BUF_SIZE];
    int in_len, out_len, len;

    // Set up the decryption context and algorithm
    OpenSSL_add_all_algorithms();
    cipher = EVP_get_cipherbyname("aes-256-cbc");
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, cipher, NULL, (unsigned char*)key, (unsigned char*)iv);

    // Open the input and output files and decrypt the data
    ifstream fin(in_filename, ios::binary);
    if (!fin) {
        cerr << "Unable to open input file.\n";
        return 1;
    }
    ofstream fout(out_filename, ios::binary);
    if (!fout) {
        cerr << "Unable to open output file.\n";
        return 1;
    }
    while (fin) {
        in_len = fin.readsome((char*)in_buf, BUF_SIZE + EVP_MAX_BLOCK_LENGTH);
        if (in_len > 0) {
            if (!EVP_DecryptUpdate(ctx, out_buf, &out_len, in_buf, in_len)) {
                cerr << "Error decrypting data.\n";
                EVP_CIPHER_CTX_cleanup(ctx);
                return 1;
            }
            fout.write((char*)out_buf, out_len);
        }
    }
    if (!EVP_DecryptFinal_ex(ctx, out_buf, &out_len)) {
        cerr << "Error finalizing decryption.\n";
        EVP_CIPHER_CTX_cleanup(ctx);
        return 1;
    }
    fout.write((char*)out_buf, out_len);

    return 0;
}

int main() {
    const char* in_filename = "input.txt";
    const char* enc_filename = "encrypted.dat";
    const char* dec_filename = "decrypted.txt";
    const char* key = "01234567890123456789012345678901";
    const char* iv = "1234123412341234";
    // Decrypt the encrypted file
    if (decrypt_file(enc_filename, dec_filename, key, iv) != 0) {
        cerr << "Error decrypting file.\n";
        return 1;
    }

    // Output results
    cout << "Encryption and decryption completed successfully.\n";
    return 0;
}

