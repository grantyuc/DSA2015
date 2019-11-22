#include <iostream>

int main(){
    char* n = new char[5];
    n[2] = 65;
    for(int i = 0; i<5; ++i){
        std::cout << n[i] << std::endl;
    }
    delete[] n;
}
