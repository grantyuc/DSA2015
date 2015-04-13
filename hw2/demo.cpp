#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

#define N 149639105
#define DATAPOS_WORKSTATION "/tmp2/KDDCup2012/track2/kddcup2012track2.txt"
#define DATAPOS_LOCAL "../sample_data_100000.txt"
//#define DATAPOS_LOCAL "../demotrack.txt"

using namespace std;

vector<int> user_SortedAdID;
bool checkSortedAdID(int userID){
    bool haveSortedAdID = false;
    for(vector<int>::iterator it = user_SortedAdID.begin(); it != user_SortedAdID.end(); ++it){
        haveSortedAdID |= (userID == *it);
    }
    if(!haveSortedAdID){
        user_SortedAdID.push_back(userID);
    }
    return haveSortedAdID;
}

struct AdData {
    //'(Click)\t(Impres)\t(DisplayURL)\t(AdID)\t(Advsr)\t(Depth)\t(Pos)\t(Query)\t(Kword)\t(Title)\t(Dscri)\t(UserID)'
    int* prop[12];

    AdData(){
        prop[0] = new int;
        prop[1] = new int;
        prop[2] = (int*)(new unsigned long long);
        for (int i = 3; i<12; ++i){
            prop[i] = new int;
        }
    }

    ~AdData(){
        for (int i = 0; i<12; ++i){
            delete prop[i];
        }
    }

};

bool readinLine(ifstream& fin, AdData* data){
    //read in one line..
    fin >> *data->prop[0];
    fin >> *data->prop[1];
    fin >> *(unsigned long long*)(data->prop[2]);
    for (int i = 3; i<11; ++i){
        fin >> *data->prop[i];
    }
    bool isSuc = !fin.eof();
    fin >> *data->prop[11];
    return isSuc;
}

bool compUser(AdData* const &a, AdData* const &b){
        return *(a->prop[11]) < *(b->prop[11]);
}

bool compAdID(AdData* const &a, AdData* const &b){
        return *(a->prop[3]) < *(b->prop[3]);
}

bool compQuery(AdData* const &a, AdData* const &b){
        return *(a->prop[7]) < *(b->prop[7]);
}

bool compPos(AdData* const &a, AdData* const &b){
        return *(a->prop[6]) < *(b->prop[6]);
}

bool compDepth(AdData* const &a, AdData* const &b){
        return *(a->prop[5]) < *(b->prop[5]);
}

bool (*compProp[5])(AdData* const&, AdData* const&) = {compUser, compAdID, compQuery, compPos, compDepth};

void get(vector<AdData*>& vpdata, const int& u, const int& a, const int& q, const int& p, const int& d){
    AdData* pwant = new AdData;
    *pwant->prop[11] = u;
    *pwant->prop[3] = a;
    *pwant->prop[7] = q;
    *pwant->prop[6] = p;
    *pwant->prop[5] = d;

    bool haveSortedAdID = checkSortedAdID(u);
    int sortFrom = haveSortedAdID ? 2 : 1;

    pair<vector<AdData*>::iterator, vector<AdData*>::iterator> bounds;
    bounds = equal_range(vpdata.begin(), vpdata.end(), pwant, compUser);


    for (int i = sortFrom; i<5; ++i){
        stable_sort(bounds.first, bounds.second, compProp[i]);
        bounds = equal_range(bounds.first, bounds.second, pwant, compProp[i]);
    }
    int clicksum = 0, impsum = 0;
    for (vector<AdData*>::iterator it = bounds.first; it != bounds.second; ++it){
        clicksum += *(*it)->prop[0];
        impsum += *(*it)->prop[1];
    }
    cout << clicksum << " " << impsum << endl;
    delete pwant;
}

void clicked(vector<AdData*>& vpdata, const int& u){
    AdData* pwant = new AdData;
    *pwant->prop[11] = u;

    checkSortedAdID(u);

    pair<vector<AdData*>::iterator, vector<AdData*>::iterator> bounds;
    bounds = equal_range(vpdata.begin(), vpdata.end(), pwant, compUser);
    stable_sort(bounds.first, bounds.second, compQuery);
    stable_sort(bounds.first, bounds.second, compAdID);
    int adIDtmp = 0, querytmp = 0;
    for (vector<AdData*>::iterator it = bounds.first; it != bounds.second; ++it){
        if (*(*it)->prop[0] >= 1){
          if (adIDtmp == *(*it)->prop[3] && querytmp == *(*it)->prop[7]){
              continue;
          }
          else {
              adIDtmp = *(*it)->prop[3];
              querytmp = *(*it)->prop[7];
              cout << adIDtmp << " " << querytmp << endl;
          }
        }
    }
    delete pwant;
}

