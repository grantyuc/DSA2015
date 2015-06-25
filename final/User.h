#include "md5.h"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int maxLenDiffWithScore(const int& score){
    return floor(-0.5 + sqrt(2*score+0.25));
}

class CompareStringLength{
    bool operator() (const string& id1, const string& id2){
        if(id1.length() != id2.length()) 
            return id1.length() < id2.length();
        else return id1 < id2;
    }
};

class History{
    public:
        History();
        History(const bool& to, const string& i1, const string& i2, const int& mon) : transferTo(to), id1(i1), id2(i2), money(mon) {timeStamp = time(0);}

        bool transferTo;    //id1 transfer to or recieve from id2, 1: to, 0: from
        int money;
        string id1;
        string id2;
        time_t timeStamp;   //as key
};

class User{
    public:
        User();
        User(const string& i, const string& pw) : id(i), money(0), isDeleted(false) {hashpw = md5(pw);}

        int getMoney(){return money;}
        bool isCorrectPw(const string& pw){return hashpw == md5(pw);}

        int deposit(const int& num){money += num; return money;}
        int withdraw(const int& num){money -= num; return money;}

        void remove();
        void merge(const User& u);
        void printHistory();

    private:
        string id;
        string hashpw;           //hashed
        int money;
        bool isDeleted;          //or del directly
        vector<History> history; //history heap
};
