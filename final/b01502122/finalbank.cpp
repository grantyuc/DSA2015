#include "User.h"
#include "findID.h"
#include <cstring>
#include <sstream>

using namespace std;

int hashCode(const int& n){
    return abs((n%11541*5 + n%52361) % 20000);
}
int hashCode(const char* str, const int& len){
    unsigned int hash = 0;
    for(int i = 0; i < len; ++i){
        hash = (hash<<5)^(hash>>27);
        hash += (unsigned int)str[i];
    }
    return hashCode(int(hash));
}

int main(){
    string istring;
    User* curUser = nullptr;
    while(getline(cin, istring)){
        istringstream sin(istring);
        char cmd[10] = {'\0'};
        sin >> cmd;

        string id, id2, pw, pw2;
        int num;

        auto &ump = User::usermp;

        switch(hashCode(cmd, strlen(cmd))){
            //login
            case 4329:{
                sin >> id >> pw;
                if(!sin) {
                    cout << "-> login id pw" << endl; 
                    break;
                }
                auto it = ump.find(id);
                if(it == ump.end()) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                User* temUser = it->second;
                if(!temUser->isCorrectPw(pw)) {
                    cout << "wrong password" << endl;
                    break;
                }
                curUser = temUser;
                cout << "success" << endl;
                break;
            }
            //create
            case 90:{
                sin >> id >> pw;
                if(!sin) {
                    cout << "-> create id pw" << endl; 
                    break;
                }
                auto it = ump.find(id);
                if(it != ump.end()) {
                    cout << "ID " << id << " exists, ";
                    //TODO print 10 best unused id
                    User::create_suggest(id);
                    break;
                }
                new User(id, pw);
                cout << "success" << endl;
                break;
            }
            //delete
            case 13509:{
                sin >> id >> pw;
                if(!sin) {
                    cout << "-> delete id pw" << endl; 
                    break;
                }
                auto it = ump.find(id);
                if(it == ump.end()) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                User* temUser = it->second;
                if(!temUser->isCorrectPw(pw)) {
                    cout << "wrong password" << endl;
                    break;
                }
                delete temUser;
                cout << "success" << endl;
                break;
            }
            //merge
            case 8382:{
                sin >> id >> pw >> id2 >> pw2;
                if(!sin) {
                    cout << "-> merge id1 pw1 id2 pw2" << endl; 
                    break;
                }
                auto it = ump.find(id);
                if(it == ump.end()) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                auto it2 = ump.find(id2);
                if(it2 == ump.end()) {
                    cout << "ID " << id2 << " not found" << endl;
                    break;
                }
                User* temUser = it->second;
                if(!temUser->isCorrectPw(pw)) {
                    cout << "wrong password1" << endl;
                    break;
                }
                User* temUser2 = it2->second;
                if(!temUser2->isCorrectPw(pw2)) {
                    cout << "wrong password2" << endl;
                    break;
                }
                temUser->merge(*temUser2);
                cout << "success, " << id << " has " << temUser->getMoney() << " dollars" << endl;
                break;
            }
            //deposit
            case 4961:{
                sin >> num;
                if(!sin) {
                    cout << "-> deposit num" << endl; 
                    break;
                }
                curUser->deposit(num);
                cout << "success, " << curUser->getMoney() << " dollars in current account" << endl;
                break;
            }
            //withdraw
            case 9533:{
                sin >> num;
                if(!sin) {
                    cout << "-> withdraw num" << endl; 
                    break;
                }
                if(curUser->getMoney() < num){
                    cout << "fail, " << curUser->getMoney() << " dollars only in current account" << endl;
                }
                else{
                    curUser->withdraw(num);
                    cout << "success, " << curUser->getMoney() << " dollars left in current account" << endl;
                }
                break;
            }
            //transfer
            case 14888:{
                sin >> id >> num;
                if(!sin) {
                    cout << "-> transfer id num" << endl; 
                    break;
                }
                auto it = ump.find(id);
                if(it == ump.end()) {
                    cout << "ID " << id << " not found, ";
                    //TODO print best 10 existing ids
                    User::recommend(id);
                    break;
                }
                if(curUser->getMoney() < num){
                    cout << "fail, " << curUser->getMoney() << " dollars only in current account" << endl;
                }
                else{
                    User* temUser = it->second;
                    if(temUser->getId() == curUser->getId()){
                        cout << "cannot transfer to yourself" << endl;
                        break;
                    }
                    curUser->transferTo(*temUser, num);
                    cout << "success, " << curUser->getMoney() << " dollars left in current account" << endl;
                }
                break;
            }
            //find
            case 3596:{
                cout << endl; break;
                sin >> id;
                if(!sin) {
                    cout << "-> find id" << endl; 
                    break;
                }
                //print all satisfying ids
                id = "\\b" + id + "\\b";
                id = IDreplace(id, "?", "\\S");
                id = IDreplace(id, "*", "[0-9A-Za-z]*");
                string idlist;
                for(auto it = User::usermp.begin(); it != User::usermp.end(); ++it){
                    if(curUser != nullptr && it->first == curUser->getId()) continue;
                    idlist += it->first;
                    idlist += " ";
                }
                regex_find(id, idlist);

                break;
            }
            //search
            case 9701:{
                sin >> id;
                if(!sin) {
                    cout << "-> search id" << endl; 
                    break;
                }
                curUser->printHistory(id);
                break;
            }
            //print
            case 13399:{
                sin >> id;   
                if(!sin) {
                    for(auto it = ump.begin(); it != ump.end(); ++it){
                        cout << it->first << " ";
                    }
                    cout << endl;
                    break;
                }
                auto it = ump.find(id);
                if(it == ump.end()) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                User* temUser = it->second;
                cout << id << " has " << temUser->getMoney() << " dollars. history as follow" << endl;
                temUser->printHistory();
                break;
            }
            //'\n'
            case 0: break;

            default:
                cout << "command error! hashCode = " << hashCode(cmd, strlen(cmd)) << endl;
                break;
        }
    }
}
