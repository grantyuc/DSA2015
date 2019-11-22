#include "User.h"

int maxLenDiffWithScore(const int& score){
    return floor(-0.5 + sqrt(2*score+0.25));
}

unsigned int History::time = 0;

map<string, User*> User::usermp;  //map id to the user

void User::transferTo(User& u, const int& dollars){
    this->withdraw(dollars);
    u.deposit(dollars);
    History* history = new History(this->getId(), u.getId(), dollars);
    this->historymp[u.getId()][history->timeStamp] = history;
    u.historymp[this->id][history->timeStamp] = history;
}

void User::merge(User& u){                          //merge A <- B
    string thisid = this->getId();
    string uid = u.getId();
    for(auto it = u.historymp.begin(); it != u.historymp.end(); ++it){
        string dealerId = it->first;
        for(auto iit = it->second.begin(); iit != it->second.end(); ++iit){
            History* curHistory = iit->second;
            if(curHistory->id1Is(uid) && curHistory->id2Is(uid)){ // B transfer to B
                curHistory->id1 = thisid;        
                curHistory->id2 = thisid;        
                continue;
            }           
            User* x;
            if(curHistory->id1Is(uid)){           // B transfer to X
                auto findit = usermp.find(curHistory->id2);
                if(findit != usermp.end()){
                    x = findit->second;
                    if(x->historymp.find(uid) != x->historymp.end() && x->historymp[uid].find(curHistory->timeStamp) != x->historymp[uid].end()){
                        (x->historymp)[thisid][curHistory->timeStamp] = (x->historymp)[uid][curHistory->timeStamp];
                        (x->historymp)[uid].erase(curHistory->timeStamp);
                    }
                }
                curHistory->id1 = thisid;        
                if(curHistory->id2Is(thisid)){   // B transfer to X=A
                    curHistory->flag = -1;
                }
            }
            else if(curHistory->id2Is(uid)){      // X transfer to B
                auto findit = usermp.find(curHistory->id1);
                if(findit != usermp.end()){
                    x = findit->second;
                    if(x->historymp.find(uid) != x->historymp.end() && x->historymp[uid].find(curHistory->timeStamp) != x->historymp[uid].end()){
                        (x->historymp)[thisid][curHistory->timeStamp] = (x->historymp)[uid][curHistory->timeStamp];
                        (x->historymp)[uid].erase(curHistory->timeStamp);
                    }
                }
                curHistory->id2 = thisid;
                if(curHistory->id1Is(thisid)){   // X=A transfer to B
                    curHistory->flag = +1;
                }
            }
        }
        if(dealerId != uid /*&& dealerId != thisid*/){
            this->historymp[dealerId].insert(u.historymp[dealerId].begin(), u.historymp[dealerId].end());
        }
        else if(dealerId == uid)
            this->historymp[thisid].insert(u.historymp[dealerId].begin(), u.historymp[dealerId].end());
    }
    this->deposit( u.getMoney() );
    u.remove();
}

void User::printHistory(){
    if( historymp.empty() ){
        cout << "no record" << endl;
    }
    else{
        for(auto it = historymp.begin(); it != historymp.end(); ++it){
            for(auto iit = it->second.begin(); iit != it->second.end(); ++iit){
                History* curHistory = iit->second;
                if(curHistory->flag == 0){
                    if(curHistory->id1Is(id)){
                        cout << "To " << curHistory->id2 << " ";
                    }
                    else if(curHistory->id2Is(id)){
                        cout << "From " << curHistory->id1 << " ";
                    }
                    cout << curHistory->money << endl;
                }
                else if(curHistory->flag == +1){
                    cout << "To " << curHistory->id2 << " ";
                    cout << curHistory->money << endl;
                    cout << "From " << curHistory->id1 << " ";
                    cout << curHistory->money << endl;
                }
                else{
                    cout << "From " << curHistory->id1 << " ";
                    cout << curHistory->money << endl;
                    cout << "To " << curHistory->id2 << " ";
                    cout << curHistory->money << endl;
                }
            }
            cout << endl;
        }
    }
}

