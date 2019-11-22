#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

int hashCode(const int& n, const int& mod = 164){
    return (n % mod + mod) % mod;
}
int hashCode(const char* str, const int& len, const int& mod = 164){
    unsigned int hash = 0;
    for(int i = 0; i < len; ++i){
        hash = (hash<<5)^(hash>>27);
        hash += (unsigned int)str[i];
    }
    return hashCode(int(hash), mod);
}

int main(){
    char* cmd[32];
    int count = 0;
    string istring;
    //read in
    while(getline(cin, istring)){
        istringstream sin(istring);
        cmd[count] = new char[10];
        sin >> cmd[count];
        ++count;
    }
    //find smallest safe mod
    int hasharr[32];
    int smod = 0;
    int round = 1;
    cout << "\nround " << round << endl;
    for(int i=32; i<300; ++i){
        bool collision = false;
        bool repeat[3000] = {false};
        for(int j=0; j<32; ++j){
            int hash = hashCode(cmd[j], strlen(cmd[j]), i);

            if(repeat[hash]){
                //cout << "collide when mod " << i << endl;
                collision = true;
                break;
            } 
            repeat[hash] = true;
        }
        if(!collision) {
            cout << "safe when mod " << i << endl;
            smod = i;
            break;
        }
    }

    for(int j=0; j<32; j++){
        int hash = hashCode(cmd[j], strlen(cmd[j]), smod);
        hasharr[j] = hash;
        cout << "hashCode( " << cmd[j] << " ) \t= " << hash << endl;
    }

    //find next smallest safe mod
    int lastSmod = 0;
    while(true){
        smod = 0;
        for(int i=32; i<300; ++i){
            bool collision = false;
            bool repeat[300] = {false};
            for(int j=0; j<32; ++j){
                int hash = hashCode(hasharr[j], i);

                if(repeat[hash]){
                    //cout << "collide when mod " << i << endl;
                    collision = true;
                    break;
                } 
                repeat[hash] = true;
            }
            if(!collision) {
                smod = i;
                break;
            }
        }

        if(lastSmod == smod){
            int moveStep = 150;
            bool used[150] = {false};
            for(int j=0; j<32; ++j){
                used[hasharr[j]] = true;
            }
            for(int i=0; i<smod; ++i){
                if(!used[i] && smod-i-1 < moveStep)
                    moveStep = smod-i-1;
            }
            for(int j=0; j<32;++j){
                hasharr[j] += moveStep;
                hasharr[j] %= smod;
            }
            cout << "moveStep " << moveStep <<endl;
        }
        else{
            ++round;
            cout << "\nround " << round << endl;
            cout << "safe when mod " << smod << endl;
            for(int j=0; j<32; j++){
                int hash = hashCode(hasharr[j], smod);
                hasharr[j] = hash;
                cout << "hashCode( " << cmd[j] << " ) \t= " << hash << endl;
            }
        }
        if(smod <= 32) break;
        lastSmod = smod;
    }
    //count letters
    //int countAbc[26] = {0};
    //for(int j=0; j<32; ++j){
    //    int letter = cmd[j][0] - 'a';
    //    ++countAbc[letter];
    //    letter = cmd[j][strlen(cmd[j])-1] - 'a';
    //    ++countAbc[letter];
    //}
    //for(int i=0; i<26; ++i){
    //    cout << char('a'+i) << " : " << countAbc[i] <<endl;
    //}
    //for(int j=0; j<32; ++j){
    //    cout << cmd[j] << "\t: " << countAbc[cmd[j][0] - 'a'] + countAbc[ cmd[j][strlen(cmd[j])-1] - 'a' ] << endl;
    //}
}
