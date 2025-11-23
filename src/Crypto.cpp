#include "Crypto.h"
#include "FixedShift.h"
#include "TableShift.h"
#include "ZeroAndOne.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// реализация методов класса
void Crypto::showMenu() {
    cout << "\n========================================\n";
    cout << "              CRYPTO TOOL\n";
    cout << "========================================\n";
    cout << "1 – Ввод текста и сохранение в файл\n";
    cout << "2 – Шифрование файла\n";
    cout << "3 – Дешифрование файла\n";
    cout << "4 – Просмотр файла\n";
    cout << "0 – Выход\n";
    cout << "========================================\n";
    cout << "Ваш выбор: ";
}

string Crypto::getFilename(const string& prompt) { 
    string filename; 
    cout << prompt; 
    getline(cin, filename); 
    return filename; 
}

string Crypto::checkFilename(const string& prompt) {
    string filename;
    char repeat;

    do {
        cout << prompt;
        getline(cin, filename);

        if (fileExists(filename)) {
            return filename; 
        } else {
            do {
                cout << "Файл " << filename << " не найден. Повторить ввод? (Y/N): ";
                cin >> repeat;
                repeat = toupper(repeat);
                cin.ignore(10000, '\n');
            } while (repeat != 'Y' && repeat != 'N');

            if (repeat == 'N') return "";
        }
    } while (true);
}


bool Crypto::fileExists(const string& filename) {
    std::ifstream fin(filename, ios::binary);
    return fin.good();
}

int Crypto::chooseAlgorithm() {
    int algo;
    char repeat;
    do {
        cout << "\nВыберите алгоритм:\n";
        cout << "1 – Шифровка последовательностей нулей и единиц\n";
        cout << "2 – Шифровка простой табличной перестановкой\n";
        cout << "3 – Шифровка зафиксированной перестановкой\n";
        cout << "Ваш выбор: ";
        if (!(cin >> algo)) {
            cin.clear();
            cin.ignore(10000, '\n');
            algo = -1;
        }
        cin.ignore(10000, '\n');

        if (algo < 1 || algo > 3) {
            do {
                cout << "Неизвестный алгоритм! Повторить ввод? (Y/N): ";
                cin >> repeat;
                repeat = toupper(repeat);
                cin.ignore(10000, '\n');
            } while (repeat != 'Y' && repeat != 'N');

            if (repeat == 'N') return 0;
        } else {
            return algo;
        }
    } while (true);
}


void Crypto::clearScreen() {
    #ifdef _WIN32
        if (system("cls") != 0) std::cerr << "Не удалось очистить экран.\n";
    #else
        if (system("clear") != 0) std::cerr << "Не удалось очистить экран.\n";
    #endif
}

// ввод и вывод
void Crypto::inputTextToFile() {
    string filename = getFilename("Введите имя файла для сохранения текста: ");
    cout << "Введите текст:\n";
    string text;
    getline(cin, text);

    ofstream fout(filename, ios::binary);
    if (!fout) {
        cerr << "Ошибка открытия файла!\n";
        return;
    }
    fout.write(text.c_str(), text.size());
    cout << "Введённый текст был успешно сохранен в " << filename << endl;
}

void Crypto::displayFile() {
    string filename = getFilename("Введите имя файла для просмотра: ");
    ifstream fin(filename, ios::binary);
    if (!fin) { cerr << "Ошибка открытия файла!\n"; return; }

    stringstream ss;
    ss << fin.rdbuf();
    cout << "Содержимое файла:\n" << ss.str() << endl;
}

// алгоритмы шифрования и расшифровки

// последовательность нулей и единиц
bool Crypto::zeroandoneEnc(const string& inFile, const string& outFile) { 
    return ::zeroandoneEnc(inFile, outFile);
}
bool Crypto::zeroandoneDec(const string& inFile, const string& outFile) { 
    return ::zeroandoneDec(inFile, outFile);
}

// простая табличная
void Crypto::tableshiftEnc(const string& inFile, const string& outFile, int rows, int cols, const string& keyFile) {
    ::tableshiftEnc(inFile, outFile, rows, cols, keyFile);
}
void Crypto::tableshiftDec(const string& inFile, const string& outFile, const string& keyFile) {
    ::tableshiftDec(inFile, outFile, keyFile);
}

