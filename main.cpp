#include "Crypto.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dlfcn.h>

using namespace std;

// CLI вспомогательное

string getArg(int argc, char* argv[], const string& name) {
    for (int i = 1; i < argc - 1; ++i)
        if (argv[i] == name) return argv[i + 1];
    return "";
}

bool hasFlag(int argc, char* argv[], const string& flag) {
    for (int i = 1; i < argc; ++i)
        if (argv[i] == flag) return true;
    return false;
}

// CLI-режим

int cliMode(int argc, char* argv[]) {
    string cipher = getArg(argc, argv, "--cipher");
    bool encrypt = hasFlag(argc, argv, "-e") || hasFlag(argc, argv, "--encrypt");
    bool decrypt = hasFlag(argc, argv, "-d") || hasFlag(argc, argv, "--decrypt");

    string input = getArg(argc, argv, "--input");
    string output = getArg(argc, argv, "--output");

    string keyFile = getArg(argc, argv, "--key");
    string rowsStr = getArg(argc, argv, "--rows");
    string colsStr = getArg(argc, argv, "--cols");

    if (cipher.empty() || input.empty() || output.empty() || (!encrypt && !decrypt)) {
        cout << "Недостаточно параметров.\n";
        cout << "Пример:\n";
        cout << "./crypto_tool --cipher fixedshift -e --input in.txt --output out.txt --key perm.txt\n";
        cout << "Алгоритмы: fixedshift, tableshift, zeroandone\n";
        return 1;
    }

    // подбор библиотеки
    string libname;
    if (cipher == "fixedshift") libname = "./lib/libfixedshift.so";
    else if (cipher == "tableshift") libname = "./lib/libtableshift.so";
    else if (cipher == "zeroandone") libname = "./lib/libzeroandone.so";
    else {
        cout << "Неизвестный алгоритм: " << cipher << endl;
        return 2;
    }

    // загрузка библиотеки
    void* handle = dlopen(libname.c_str(), RTLD_LAZY);
    if (!handle) {
        cout << "Не удалось загрузить библиотеку: " << libname << endl;
        cout << dlerror() << endl;
        return 3;
    }

    // выбор и подготовка функции
    if (cipher == "fixedshift") {
        using func_enc_t = void(*)(const string&, const string&, const vector<int>&);
        using func_dec_t = void(*)(const string&, const string&, const vector<int>&);
        
        func_enc_t encFunc = (func_enc_t) dlsym(handle, "fixedshiftEnc");
        func_dec_t decFunc = (func_dec_t) dlsym(handle, "fixedshiftDec");

        if (!encFunc || !decFunc) {
            cout << "Не найдены функции шифрования/дешифрования!" << endl;
            dlclose(handle);
            return 4;
        }

        if (keyFile.empty()) {
            cout << "Для fixedshift требуется файл с перестановкой (--key)" << endl;
            dlclose(handle);
            return 1;
        }

        ifstream keyIn(keyFile);
        if (!keyIn.is_open()) {
            cout << "Не удалось открыть файл с перестановкой: " << keyFile << endl;
            dlclose(handle);
            return 1;
        }

        vector<int> perm;
        int num;
        while (keyIn >> num) {
            perm.push_back(num);
        }
        keyIn.close();

        // выполнение операции
        if (encrypt) encFunc(input, output, perm);
        if (decrypt) decFunc(input, output, perm);

    } 
    else if (cipher == "tableshift") {
        using func_enc_t = void(*)(const string&, const string&, int, int, const string&);
        using func_dec_t = void(*)(const string&, const string&, const string&);
        
        func_enc_t encFunc = (func_enc_t) dlsym(handle, "tableshiftEnc");
        func_dec_t decFunc = (func_dec_t) dlsym(handle, "tableshiftDec");

        if (!encFunc || !decFunc) {
            cout << "Не найдены функции шифрования/дешифрования!" << endl;
            dlclose(handle);
            return 4;
        }

        if (encrypt) {
            if (rowsStr.empty() || colsStr.empty() || keyFile.empty()) {
                cout << "Для tableshift в режиме шифрования требуются --rows, --cols и --key" << endl;
                dlclose(handle);
                return 1;
            }
            int rows = stoi(rowsStr);
            int cols = stoi(colsStr);
            encFunc(input, output, rows, cols, keyFile);
        }
        if (decrypt) {
            if (keyFile.empty()) {
                cout << "Для tableshift в режиме дешифрования требуется --key" << endl;
                dlclose(handle);
                return 1;
            }
            decFunc(input, output, keyFile);
        }
    }
    else if (cipher == "zeroandone") {
        using func_t = bool(*)(const string&, const string&);
        func_t encFunc = (func_t) dlsym(handle, "zeroandoneEnc");
        func_t decFunc = (func_t) dlsym(handle, "zeroandoneDec");

        if (!encFunc || !decFunc) {
            cout << "Не найдены функции шифрования/дешифрования!" << endl;
            dlclose(handle);
            return 4;
        }

        if (encrypt) encFunc(input, output);
        if (decrypt) decFunc(input, output);
    }

    dlclose(handle);
    cout << "Готово. Результат: " << output << endl;
    return 0;
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    // CLI
    if (argc > 1)
        return cliMode(argc, argv);

    // интерактивное меню
    Crypto system;
    int choice;

    do {
        system.showMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }
        cin.ignore();

        if (choice < 0 || choice > 4) { 
            char repeat;
            do {
                cout << "Неверный выбор. Повторить ввод? (Y/N): ";
                cin >> repeat;
                cin.ignore(10000, '\n');
                repeat = toupper(repeat);
            } while (repeat != 'Y' && repeat != 'N');

            if (repeat == 'N') {
                cout << "Выход...\n";
                break;
            } else {
                continue; 
            }
        }

        switch (choice) {
            case 1: system.inputTextToFile(); break;
            case 2: system.encryptFile(); break;
            case 3: system.decryptFile(); break;
            case 4: system.displayFile(); break;
            case 0:
                cout << "Выход..." << endl;
                break;
        }

    } while (choice != 0);

    return 0;
}
