/*regex_search */
#include <string>
#include <regex>
#include <iostream>
using namespace std;	
int regex_find(string wildcardID, string ID){
		string s (ID);   //IDs to be searched
		smatch m;
		regex wID (wildcardID);   //regex words
	if(regex_search(s,m,wID)){
		cout << m[0] ;
		s = m.suffix().str();
		
		while (regex_search(s,m,wID)){      //print out other matched IDs
			cout <<','<< m[0] ;
			s=m.suffix().str();
		}
	}else cout<<endl; return 0;		//cout "\n" if not found
		return 0;
}
 
string   IDreplace(string wildID, const string olds, const string news){                  //replace olds with news in wildID 
    for(string::size_type pos(0); pos!=string::npos; pos+=news.length()){   
        if(   (pos=wildID.find(olds,pos))!=string::npos   )   
            wildID.replace(pos,olds.length(),news);   
        else   break;   
    }   
    return   wildID;   
}   