void printIter(vector<AdData*>::iterator& it){
    for (int i = 0; i<12; ++i){
        cout << *(*it)->prop[i] << "\t";
    }
    cout << endl;
}

void printAdProp(AdData* const& a){
    cout << "\t" << *(unsigned long long*)(a->prop[2]) << " "
                 << *a->prop[4] << " "
                 << *a->prop[8] << " "
                 << *a->prop[9] << " "
                 << *a->prop[10] << endl;
}

bool diffAdProp(AdData* const &a, AdData* const &b){
        return *(a->prop[10]) != *(b->prop[10]) || *(a->prop[9]) != *(b->prop[9]) || *(a->prop[8]) != *(b->prop[8]) || *(a->prop[4]) != *(b->prop[4]) || *(unsigned long long*)(a->prop[2]) != *(unsigned long long*)(b->prop[2]);
}

void impressed(vector<AdData*>& vpdata, const int& u1, const int& u2){
    AdData* pwant1 = new AdData;
    AdData* pwant2 = new AdData;
    *pwant1->prop[11] = u1;
    *pwant2->prop[11] = u2;

    bool u1haveSortedAdID = checkSortedAdID(u1);
    bool u2haveSortedAdID = checkSortedAdID(u2);

    pair<vector<AdData*>::iterator, vector<AdData*>::iterator> bounds1, bounds2;
    bounds1 = equal_range(vpdata.begin(), vpdata.end(), pwant1, compUser);
    bounds2 = equal_range(vpdata.begin(), vpdata.end(), pwant2, compUser);
    if(!u1haveSortedAdID){
        stable_sort(bounds1.first, bounds1.second, compAdID);
    }
    if(!u2haveSortedAdID){
        stable_sort(bounds2.first, bounds2.second, compAdID);
    }

    vector<AdData*>::iterator it1 = bounds1.first, it2 = bounds2.first;
    vector<AdData*> vpdque;
    int thisAdID = -1;
    while (it1 != bounds1.second && it2 != bounds2.second){
        int AdID1 = *(*it1)->prop[3], AdID2 = *(*it2)->prop[3];
        if (AdID1 > AdID2){
            ++it2;
        }
        else if(AdID1 < AdID2){
            ++it1;
        }
        else {
            if (AdID1 != thisAdID){
                thisAdID = AdID1;
                cout << AdID1 << endl;
                printAdProp(*it1);
                vpdque.clear();
                vpdque.push_back(*it1);
                if (diffAdProp(*it1, *it2)){
                    printAdProp(*it2);
                    vpdque.push_back(*it2);
                }
            }
            else {
                bool it1_isNewAdProp = true, it2_isNewAdProp = true;
                for (unsigned int i = 0; i < vpdque.size(); ++i){
                    it1_isNewAdProp &= diffAdProp(*it1, vpdque.at(i));
                    it2_isNewAdProp &= diffAdProp(*it2, vpdque.at(i));
                }
                if (it1_isNewAdProp){
                    printAdProp(*it1);
                    vpdque.push_back(*it1);
                }
                if (it2_isNewAdProp && diffAdProp(*it1, *it2)){
                    printAdProp(*it2);
                    vpdque.push_back(*it2);
                }
            }
            if(*(*next(it1))->prop[3] == thisAdID){
                ++it1;
            }
            if(*(*next(it2))->prop[3] == thisAdID){
                ++it2;
            }
            if(*(*next(it1))->prop[3] != thisAdID && *(*next(it2))->prop[3] != thisAdID){
                ++it1;
                ++it2;
            }
        }
    }
    delete pwant1;
    delete pwant2;
}
/*

    cout << "thisUser = " << *(*it)->prop[11] <<endl;
    cout << "it : \t";
    printIter(it);
    cout << "next : \t";
    printIter(next);
    cout << endl;
*/

bool equalAdID(AdData* const &a, AdData* const &b){
        return *(a->prop[3]) == *(b->prop[3]);
}

//try to construct the "pred" (a function or an object overloading operator '()') in the find_if function..
//auto AdIDsameAs(AdData* const &a){
//    auto funcptr = [&a](AdData* const &b){return equalAdID(a, b);};
//    return &funcptr;
//}

