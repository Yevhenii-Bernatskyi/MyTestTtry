#include <math.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

// Визначення "чарівних" констант
#define P32 0xB7E15163
#define Q32 0x9E3779B9

// Забороняється змінювати сигнатуру функції Task1!  

// Функція циклічного зсуву вліво
// x - число, яке підлягає зсуву
// n - кількість розрядів, на яку відбувається зсув
unsigned int ROL(unsigned int x, unsigned int n) {	    
     return (x << n) | (x >> (32 - n));
}

// Функція циклічного зсуву вправо
// x - число, яке підлягає зсуву
// n - кількість розрядів, на яку відбувається зсув
unsigned int ROR(unsigned int x, unsigned int n) {	
   return (x >> n) | (x << (32 - n));
}

// Функція генерування розпорядку ключів
// K - ключ користувача
// S - масив раундових ключів (функція здійснює його заповнення)
void RC5_Key_Shedule(unsigned int* K, unsigned int* S) {	
    int b = 16;
    int u = 4;
    int c = b / u;
    unsigned int L[c];

    for (int i = 0; i < c; i++) {
        L[i] = 0;
    }
    
    for (int i = b - 1; i >= 0; i--) {
        L[i / u] = (L[i / u] << 8) + ((unsigned char*)K)[i];
    }

    S[0] = P32;
    for (int i = 1; i < 26; i++) {
        S[i] = S[i - 1] + Q32;
    }

    unsigned int A = 0, B = 0;
    int i = 0, j = 0;
    for (int k = 0; k < 3 * max(c, 26); k++) {
        A = S[i] = ROL(S[i] + A + B, 3);
        B = L[j] = ROL(L[j] + A + B, (A + B) & 31);
        i = (i + 1) % 26;
        j = (j + 1) % c;
    }
}

// Функція шифрування RC5
// M - масив чисел повідомлення
// C - масив чисел шифру (функція здійснює його заповнення)
// S - масив раундових ключів (попередньо заповнений функцією RC5_Key_Shedule)
void RC5_Encrypt(unsigned int* M, unsigned int* C, unsigned int* S) {	
    unsigned int A = M[0] + S[0];
    unsigned int B = M[1] + S[1];
    
    for (int i = 1; i <= 12; i++) {
        A = ROL(A ^ B, B & 31) + S[2 * i];
        B = ROL(B ^ A, A & 31) + S[2 * i + 1];
    }

    C[0] = A;
    C[1] = B;
}

// Функція дешифрування RC5
// Функція шифрування RC5
// C - масив чисел шифру
// M - масив чисел повідомлення (функція здійснює його заповнення)
// S - масив раундових ключів (попередньо заповнений функцією RC5_Key_Shedule)
void RC5_Decrypt(unsigned int* C, unsigned int* M, unsigned int* S) {
    unsigned int A = C[0];
    unsigned int B = C[1];

    for (int i = 12; i > 0; i--) {
        B = ROR(B - S[2 * i + 1], A & 31) ^ A;
        A = ROR(A - S[2 * i], B & 31) ^ B;
    }

    M[0] = A - S[0];
    M[1] = B - S[1];
}


// Завдання 1
// 
// Функція приймає у якості аргументів:
// source - масив джерела інформації (масив шістнадцяткових чисел), 
// sourceSize - кількість елементів масиву джерела інформації, 
// key - масив ключа (масив шістнадцяткових чисел), 
// encryptionMode - передається true у випадку, коли необхідно здійснити 
//                  шифрування елементів масиву джерела інформації за 
//                  алгоритмом RC5 і false - коли необхідно здійснити 
//                  дешифрування.
// 
// Функція повинна повертати рядок, який містить вивід результату застосування 
// процедури шифрування (дешифрування) за алгоритмом RC5 до масиву джерела 
// інформації (послідовність розділених пробілами шістнадцяткових чисел без 
// префіксу "0x" із великими шістнадцятковими літерами). 
// Наприклад (масиви M2 та K2 були визначені попередньо):
// Task1("M2,  4, K2, true") - "90423F35 F41D46C7 3A06D9F2 81823FEE" (без лапок)
string Task1 (unsigned int* source, unsigned int sourceSize, unsigned int* key, bool encryptionMode) {
    // Об'єктом stringstream можна користуватись як і об'єктом iostream
    // Наприклад, коректним є запис
    // functionOutput << arr[i] << " ";
    stringstream functionOutput;
    
    unsigned int S[26];
    RC5_Key_Shedule(key, S);
    
    // Вивід результатів
    for (unsigned int i = 0; i < sourceSize; i += 2) {
        unsigned int block[2] = { source[i], source[i + 1] };
        unsigned int result[2];
        
        if (encryptionMode) {
            RC5_Encrypt(block, result, S);
        } else {
            RC5_Decrypt(block, result, S);
        }
        
        functionOutput << uppercase << hex << setfill('0') << setw(8) << result[0] << " " << setw(8) << result[1];
        if (i + 2 < sourceSize) {
            functionOutput << " ";
        }
    }

    // Розмістіть тут Ваш код

    // Конвертування об'єкту stringstream у рядок для відповідності сигнатурі функції
    return functionOutput.str();
}
