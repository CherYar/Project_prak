#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <cctype>
#include <limits>
#include <functional>
#include <algorithm>
#include <openssl/rand.h>//Для создания случайного ключа
#include <openssl/evp.h>//Для использования стандарта шифрования AES
using namespace std;
#define CinDel cin.ignore((numeric_limits<streamsize>::max)(), '\n');//Очистка буфера ввода

bool ValidIntModernised(const string& num) {//Проверки на возможность корректного перевода строки в число
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
int readIntV(const string& prompt, const function<bool(int)>& validator) {//Функции для чтения с клавиатуры в переменную с проверкой на корректность
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
        return 0;
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
}

string readStrV(const string& prompt, const function<bool(const string&)>& validator) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (validator(input)) {
            return input;
        }
        cout << "Некорректное значение. Пожалуйста, введите заново." << endl;
    }
}

bool mrandkey(unsigned char* key, int keylen) {//Создание криптографиечески стойкого случайного ключа, используя генератор случайных чисел библиотеки OpenSSL. Обычные средства генерации рандомных чисел в C++ могуть быть предсказуемыми. Хотя для такого простого проекта это не так уж и важно
    if (!RAND_bytes(key, keylen)) {
        return false;
    }
    return true;
}
int encrypt(const string& filename, const unsigned char* key, const unsigned char* iv) {//Шифрование файла
    // Открытие входного файла
    ifstream input(filename, ios::binary);
    if (!input.is_open()) {
        cerr << "Ошибка при открытии файла " << filename << endl;
        return 1;
    }

    // Создание выходного файла
    ofstream output(filename + ".enc", ios::binary);
    if (!output.is_open()) {
        cerr << "Ошибка при создании выходного файла" << endl;
        input.close();
        return 1;
    }

    // Создание и инициализация контекста шифрования
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "Ошибка при создании контекста шифрования" << endl;
        input.close();
        output.close();
        return 1;
    }
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        cerr << "Ошибка при инициализации контекста шифрования" << endl;
        EVP_CIPHER_CTX_free(ctx);
        input.close();
        output.close();
        return 1;
    }

    // Чтение данных из входного файла и шифрование их
    const size_t buffer_size = 4096;
    unsigned char input_buffer[buffer_size];
    unsigned char output_buffer[buffer_size + EVP_MAX_BLOCK_LENGTH];
    int output_len = 0;
    while (input.good()) {
        input.read(reinterpret_cast<char*>(input_buffer), buffer_size);
        size_t input_len = input.gcount();
        if (!EVP_EncryptUpdate(ctx, output_buffer, &output_len, input_buffer, input_len)) {
            cerr << "Ошибка при шифровании данных" << endl;
            EVP_CIPHER_CTX_free(ctx);
            input.close();
            output.close();
            return 1;
        }
        output.write(reinterpret_cast<char*>(output_buffer), output_len);
    }

    // Завершение шифрования
    if (!EVP_EncryptFinal_ex(ctx, output_buffer, &output_len)) {
        cerr << "Ошибка при завершении шифрования" << endl;
        EVP_CIPHER_CTX_free(ctx);
        input.close();
        output.close();
        return 1;
    }
    output.write(reinterpret_cast<char*>(output_buffer), output_len);

    // Освобождение ресурсов
    EVP_CIPHER_CTX_free(ctx);
    input.close();
    output.close();

    cout << "Файл " + filename + " успешно зашифрован" << endl;
    return 0;
}