void profit(vector<AdData*>& vpdata, const int& a, const double& theta){
    AdData* pwant = new AdData;
    *pwant->prop[3] = a;
    vector<AdData*>::iterator it = vpdata.begin(), next = vpdata.begin(), last = vpdata.end();
    auto pred = [&pwant](AdData* const& pdata){return equalAdID(pdata, pwant);};
    next = find_if(next, last, pred);
    while (true){
        if (next == last){
            break;
        }
        else {
            it = next;
            ++next;
        }
        int thisUser = *(*it)->prop[11];
        int clicksum = 0, impsum = 0;
        while (true){
            clicksum += *(*it)->prop[0];
            impsum += *(*it)->prop[1];
            next = find_if(next, last, pred);
            if (next == last){
                break;
            }
            else if (*(*next)->prop[11] != thisUser){
                break;
            }
            else {
                it = next;
                ++next;
            }
        }
        double clkRate = (impsum == 0 ? 0 : double(clicksum)/double(impsum));
        if (clkRate >= theta){
            cout << *(*it)->prop[11] << endl;
        }
    }
    delete pwant;
}

void printstar(){
    cout << "********************" << endl;
}

void printdata(AdData* pdata[], const int& first, const int& last){
    //print n lines of data beginning from *pdata[]
    for (int i = first; i<last; ++i){
        for (int j = 0; j<12; ++j){
            if (j != 2){
                cout << *pdata[i]->prop[j] << '\t';
            }
            else {
                cout << *(unsigned long long*)(pdata[i]->prop[j]) << '\t';
            }
        }
        cout << endl << endl;
    }
}

int main(int argc, char* argv[]){

    time_t begin = time(NULL);

    //open data file..
    ifstream fin;
    fin.sync_with_stdio(false);
    if (argc == 2){
        fin.open(argv[1]);
    }
    else {
        fin.open(DATAPOS_WORKSTATION);
        if (!fin) {
            fin.open(DATAPOS_LOCAL);
        }
        if (!fin) {
            cout << "Can't open data file!" << endl;
            return 0;
        }
    }

    int dataNum;
    AdData** data = new AdData*[N];
    
    time_t readinstart = time(NULL);
    cout << difftime(readinstart, begin) << endl;
    
    for (dataNum = 0; dataNum<N; ++dataNum){
        data[dataNum] = new AdData;
        if(!readinLine(fin, data[dataNum])){
            break;
        }
    }

    //STL vector storing the pointers to data[N]
    vector<AdData*> vpdata;
    for (int i = 0; i<dataNum; ++i){
        vpdata.push_back(data[i]);
    }

    time_t sortstart = time(NULL);
    cout << difftime(sortstart, readinstart) << endl;

    //sort by UserID first..
    stable_sort(vpdata.begin(), vpdata.end(), compUser);
   
    time_t sortend = time(NULL);
    cout << difftime(sortend, sortstart) << endl;
   
    //UI started..
    string line;
    while (getline(cin, line)){
        istringstream sin(line);
        string s;
        sin >> s;

        switch (hash<string>()(s)){
            //get..
            case 13741091478058629623ULL:{
                int u, a, q, p, d;
                sin >> u >> a >> q >> p;
                if (sin >> d){}
                else {
                    cout << "command: get [u] [a] [q] [p] [d]" << endl;
                    break;
                }
                printstar();
                get(vpdata, u, a, q, p, d);
                printstar();
                break;
            }
            //click..
            case 17401994760632542616ULL:{
                int u;
                if (sin >> u){}
                else {
                    cout << "command: clicked [u]" << endl;
                    break;
                }
                printstar();
                clicked(vpdata, u);
                printstar();
                break;
            }
            //impressed..
            case 9871797291816109323ULL:{
                int u1, u2;
                sin >> u1;
                if (sin >> u2){}
                else {
                    cout << "command: impressed [u1] [u2]" << endl;
                    break;
                }
                printstar();
                impressed(vpdata, u1, u2);
                printstar();
                break;
            }
            //profit..
            case 975647975223515739ULL:{
                double _a, theta;
                sin >> _a;
                int a = int (_a);
                if (sin >> theta){}
                else {
                    cout << "command: profit [a] [theta]" << endl;
                    break;
                }
                printstar();
                profit(vpdata, a, theta);
                printstar();
                break;
            }
            //printdata..
            case 4727760687024200947ULL:{
                int first, last;
                if (sin >> first){
                    last = first + 1;
                    sin >> last;
                }
                else {
                    cout << "command: print [first] [last]" << endl;
                    break;
                }
                printstar();
                printdata(vpdata.data(), first, last);
                printstar();
                break;
            }
            //quit..
            case 12250438860804190241ULL:{
                return 1;
                break;
            }

            default:{
                if (s != ""){
                    cout << "command error!" << endl << ">>hash<string>(input) : " << hash<string>()(s) << endl;
                }
                break;
            }
        }
    }
    for (int i = 0; i<dataNum; ++i){
        delete data[i];
    }
    delete [] data;
}