void User::printHistory(string id){
    if( historymp[id].empty() ){
        cout << "no record" << endl;
    }
    else{
        for(auto it = historymp[id].begin(); it != historymp[id].end(); ++it){
            History* curHistory = it->second;
            if(curHistory->flag == 0){
                if(curHistory->id1Is(this->id)){
                    cout << "To " << curHistory->id2 << " ";
                }
                else if(curHistory->id2Is(this->id)){
                    cout << "From " << curHistory->id1 << " ";
                }
                cout << curHistory->money << endl;
            }
            else if(curHistory->flag == +1){
                cout << "To " << curHistory->id2 << " ";
                cout << curHistory->money << endl;
                cout << "From " << curHistory->id1 << " ";
                cout << curHistory->money << endl;
            }
            else{
                cout << "From " << curHistory->id1 << " ";
                cout << curHistory->money << endl;
                cout << "To " << curHistory->id2 << " ";
                cout << curHistory->money << endl;
            }
        }
    }
}

int Score(const string& idbase, const string& idscore)
{
	int len;
    int dl;
	int score = 0;
	int iter = 0;
	if (idbase.length() < idscore.length()) 
	{
		len = idbase.length();
		dl = idscore.length() - len;
	}
	else
	{
		len = idscore.length();
		dl = idbase.length() - len;
	}
    score += dl*(dl+1)/2;
	//算相同程度的score  不包含長度時（不太會寫先寫這樣看看）
	for(int i = len-1; i >= 0; i--)
	{
		iter++;
		if(idbase[i] != idscore[i]) score+=iter;
	}
	return score;
}

char chart[63];
void makechart()
{
    for(int i=0 ; i<10 ; i++)
	    chart[i]='0'+i;
    for(int i=0 ; i<26 ; i++)
    	chart[i+10]='A'+i;
    for(int i=0 ; i<26 ; i++)
    	chart[i+36]='a'+i;
    chart[62]='*';
}

void User::recommend(const string& id){
	vector<string> Q[5000];

	for(auto it = User::usermp.begin(); it != User::usermp.end(); ++it){
		Q[Score(it->second->getId(), id)].push_back( it->second->getId() ); 	
	}

	int count = 0;
    bool reach10 = false;
	for(int i=1; i<5000; ++i){
		for(auto it=Q[i].begin(); it!=Q[i].end(); ++it){
            Q[0].push_back(*it);
			count++;
			if(count == 10){
                reach10 = true;
				break;
			}
		}
        if(reach10) break;
    }
    //sort(Q[0].begin(), Q[0].end());
    cout << Q[0].at(0);
    for(unsigned int i=1; i<Q[0].size(); ++i)
	    cout << "," << Q[0].at(i);
    cout << endl;
	return;
}

void User::create_suggest(const string& id)
{ 
    makechart();
	vector<string> Q;

	for(auto it = User::usermp.begin(); it != User::usermp.end(); ++it)
	{
		if(Score(id, it->second->getId()) == 1)
		Q.push_back( it->second->getId() ); 	
	} 

	int count = 0;
	//score 1
    string chosen = id.substr(0, id.length()-1);

	if(!binary_search(Q.begin(), Q.end(), chosen) && chosen!="" ){
		if(count < 9){
            cout << chosen << ",";
            ++count; 
        }
		else {
            cout << chosen << endl; 
            ++count; 
            return;
        }
	}
	
	for(int i=0 ; i<62; ++i)
    {	
        chosen += chart[i];
		if(chart[i] == id[id.length() -1 ])
		{
			for(int j=0 ; j<62; ++j)
			{
                chosen += chart[j];

				if(!binary_search(Q.begin(), Q.end(), chosen)){
					if(count < 9){
                        cout << chosen << ",";
                        ++count; 
                    }
					else {
                        cout << chosen << endl; 
                        ++count; 
                        return;
                    }
				}
                chosen.pop_back();
			}
		}

		if(!binary_search(Q.begin(), Q.end(), chosen)){
			if(count < 9){
                cout << chosen << ",";
                ++count; 
            }
			else {
                cout << chosen << endl; 
                ++count; 
                return;
            }
	    }			
        chosen.pop_back();
    }
    cout << endl;	
}
