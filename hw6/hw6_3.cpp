extern "C"{
    #include "avl_ntudsa.h"
}

#include <iostream>
#include <cstring>

using namespace std;

class Game{
    public:
        Game() : price(0), owner(0){}
        Game(const int& p, const int& o) : price(p), owner(o){}
        operator int() const{
            return price;
        }

        int price;
        int owner;
};

int comp(const void* sa, const void* sb, void* param){
    int a = int(*(const Game*)sa);
    int b = int(*(const Game*)sb);
    if(a < b) return -1;
    else if(a == b) return 0;
    else return +1;
}

void mergeTree(avl_table* m, avl_node* p){
    if(p != NULL){
        avl_probe(m, p->avl_data);
        mergeTree(m, p->avl_link[0]);
        mergeTree(m, p->avl_link[1]);
    }
    else return;
    free(p);
}

int calK(avl_node* p, const long long int& s){
    if(p == NULL) return 0;
    if(p->avl_link[0] == NULL && p->avl_link[1] == NULL){
        return p->avl_data<=s ? 1 : 0;
    }
    if(p->avl_sum[0] <= s){
        if(p->avl_sum[0]+p->avl_data <= s){
            return p->avl_cnode[0] + 1 + calK(p->avl_link[1], s-p->avl_sum[0]-p->avl_data);
        }
        else return p->avl_cnode[0];
    }
    else return calK(p->avl_link[0], s);
}

int main(){
    int n, m;
    cin >> n >> m;
    Game* g = new Game[n+1];

    int pri;
    avl_table** friend_avlt = new avl_table*[n+1];
    for(int i=1; i<=n; ++i){
        cin >> pri;
        g[i] = Game(pri, i);

        friend_avlt[i] = avl_create(comp, NULL, NULL);
        avl_probe(friend_avlt[i], g[i]);
    }

    for(int i=0; i<m; ++i){
        int event;
        cin >> event;
        switch(event){
            case 1:{
                int i, j;
                cin >> i >> j;
                if(g[i].owner != g[j].owner){
                    for(int ii=1; ii<=n; ++ii){
                        if(g[ii].owner == g[j].owner){
                            g[ii].owner = g[i].owner;
                        }
                    }
                    if(friend_avlt[g[i].owner]->avl_count < friend_avlt[g[j].owner]->avl_count){
                        mergeTree(friend_avlt[g[j].owner], friend_avlt[g[i].owner]->avl_root);
                    }
                    else{
                        mergeTree(friend_avlt[g[i].owner], friend_avlt[g[j].owner]->avl_root);
                    }
                }
            }
            case 2:{
                int i;
                long long s;
                cin >> i >> s;
                int o = g[i].owner;
                int k = calK(friend_avlt[o]->avl_root, s);
                cout << o << " " << k << endl;
            }
        }
    }
}

