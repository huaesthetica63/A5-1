#include "a51.h"
using namespace std;
int main(){//код для демонстрации работы алгоритма шифрования
    srand(time(0));//инициализация рандомайзера
    long long sizeFile = getSizeFile((char*)"sample.pdf");//получаем размер в байтах исходного файла
    if(sizeFile==-1){
        cout<<"Error!"<<endl;
        return -1;
    }
    cout<<"Size of file: "<<sizeFile<<" bytes"<<endl;
    generateKey((char*)"key.bin", 8);//формируем ключ длиной 8 байт
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();//замеряем время для шифрования
    bool resOperation=A51((char*)"sample.pdf", (char*)"res.pdf", (char*)"key.bin");
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    if(resOperation)
        cout << "Time of encryption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
    else{
        cout<<"Error!"<<endl;
        return -1;
    }
    begin = chrono::steady_clock::now();//замеряем время для дешифрования
    resOperation=A51((char*)"res.pdf", (char*)"deciph_sample.pdf", (char*)"key.bin");
    end = chrono::steady_clock::now();
    if(resOperation)
        cout << "Time of decryption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
    else{
        cout<<"Error!"<<endl;
        return -1;
    }
    return 0;
}