int decrypt(const string& filename, const unsigned char* key, const unsigned char* iv) {//Дешифрование файла
    // Открытие входного файла
    ifstream input(filename, ios::binary);
    if (!input.is_open()) {
        cerr << "Ошибка при открытии файла " << filename << endl;
        return 1;
    }

    // Создание выходного файла
    ofstream output(filename + ".dec", ios::binary);
    if (!output.is_open()) {
        cerr << "Ошибка при создании выходного файла" << endl;
        input.close();
        return 1;
    }

    // Создание и инициализация контекста расшифровки
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "Ошибка при создании контекста расшифровки" << endl;
        input.close();
        output.close();
        return 1;
    }
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        cerr << "Ошибка при инициализации контекста расшифровки" << endl;
        EVP_CIPHER_CTX_free(ctx);
        input.close();
        output.close();
        return 1;
    }

    // Чтение данных из входного файла и расшифровка их
    const size_t buffer_size = 4096;
    unsigned char input_buffer[buffer_size];
    unsigned char output_buffer[buffer_size + EVP_MAX_BLOCK_LENGTH];
    int output_len = 0;
    while (input.good()) {
        input.read(reinterpret_cast<char*>(input_buffer), buffer_size);
        size_t input_len = input.gcount();
        if (!EVP_DecryptUpdate(ctx, output_buffer, &output_len, input_buffer, input_len)) {
            cerr << "Ошибка при расшифровке данных" << endl;
            EVP_CIPHER_CTX_free(ctx);
            input.close();
            output.close();
            return 1;
        }
        output.write(reinterpret_cast<char*>(output_buffer), output_len);
    }

    // Завершение расшифровки
    if (!EVP_DecryptFinal_ex(ctx, output_buffer, &output_len)) {
        cerr << "Ошибка при завершении расшифровки" << endl;
        EVP_CIPHER_CTX_free(ctx);
        input.close();
        output.close();
        return 1;
    }
    output.write(reinterpret_cast<char*>(output_buffer), output_len);

    // Освобождение ресурсов
    EVP_CIPHER_CTX_free(ctx);
    input.close();
    output.close();

    cout << "Файл " + filename + " успешно расшифрован" << endl;
    return 0;
}

int menu() {//Меню для удобной работы с программой
    unsigned char key[32];
    if (!mrandkey(key, sizeof(key))) {
        cerr << "Ошибка при генерации случайного ключа" << endl;
        return 1;
    }
    unsigned char iv[16];
    if (!mrandkey(iv, sizeof(iv))) {
        cerr << "Ошибка при генерации вектора инициализации" << endl;
        return 2;
    }
    int choice = -1;
    cout << "Добро пожаловать в простую программу шифрования. Для продолжения нажмите ENTER." << endl;
    for (; choice != 0;)
    {
        CinDel
        system("cls");
        cout << "-------------------------------------------------------------\n";
        cout << "Выберете действие:\n";
        cout << "-------------------------------------------------------------\n";
        cout << "1.Зашифровать файл\n";
        cout << "2.Расшифровать файл\n";
        cout << "-------------------------------------------------------------\n";
        cout << "3.Показать информацию о программе\n";
        cout << "-------------------------------------------------------------\n";
        cout << "0.Выход из программы\n";
        cout << "-------------------------------------------------------------\n";
        cout << ">";
        choice = readIntV(" ", [](int c) {return c >= 0 && c <= 3; });
        switch (choice)
        {
        case 0: {cout << "\nРабота программы завершена." << endl; break; return 0; }
        case 1: {CinDel string filename = readStr("\nВведите имя файла для шифрования (или 0 для отмены) :"); if (filename == "0") { cout << "\nОтмена операции, возвращение в главное меню." << endl; system("pause"); break; }
              else {
            encrypt(filename, key, iv);
            system("pause");
            break;}}
        case 2: {CinDel string filename = readStr("\nВведите имя файла для дешифрования (или 0 для отмены) :"); if (filename == "0") { cout << "\nОтмена операции, возвращение в главное меню." << endl; system("pause"); break; }
              else {
            decrypt(filename, key, iv);
            system("pause");
            break;
        }}
        case 3: {cout << "\nДанная программа написана на языке C++ стандарта С++20 в Microsoft Visual Studio 2022.\nОна предназначена для шифрования/расшифровывания файлов используя стандарт AES-256 из библиотеки OpenSSL v3.1.0." << endl; system("pause"); break; }
        default: {cout << "\nНеизвестная операция!\n"; CinDel break; }
        }
    }
    return 0;
}



int main() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251);//Установка корректной кодировки
    menu();
}

