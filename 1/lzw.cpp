#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

#define DICTIONARY_SIZE 256

using namespace std;

// Function to convert decimal number to binary
string DecToBin(int number, int dictionarySize) {
    string result = "";

    do {
        if ((number & 1) == 0)
            result += "0";
        else
            result += "1";

        number >>= 1;
    } while (number);

    int cost = ceil(log2(dictionarySize));
    if (result.length() < cost) {
        for (int i = result.length(); i < cost; i++) {
            result += "0";
        }
    };

    reverse(result.begin(), result.end());
    return result;
}

// Function for reading files and copying information to string
void ReadFile(const string filename, string& input) {
    ifstream inputFile(filename);

    // Proccessing openning file error
    if (!inputFile) {
        cerr << "Unable to open file " << filename << endl;
        exit(-1);
    }

    string temp;
    while (getline(inputFile, temp)) {
        input += temp;
        input.push_back('\n');
    }
    input.pop_back();

    inputFile.close();
}

// Function for filling in information from vector to binary file
void FillFile(const string filename, vector<int> output_code, string bits) {
    ofstream outputFile(filename, ios::out | ios::binary);
    ofstream outFile("output_codes.txt");
    if (!outputFile) {
        cerr << "Unable to open output file" << filename << endl;
        exit(-1);
    }

    unsigned char byte = 0; // Переменная для накопления байтов
    int bitCount = 0;       // Счётчик накопленных битов

    for (size_t i = 0; i < bits.size(); ++i) {
        // Заполняем байт битами
        byte = (byte << 1) | (bits[i] & 1); // Добавляем бит в младший разряд
        bitCount++;

        // Если заполнили байт, записываем его в файл
        if (bitCount == 8) {
            outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
            byte = 0; // Сброс накопленного байта
            bitCount = 0; // Сброс счётчика
        }
    }

    // Если есть незаписанные биты, заполняем их нулями и записываем последний байт
    if (bitCount > 0) {
        byte <<= (8 - bitCount); // Сдвигаем влево, чтобы заполнить оставшиеся биты нулями
        outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    cout << "Output Codes are:\n";

    for (int code : output_code) {
        cout << code << " "; // Print codes in console
        outFile << code << " ";
    }

    // Закрываем файл
    outFile.close();
    outputFile.close();
}

// Function for reading files and copying information into a vector
void ReadFile(const string filename, vector<int>& input) {
    ifstream inputFile(filename);

    // Proccessing openning file error
    if (!inputFile) {
        cerr << "Unable to open file " << filename << endl;
        exit(-1);
    }

    copy(istream_iterator<int>(inputFile), istream_iterator<int>(), back_inserter(input));

    inputFile.close();
}

// Function for filling in information from string in file
void FillFile(const string filename, string result) {
    ofstream decodedFile(filename);
    if (!decodedFile) {
        cerr << "Unable to open output file" << filename << endl;
        exit(-1);
    }
    decodedFile << result;
    decodedFile.close();
}

// Function for printing dictionary contents
void PrintDictionary(unordered_map<string, int> dictionary) {
    cout << "\nDictionary contents:\nString\tCode\n";
    for (auto& pair : dictionary) {
        cout << " " << pair.first << "\t" << " " << pair.second << "\n";
    }
    cout << endl;
}

// Function for converting unordered_map
unordered_map<string, int> convertMap(const unordered_map<int, string>& original) {
    unordered_map<string, int> converted;

    for (const auto& pair : original) {
        converted[pair.second] = pair.first;
    }

    return converted;
}

// Function to encode input string
unordered_map<string, int> encoding(const string inputFilename, const string outputFilename) {
    string input;
    ReadFile(inputFilename, input);

    // Dictionary, which is a associated container that stores elements formed 
    // by the combination of a key value and a mapped value.
    unordered_map<string, int> dictionary;

    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        dictionary[string(1, char(i))] = i; // Initialize table with single character strings
    }

    // current - first input code
    string current = "", next = "";
    current += input[0];
    int code = DICTIONARY_SIZE;
    vector<int> output_code;
    string bits = "";
    int index = 0;

    cout << "String\tOutput_Code\tAddition\n";

    for (int i = 0; i < input.length(); i++) { // While not end of input stream
        if (i != input.length() - 1) {
            next += input[i + 1]; // next - next input character 
        }

        if (dictionary.find(current + next) != dictionary.end()) { // if current + next is in the string table
            current = current + next; // update string current to current + next
        }
        else {
            cout << current << "\t" << dictionary[current] << "\t\t"
                << current + next << "\t" << code << endl;

            output_code.push_back(dictionary[current]); // add current + next to the string table
            bits += DecToBin(output_code[index], code);
            dictionary[current + next] = code; // add new string in the dictionary
            code++;
            index++;
            current = next; // current is next character
        }

        next = "";
    }

    if (!current.empty()) {
        output_code.push_back(dictionary[current]);
    }

    FillFile(outputFilename, output_code, bits);

    return dictionary;
}

// Function to decode input string
unordered_map<string, int> decoding(const string inputFilename, const string outputFilename) {
    vector<int> input;
    ReadFile(inputFilename, input);

    // Dictionary, which is a associated container that stores elements formed 
    // by the combination of a key value and a mapped value.
    unordered_map<int, string> dictionary;

    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        string ch = "";
        ch += char(i);
        dictionary[i] = ch; // Initialize table with single character strings
    }

    // old - first input code
    int old = input[0], next;
    string str = dictionary[old];
    string result = str; // string to save decoding result
    string c = "";
    c += str[0];

    cout << "Decoded message:\n" << str; // output translation of old

    int count = DICTIONARY_SIZE;
    for (int i = 0; i < input.size() - 1; i++) { // while not end of input stream
        next = input[i + 1]; // next input code

        if (dictionary.find(next) == dictionary.end()) { // if next is not in the string table
            str = dictionary[old]; // translation of old
            str = str + c;
        }
        else {
            str = dictionary[next]; // translation of next
        }

        cout << str;
        result += str; // save decoding result
        c = "";
        c += str[0]; // c is a first character of str

        dictionary[count] = dictionary[old] + c;
        count++;
        old = next;
    }

    FillFile(outputFilename, result);

    unordered_map<string, int> dict = convertMap(dictionary);

    return dict;
}

int main(void) {
    cout << "Choose the option:\n1. To encode\n2. To decode\n";
    int option;
    cin >> option;

    unordered_map<string, int> dictionary;

    switch (option)
    {
    case 1:
        dictionary = encoding("input.txt", "output_codes.bin");
        cout << "\nEncoding complete" << endl;
        break;
    case 2:
        dictionary = decoding("output_codes.txt", "decoded_output.txt");
        cout << "\nDecoding complete" << endl;
        break;
    default:
        cout << "Incorrect option" << endl;
        return 0;
    }

    cout << "\nWanna print the dictionary?\n1. Yes\n2. No" << endl;
    cin >> option;
    switch (option)
    {
    case 1:
        PrintDictionary(dictionary);
        break;
    case 2:
        cout << "Goodbye" << endl;
        break;
    default:
        cout << "Incorrect option" << endl;
        return 0;
    }

    return 0;
}