// фиксированная перестановка
bool Crypto::isValidPermutation(const vector<int>& perm) { 
    return ::isValidPermutation(perm); 
}
void Crypto::fixedshiftEnc(const string& inFile, const string& outFile, const vector<int>& perm) {
    ::fixedshiftEnc(inFile, outFile, perm);
}
void Crypto::fixedshiftDec(const string& inFile, const string& outFile, const vector<int>& perm) {
    ::fixedshiftDec(inFile, outFile, perm);
}

// работа с файлами
vector<unsigned char> Crypto::readBinaryFile(const string& filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) return {};
    vector<unsigned char> data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    return data;
}

void Crypto::writeBinaryFile(const string& filename, const vector<unsigned char>& data) {
    ofstream fout(filename, ios::binary);
    fout.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// шифрование и дешифрование для меню
void Crypto::encryptFile() {
    int algo = chooseAlgorithm();
    if (algo == 0) return;

    string infile = checkFilename("Введите имя входного файла: ");
    if (infile.empty()) return;
    string outfile = getFilename("Введите имя выходного файла: ");

    if (algo == ZEROANDONE) {
        if (!zeroandoneEnc(infile, outfile)) {
            cerr << "Ошибка: входной файл содержит символы, отличные от 0 и 1.\n";
            return;
        }
        cout << "Файл успешно зашифрован. Результат сохранён в: " << outfile << endl;
    }
    else if (algo == TABLESHIFT) {
        int rows, cols;
        cout << "Введите количество строк: "; 
        cin >> rows;
        cout << "Введите количество столбцов: "; 
        cin >> cols;
        cin.ignore();
        string keyfile = getFilename("Введите имя файла для сохранения ключа: ");
        tableshiftEnc(infile, outfile, rows, cols, keyfile);
        cout << "Файл успешно зашифрован. Результат сохранён в: " << outfile << endl;
    } 
    else if (algo == FIXEDSHIFT) {
        int n;
        cout << "Введите размер перестановки: ";
        cin >> n;
        vector<int> perm(n);
        cout << "Введите перестановку через пробел: ";
        for (int i = 0; i < n; ++i) cin >> perm[i];
        cin.ignore();
        if (!isValidPermutation(perm)) {
            cerr << "Неверная перестановка!\n";
            return;
        }
        fixedshiftEnc(infile, outfile, perm);
        cout << "Файл успешно зашифрован. Результат сохранён в: " << outfile << endl;
    } 
    else {
        cerr << "Неизвестный алгоритм!\n";
    }
}

void Crypto::decryptFile() {
    int algo = chooseAlgorithm();
    if (algo == 0) return;

    string infile = checkFilename("Введите имя входного файла: ");
    if (infile.empty()) return;
    string outfile = getFilename("Введите имя выходного файла: ");

    if (algo == ZEROANDONE) {
        if (!zeroandoneDec(infile, outfile)) {
            cerr << "Ошибка: входной файл содержит символы, отличные от 0 и 1.\n";
            return;
        }
        cout << "Файл успешно расшифрован. Результат сохранён в: " << outfile << endl;
    } 
    else if (algo == TABLESHIFT) {
        string keyfile = getFilename("Введите имя файла, содержащего ключ: ");
        tableshiftDec(infile, outfile, keyfile);
        cout << "Файл успешно расшифрован. Результат сохранён в: " << outfile << endl;
    } 
    else if (algo == FIXEDSHIFT) {
        int n;
        cout << "Введите размер перестановки: ";
        cin >> n;
        vector<int> perm(n);
        cout << "Введите перестановку через пробел: ";
        for (int i = 0; i < n; ++i) cin >> perm[i];
        cin.ignore();
        if (!isValidPermutation(perm)) {
            cerr << "Неверная перестановка!\n";
            return;
        }
        fixedshiftDec(infile, outfile, perm);
        cout << "Файл успешно расшифрован. Результат сохранён в: " << outfile << endl;
    } 
    else {
        cerr << "Неизвестный алгоритм!\n";
    }
}
