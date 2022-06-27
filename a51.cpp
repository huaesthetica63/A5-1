#include "a51.h"
using namespace std;
bool A51(char*filename1,char*filename2,char*key){//поточный шифр А5/1
    ifstream file(filename1, ios::binary|ios::in);//исходный файл
    ofstream cipherfile(filename2,ios::binary|ios::out);//файл-результат (шифротекст)
    ifstream keyfile(key, ios::binary|ios::in);//файл с ключом
    if((!file.is_open())||(!cipherfile.is_open())||(!keyfile.is_open()))
        return false;
    //x,y,z-линейные регистры сдвига с обратной связью
    //их размеры определяются алгоритмом как 19, 22 и 23 бит соответственно
    vector<unsigned char> x(19);
    vector<unsigned char> y(22);
    vector<unsigned char> z(23);
    char keybyte;
    vector<unsigned char>keystr(64);//массив на 64 бита ключа
    int j=0;
    while(keyfile.read(&keybyte, sizeof(char))){
        for(int i=0;i<8;i++){//считываем 8 байт ключа
            keystr[j++] = (keybyte>>i)&1;//затем каждый бит (8 битов в каждом байте) записываем в отдельные ячейки массива
        }
    }
    for(int i=0;i<19;i++){
        x[i] = keystr[i];//первые 19 бит из ключа переносим в первый регистр
    }
    for(int i=0;i<22;i++){
        y[i] = keystr[19+i];//следующие 22 бита - во второй
    }
    for(int i=0;i<23;i++){
        z[i] = keystr[19+22+i];//оставшиеся биты в третий регистр
    }
    char byte;
    while(file.read(&byte,sizeof(char))){
        unsigned char majorbit;//преобладающий среди синхронизационных бит (единица или ноль)
        if(x[8]+y[10]+z[10]>1)//определяем преобладающий бит среди битов синхронизации (1 или 0)
            majorbit=1;
        else
            majorbit=0;
        if(x[8] == majorbit){//если синхронизационный бит совпал с majorbit - сдвигаем первый регистр
            unsigned char bit = x[13]^x[16]^x[17]^x[18];//получаем бит через многочлен обратной связи первого регистра
            for(int i=18;i>0;i--)//сдвигаем биты регистра вправо
                x[i]=x[i-1];
            x[0] = bit;//на освободившемся месте располагаем новый бит
        }
        if(y[10] == majorbit){//если синхронизационный бит совпал с majorbit - сдвигаем второй регистр
            unsigned char bit = y[20]^y[21];//получаем бит через многочлен обратной связи второго регистра
            for(int i=21;i>0;i--)//сдвигаем биты регистра вправо
                y[i]=y[i-1];
            y[0] = bit;//на освободившемся месте располагаем новый бит
        }
        if(z[10] == majorbit){//если синхронизационный бит совпал с majorbit - сдвигаем третий регистр
            unsigned char bit = z[7]^z[20]^z[21]^z[22];//получаем бит через многочлен обратной связи третьего регистра
            for(int i=22;i>0;i--)//сдвигаем биты регистра вправо
                z[i]=z[i-1];
            z[0] = bit;//на освободившемся месте располагаем новый бит
        }
        //в итоге регистры на каждую итерацию сдвигаются неравномерно, что делает криптосистему более случайной
        char resbyte = byte^x[18]^y[21]^z[22];
        cipherfile.write(&resbyte, sizeof(char));//записываем результат в шифротекст
    }
    file.close();
    cipherfile.close();
    keyfile.close();
    return true;
}
long long getSizeFile(char*filename){//получаем размер файла в байтах
    ifstream file(filename, ios::binary);
    file.seekg(0,file.end);//ставим указатель в конец
    long long res = file.tellg();//получаем размер
    file.seekg(0,file.beg);//переводим указатель в начало
    file.close();
    return res;
}
void printBinaryFile(char*filename){//вывод бинарного файла в виде последовательности битов
    ifstream file(filename, ios::binary);
    char byte;
    while(file.read(&byte,sizeof(char))){
        cout<<bitset<8>(byte)<<" ";
    }
    file.close();
}
void generateKey(char*filename, long long size){//генерируем случайный ключ заданной длины и записываем его в файл
    ofstream keyfile(filename,ios::binary|ios::out);
    while(size>0){
        char byte = rand()%256;
        keyfile.write(&byte, sizeof(char));
        size--;
    }
    keyfile.close();
}
