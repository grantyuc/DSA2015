#include "md5.h"
#include <cmath>
#include <map>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int maxLenDiffWithScore(const int& score){
    return floor(-0.5 + sqrt(2*score+0.25));
}

class CompareStringLength{
    public:
        bool operator() (const string& id1, const string& id2){
            if(id1.length() != id2.length()) 
                return id1.length() < id2.length();
            else return id1 < id2;
        }
};

class History{
    public:
        History();
        History(const string& i1, const string& i2, const int& mon) : id1(i1), id2(i2), money(mon) {++time; timeStamp = time;}

        // id1 transfer money to id2 at timeStamp
        string id1;
        string id2;
        int money;
        int timeStamp;   //as key

        static unsigned int time;

        bool id1Is(const string& id){return id == id1;}
        bool id2Is(const string& id){return id == id2;}
};
unsigned int History::time = 0;

class User{
    public:
        User();
        User(const string& i, const string& pw) : id(i), money(0) {hashpw = md5(pw); usermp[id] = this;}
                                                                                //or usermp.insert( pair<string, User*>(i, this) )
        ~User(){remove();}

        int getMoney(){return money;}
        string getId(){return id;}
        bool isCorrectPw(const string& pw){return hashpw == md5(pw);}

        void deposit(const int& num){money += num;}
        void withdraw(const int& num){money -= num;}

        void remove(){historymp.clear(); usermp.erase(this->id);}
        void transferTo(const User& u, const int& money);
        void merge(const User& u);
        void printHistory();

        static map<string, User*, CompareStringLength> usermp;  //map id to the user

    private:
        string id;
        string hashpw;                                          //hashed password
        int money;
        map<unsigned int, History*> historymp;                  //map timeStamp to the history
};
