#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){
    cout << 10000 << " " << 2000 << endl;
    srand(time(0));
    for(int i=0; i<100000; ++i){
        cout << "assign " << rand()%50 << " " << i << " " << rand() % 1000 + 1  << endl;
    }
    for(int i=0; i<20; ++i){
        cout << "merge " << rand()%50+50 << " " << rand()%70 << endl;
    }
    for(int i=0; i<10000; ++i){
        cout << "execute " << rand()%70 << endl;
    }
}
