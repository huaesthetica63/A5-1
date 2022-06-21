#ifndef a51_h
#define a51_h
#include <iostream>
#include <fstream>
#include <chrono>//замер времени выполнения куска кода
#include <vector>
//прототипы используемых функций
bool A51(char*filename1,char*filename2,char*key);//алгоритм а5/1
long long getSizeFile(char*filename);//получить размер файла в байтах
void printBinaryFile(char*filename);//печать файла в виде последовательности нулей и единиц
void generateKey(char*filename, long long size);//сгенерировать случайный ключ заданной длины
#endif
