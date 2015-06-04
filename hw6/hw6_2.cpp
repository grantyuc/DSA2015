extern "C"{
    #include "avl.h"
    #include "rb.h"
    #include "bst.h"
}

#include <iostream>
#include <cstring>

using namespace std;

void print_avl_pre(const avl_node* node){
    if(node == NULL)
        return;
    cout << (char*)node->avl_data << " ";
    if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
        cout << "(";
        print_avl_pre(node->avl_link[0]);
        cout << ", ";
        print_avl_pre(node->avl_link[1]);
        cout << ")";
    }
}

void print_avl_in(const avl_node* node){
    if(node == NULL)
        return;
    print_avl_in(node->avl_link[0]);
    cout << (char*)node->avl_data << ", ";
    print_avl_in(node->avl_link[1]);
}

void print_bst_pre(const bst_node* node){
    if(node == NULL)
        return;
    cout << (char*)node->bst_data << " ";
    if(node->bst_link[0] != NULL || node->bst_link[1] != NULL){
        cout << "(";
        print_bst_pre(node->bst_link[0]);
        cout << ", ";
        print_bst_pre(node->bst_link[1]);
        cout << ")";
    }
}

void print_bst_in(const bst_node* node){
    if(node == NULL)
        return;
    print_bst_in(node->bst_link[0]);
    cout << (char*)node->bst_data << ", ";
    print_bst_in(node->bst_link[1]);
}

void print_rb_pre(const rb_node* node){
    if(node == NULL)
        return;
    cout << (char*)node->rb_data << " ";
    if(node->rb_link[0] != NULL || node->rb_link[1] != NULL){
        cout << "(";
        print_rb_pre(node->rb_link[0]);
        cout << ", ";
        print_rb_pre(node->rb_link[1]);
        cout << ")";
    }
}

void print_rb_in(const rb_node* node){
    if(node == NULL)
        return;
    print_rb_in(node->rb_link[0]);
    cout << (char*)node->rb_data << ", ";
    print_rb_in(node->rb_link[1]);
}

int lexicographComp(const void* sa, const void* sb, void* param){
    return strcmp((const char*)sa, (const char*)sb);
}

int main(){
    //read in 32 strings
    char s[32][128];
    for(int i = 0; i<32; ++i){
        cin.getline(s[i], 128);
    }

    //build avl tree
    avl_table* avlt;
    avlt = avl_create(lexicographComp, NULL, NULL);

    for(int i = 0; i<32; ++i){
        avl_probe(avlt, s[i]);
    }
    print_avl_pre(avlt->avl_root);
    cout << endl;

    //build bst
    bst_table* bst;
    bst = bst_create(lexicographComp, NULL, NULL);

    for(int i = 0; i<32; ++i){
        bst_probe(bst, s[i]);
    }
    print_bst_pre(bst->bst_root);
    cout << endl;

    //build rbt
    rb_table* rbt;
    rbt = rb_create(lexicographComp, NULL, NULL);

    for(int i = 0; i<32; ++i){
        rb_probe(rbt, s[i]);
    }
    print_rb_pre(rbt->rb_root);
    cout << endl;
}
