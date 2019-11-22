#ifndef __USER_H__
#define __USER_H__

#include "md5.h"
#include <cmath>
#include <map>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int maxLenDiffWithScore(const int&);

class CompareStringLength{
    public:
        bool operator() (const string& id1, const string& id2){
            if(id1.length() != id2.length()) 
                return id1.length() < id2.length();
            else return id1 < id2;
        }
};
//*****************try to use struct <IdTimePair, History*>***************************
//class IdTimePair{
//    public:
//        IdTimePair(string* const i, const unsigned int& t) : id(i), timeStamp(t){}
//
//        bool operator<(const IdTimePair& itp){
//            if(*id != *itp.id) 
//                return *id < *itp.id;
//            else return timeStamp < itp.timeStamp;
//        }
//
//        string* id;
//        unsigned int timeStamp;
//};
//
//class CompareITPair{
//    public:
//        bool operator() (const IdTimePair& a, const IdTimePair& b){
//            if(*a.id != *b.id) 
//                return *a.id < *b.id;
//            else return a.timeStamp < b.timeStamp; 
//        }
//};
//
//************************************************************************************
class History{
    public:
        History();
        History(const string& i1, const string& i2, const int& mon) : id1(i1), id2(i2), money(mon), flag(0) {++time; timeStamp = time;}

        // id1 transfer money to id2 at timeStamp
        string id1;
        string id2;
        int money;
        int timeStamp;   //as key

        int flag; // 0: init, id1 != id2;  1: To than From  -1: From then To

        static unsigned int time;

        bool id1Is(const string& id){return id == id1;}
        bool id2Is(const string& id){return id == id2;}
};

class User{
    public:
        User();
        User(const string& i, const string& pw) : id(i), money(0) {hashpw = md5(pw); usermp[id] = this;}
                                                                                //or usermp.insert( pair<string, User*>(i, this) )
        ~User(){remove();}

        int getMoney() const {return money;}
        string getId() const {return id;}
        bool isCorrectPw  (const string& pw) const {return hashpw == md5(pw);}

        void deposit(const int& num){money += num;}
        void withdraw(const int& num){money -= num;}

        void remove(){
            for(auto it = historymp.begin(); it != historymp.end(); ++it){
                it->second.clear();
            }
            historymp.clear();
            usermp.erase(this->id);
        }
        void transferTo(User&, const int&);
        void merge(User&);
        void printHistory();
        void printHistory(string);


        static map<string, User* /*, CompareStringLength*/ > usermp;  //map id to the user
        static void recommend(const string&);
        static void create_suggest(const string&);

    private:
        string id;
        string hashpw;                                          //hashed password
        int money;
        map<string, map<unsigned int, History*> > historymp;    //map dealer id to the map mapping timeStamp to the history
};

#endif
