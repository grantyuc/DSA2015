#include "User.h"
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
    while(getline(cin, istring)){
        istringstream sin(istring);
        char cmd[10] = {'\0'};
        sin >> cmd;

        string id, id2, pw, pw2;
        int num;

        switch(hashCode(cmd, strlen(cmd))){
            //login
            case 4329:{
                sin >> id >> pw;
                if(!sin) {
                    cout << "-> login id pw" << endl; 
                    break;
                }
                if(/*no such account*/1) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                if(/*pw error*/1) {
                    cout << "wrong password" << endl;
                    break;
                }
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
                if(/*id exsits*/1){
                    cout << "ID " << id << " exists, ";
                    //print 10 best unused id
                    break;
                }
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
                if(/*no such account*/1) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                if(/*pw error*/1) {
                    cout << "wrong password" << endl;
                    break;
                }
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
                if(/*no such account id1*/1) {
                    cout << "ID " << id << " not found" << endl;
                    break;
                }
                if(/*no such account id2*/1) {
                    cout << "ID " << id2 << " not found" << endl;
                    break;
                }
                if(/*pw1 error*/1) {
                    cout << "wrong password1" << endl;
                    break;
                }
                if(/*pw2 error*/1) {
                    cout << "wrong password2" << endl;
                    break;
                }
                cout << "success, " << id << " has " << "[X] dollars" << endl;
                break;
            }
            //deposit
            case 4961:{
                sin >> num;
                if(!sin) {
                    cout << "-> deposit num" << endl; 
                    break;
                }
                cout << "success, " << "[X] dollars in current account" << endl;
                break;
            }
            //withdraw
            case 9533:{
                sin >> num;
                if(!sin) {
                    cout << "-> withdraw num" << endl; 
                    break;
                }
                if(/*id.getMoney < num*/1){
                    cout << "fail, " << "[X] dollars only in current account" << endl;
                }
                else{
                    cout << "success, " << "[X] dollars left in current account" << endl;
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
                if(/*no such account id*/1) {
                    cout << "ID " << id << " not found, ";
                    //print best 10 existing ids
                    break;
                }
                if(/*id.getMoney < num*/1){
                    cout << "fail, " << "[X] dollars only in current account" << endl;
                }
                else{
                    cout << "success, " << "[X] dollars left in current account" << endl;
                }
                break;
            }
            //find
            case 3596:{
                sin >> id;
                if(!sin) {
                    cout << "-> find id" << endl; 
                    break;
                }
                //print all satisfying ids
                break;
            }
            //search
            case 9701:{
                sin >> id;
                if(!sin) {
                    cout << "-> search id" << endl; 
                    break;
                }
                //id.printHistory();
                break;
            }

            //print
            case 13399:{
                sin >> id;   
                if(!sin) {
                    cout << "-> print cm" << endl; 
                    break;
                }
                break;
            }

            default:
                cout << "command error! hashCode = " << hashCode(cmd, strlen(cmd)) << endl;
                break;
        }
    }
}
