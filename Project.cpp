#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <time.h>
#include <iomanip>
#include <cctype>
#include <limits>
#include <functional>
#include <algorithm>
#include <openssl/rand.h>
#include <openssl/evp.h>

#define BUF_SIZE 1024
#define CinDel cin.ignore((numeric_limits<streamsize>::max)(), '\n');

bool ValidInt(string num) {
    if (num == "" || num.length() > 9)
        return 0;
    num.append("a");
    if (num.find_first_not_of("0123456789+-") != num.length() - 1)
        return 0;
    return 1;
}

bool ValidIntModernised(const string& num) {
    if (num.empty() || num.size() > 9) {
        return false;
    }
    size_t start = 0;
    if (num[0] == '-' || num[0] == '+') {
        if (num.size() == 1 || num[1] == '0') {
            return false;
        }
        start = 1;
    }
    for (size_t i = start; i < num.size(); i++) {
        if (num[i] < '0' || num[i] > '9') {
            return false;
        }
    }
    return true;
}

bool ValidFloat(string num) {
    if (num == "" || num.length() > 9)
        return 0;
    num.append("a");
    if (num.find_first_not_of("0123456789.-+") != num.length() - 1)
        return 0;
    return 1;
}

bool ValidFloatModernised(const string& num) {
    if (num.empty()) {
        return false;
    }
    size_t start = 0;
    if (num[0] == '-' || num[0] == '+') {
        start = 1;
    }
    size_t decimal_count = 0;
    for (size_t i = start; i < num.size(); i++) {
        if (num[i] == '.') {
            decimal_count++;
            if (decimal_count > 1) {
                return false;
            }
        }
        else if (num[i] < '0' || num[i] > '9') {
            return false;
        }
    }
    return true;
}
int readIntV(const string& prompt, const function<bool(int)>& validator) {
    string input;
    cout << prompt;
    while (cin >> input) {
        if (ValidIntModernised(input)) {
            int value = stoi(input);
            if (validator(value)) {
                return value;
            }
        }
        cout << "Некорректное значение. Пожалуйста, введите заново: ";
    }
    CinDel
        return 0;
}
int readIntLV(const string& prompt, const function<bool(long)>& validator) {
    string input;
    cout << prompt;
    while (cin >> input) {
        if (ValidIntModernised(input)) {
            long value = stol(input);
            if (validator(value)) {
                return value;
            }
        }
        cout << "Некорректное значение. Пожалуйста, введите заново: ";
    }
    CinDel
        return 0;
}

int readIntV(const string& prompt, const function<bool(int)>& validator, const int& defaultValue) {
    string input;
    cout << prompt;
    while (cin >> input) {
        if (ValidIntModernised(input)) {
            int value = stoi(input);
            if (validator(value)) {
                return value;
            }
        }
        cout << "Некорректное значение. Пожалуйста, введите заново: ";
    }
    CinDel
        return defaultValue;
}
int readInt(const string& prompt) {
    string input;
    cout << prompt;
    while (cin >> input) {
        if (ValidIntModernised(input)) {
            int value = stoi(input);
            return value;
        }
        cout << "Некорректное значение. Пожалуйста, введите заново: ";
    }
    CinDel
        return 0;
}
string readStr(const string& prompt) {
    string input;
    cout << prompt;
    while (getline(cin, input)) {
        if (!input.empty()) {
            return input;
        }
    }
    CinDel
}

string readStrV(const string& prompt, const function<bool(string)>& validator) {
    string input;
    cout << prompt;
    while (getline(cin, input)) {
        if (!input.empty()) {
            if (validator(input)) {
                return input;
            }
        }
        cout << "Некорректное значение. Пожалуйста, введите заново: ";
    }
    CinDel
}
string readStrW(const string& prompt, const function<bool(const string&)>& validator) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (validator(input)) {
            return input;
        }
        cout << "Некорректное значение. Пожалуйста, введите заново." << endl;
    }
    CinDel
}
using namespace std;
bool generate_random_key(unsigned char* key, int keylen) {
    if (!RAND_bytes(key, keylen)) {
        return false;
    }
    return true;
}
/*int menu() {//Главное меню
    Decrypt();
    list.ListFromFileBin("database.bin");
    cout << "База данных расшифрована. Программа готова к работе." << endl; system("pause");
    int mchoice = -1;
    for (; mchoice != 0;)
    {
        CinDel
            unsigned short lsiz = list.getSize(); string lsize = to_string(lsiz);
        system("cls");
        cout << "Текущее количество студентов в базе данных: " + lsize << '\n';
        cout << "-------------------------------------------------------------\n";
        cout << "Выберете действие:\n";
        cout << "-------------------------------------------------------------\n";
        cout << "1.Вывод краткой информации о всех студентах\n";
        cout << "2.Вывод полной информации о всех студентах (не рекомендуется)\n";
        cout << "3.Вывод полной информации о конкретном студенте\n";
        cout << "-------------------------------------------------------------\n";
        cout << "4.Изменение данных конкретного студента\n";
        cout << "5.Добавление нового студента с вводом с клавиатуры\n";
        cout << "6.Удаление студента из базы данных\n";
        cout << "-------------------------------------------------------------\n";
        cout << "7.Выполнить задание варианта 69 (разбиение и сортировка)\n";
        cout << "-------------------------------------------------------------\n";
        cout << "0.Выход из программы, с последующим шифрованием базы данных\n";
        cout << "-------------------------------------------------------------\n";
        cout << ">";
        mchoice = readIntV(" ", [](int c) {return c >= 0 && c <= 7; });
        switch (mchoice)
        {
        case 1: {cout << endl; list.printAll(); system("pause"); break; }
        case 2: {cout << endl; list.printMNOGO(); system("pause"); break; }
        case 3: {CinDel cout << endl; string prompt = "Введите номер студента(1 - " + lsize + "): ";
            unsigned short index = readIntV(prompt, [&](int i) { return i >= 1 && i <= (lsiz); });
            list.printStudent(index - 1); system("pause"); break; }
        case 4: {CinDel cout << endl; string prompt = "Введите номер студента(1 - " + lsize + "): ";
            unsigned short index = readIntV(prompt, [&](int i) { return i >= 1 && i <= (lsiz); }); list.studsubmenu(index - 1); system("pause"); break; }
        case 5: {if (lsiz >= 15) { cout << "\nНевозможно добавить студента, достигнут предел." << endl; system("pause"); break; } else { CinDel student nstud; nstud.makestudent(); list.addStudent(nstud); cout << "\nСтудент добавлен" << endl; system("pause"); break; }  }
        case 6: { CinDel cout << endl; string prompt = "Введите номер студента(1 - " + lsize + "): ";
            unsigned short index = readIntV(prompt, [&](int i) { return i >= 1 && i <= (lsiz); }); list.removeStudent(index - 1); cout << "\nСтудент удалён." << endl; system("pause"); break;  }
        case 7: { CinDel cout << endl; unsigned int mx, mi; mi = readIntV("Введите минимальный год рождения для сортировки:", [](int y) { return y >= 1950 && y <= 2030; });
            mx = readIntV("Введите максимальный год рождения для сортировки:", [](int y) { return y >= 1950 && y <= 2030; });
            splitStudentsByGrades(list, mi, mx); system("pause"); break; }
        case 0: { list.ListToFileBin("database.bin"); Crypt(); cout << "\nРабота программы завершена, база данных зашифрована."; return 0; system("pause"); }
        default: {cout << "\nНеизвестная операция!\n"; CinDel break; }
        }
    }
    return 0;
}*/

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

