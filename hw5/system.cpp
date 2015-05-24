#include "BinomialHeap.h"
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
    int c;
    double w;
    string istring;
    cin >> c >> w;
    cin.ignore(256, '\n');
    BinomialHeap<Task>* bh = new BinomialHeap<Task>[c];
    while(getline(cin, istring)){
        istringstream sin(istring);
        char cmd[10] = {'\0'};
        sin >> cmd;

        int cm = 0, cm2 = 0, id = 0;
        double p = 0.;

        switch(hashCode(cmd, strlen(cmd))){
            //assign
            case 19043:{
                sin >> cm >> id >> p;
                if(!sin) {
                    cout << "-> assign cm id p" << endl; 
                    break;
                }
                Task t(id, p);
                bh[cm].insert(t);
                cout << "There are " << bh[cm].getSize() << " tasks on computer " << cm << "." << endl;
                //cout << bh[cm] << endl;
                break;
            }
            //execute
            case 13682:{
                sin >> cm;
                if(!sin) {
                    cout << "-> execute cm" << endl; 
                    break;
                }
                if(bh[cm].getSize() != 0){
                    Task priorTask = bh[cm].pop();
                    cout << "Computer " << cm << " executed task " << priorTask.getId() << "." << endl;
                    while(bh[cm].getSize() != 0){
                        Task next = bh[cm].top();
                        if(priorTask == next){          //operator== compare priority only
                            bh[cm].pop();
                            cout << "Computer " << cm << " executed task " << next.getId() << "." << endl;
                        }
                        else break;
                    }
                    //cout << bh[cm] << endl;
                }
                else cout << "No more task on this computer." << endl;
                break;
            }

            //merge
            case 8382:{
                sin >> cm >> cm2;
                if(!sin) {
                    cout << "-> merge cm1 cm2" << endl; 
                    break;
                }
                if(bh[cm2].getSize() >= w){
                    bh[cm].merge(bh[cm2]);
                    Task priorTask = bh[cm].top();
                    cout << "The largest priority number is now " << priorTask.getP() << " on " << cm << "." << endl;
                    //cout << bh[cm] << endl;
                }
                else{
                    cout << "Merging request failed." << endl;
                }
                break;
            }

            //print
            case 13399:{
                sin >> cm;   
                if(!sin) {
                    cout << "-> print cm" << endl; 
                    break;
                }
                cout << bh[cm] << endl;
                break;
            }

            default:
                cout << "command error! hashCode = " << hashCode(cmd, strlen(cmd)) << endl;
                break;
        }
        //cout << bh[cm] << endl;
    }
}
